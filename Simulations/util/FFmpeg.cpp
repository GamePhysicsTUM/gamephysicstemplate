#include "FFmpeg.h"

#include <iostream>
#include <sstream>
#include <d3d11_1.h>

bool FFmpeg::ms_bFFmpegInstalled = false;

FFmpeg::FFmpeg(int fps, int crf, MODE mode)
 : m_pFILE(nullptr),
   m_pStaging(nullptr),
   m_width(-1),
   m_height(-1),
   m_mode(mode),
   m_fps(fps),
   m_crf(crf),
   m_frame(-1)
{
    static bool s_checkPerformed = false;

    m_timestamp[0].QuadPart = m_timestamp[1].QuadPart = 0;
    m_frequency.QuadPart = 0;
    m_startTime.QuadPart = 0;

    // Check once if ffmpeg.exe is installed, i.e. if it's in system path
    if (!s_checkPerformed) 
    {
        FILE* file = _popen("ffmpeg -version", "rt");
        if (_pclose(file) == 0)
        {
            ms_bFFmpegInstalled = true;
        }
        else 
        {
            ms_bFFmpegInstalled = false;
            std::cout << "Warning: ffmpeg.exe not found, video recording disabled..." << std::endl;
        }

        s_checkPerformed = true;
    }
}

FFmpeg::~FFmpeg()
{
    StopRecording();
}


HRESULT FFmpeg::StartRecording(ID3D11Device* pd3dDevice, ID3D11RenderTargetView* pRenderTargetView, char* filename)
{
    if (!ms_bFFmpegInstalled) { return S_OK; }

    HRESULT hr;
    std::stringstream cmd;

    // 1) Create a staging texture from the non-MSAA source
    ID3D11Resource* pResource = nullptr;
    pRenderTargetView->GetResource(&pResource);

    ID3D11Texture2D* pTexture = nullptr;
    hr = pResource->QueryInterface( __uuidof(ID3D11Texture2D), (void**) &pTexture );
    if (hr != S_OK) { goto StartRecording_Return; }
                    
    D3D11_TEXTURE2D_DESC desc;
    pTexture->GetDesc( &desc );

    if (desc.Format != DXGI_FORMAT_R8G8B8A8_TYPELESS   &&
        desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM      &&
        desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM_SRGB &&
        desc.Format != DXGI_FORMAT_R8G8B8A8_UINT       &&
        desc.Format != DXGI_FORMAT_R8G8B8A8_SNORM      &&
        desc.Format != DXGI_FORMAT_R8G8B8A8_SINT)
    {
        std::cout << "FFmpeg error: Currently only R8G8B8A8 render targets are supported." << std::endl;
        hr = E_INVALIDARG;
        goto StartRecording_Return;
    }

    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.Usage = D3D11_USAGE_STAGING;

    hr = pd3dDevice->CreateTexture2D( &desc, 0, &m_pStaging );
    if (hr != S_OK) { goto StartRecording_Return; }

    // 2) Spawn ffmpeg process    
    // For h264 control options, see https://trac.ffmpeg.org/wiki/Encode/H.264
    cmd << "ffmpeg "
           "-f rawvideo -pix_fmt rgba " // raw rgba input video format
           "-s " << desc.Width << "x" << desc.Height << " " // input resolution
           "-r " << m_fps << " " // input framerate
           " -i - " // read input from stdin
           "-y " // auto-overwrite output file
           "-codec:video libx264 " // use h264 codec
           "-preset fast " // encoder preset (ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow)
           "-crf " << m_crf << " " // constant rate factor
        << filename;

    // open pipe to ffmpeg's stdin in binary write mode
    m_pFILE = _popen(cmd.str().c_str(), "wb");

    // 3) Allocate buffers
    m_width  = desc.Width;
    m_height = desc.Height;
    m_buffer[0].resize(m_width * m_height * sizeof(uint32_t));
    m_buffer[1].resize(m_width * m_height * sizeof(uint32_t));
    m_buffer[2].resize(m_width * m_height * sizeof(uint32_t));
    m_frame = -1;

    StartRecording_Return:
    if (pResource) {pResource->Release(); pResource = nullptr; }
    if (pTexture ) {pTexture ->Release(); pTexture  = nullptr; }
    return hr;   
}


void FFmpeg::StopRecording()
{
    // Release everything and clean up
    if (!ms_bFFmpegInstalled) { return; }

    if (m_pFILE) { _pclose(m_pFILE); m_pFILE = nullptr; }
    
    if (m_pStaging) {m_pStaging ->Release(); m_pStaging  = nullptr; }

    m_buffer[0].swap(std::vector<uint8_t>());
    m_buffer[1].swap(std::vector<uint8_t>());
    m_buffer[2].swap(std::vector<uint8_t>());
    m_timestamp[0].QuadPart = m_timestamp[1].QuadPart = 0;
    m_frequency.QuadPart = 0;
    m_startTime.QuadPart = 0;
    
    m_width  = -1;
    m_height = -1;
    m_frame  = -1;
}

HRESULT FFmpeg::AddFrame(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRenderTargetView)
{
    if (!ms_bFFmpegInstalled) { return S_OK; }
        
    HRESULT hr;

    // 1) Copy render target to staging texture
    ID3D11Resource* pTex2D = nullptr;
    pRenderTargetView->GetResource(&pTex2D);
    pd3dContext->CopyResource(m_pStaging, pTex2D);
    pTex2D->Release();

    // 2) Download staging texture from GPU to buffer
    m_buffer[0].swap(m_buffer[1]);
    uint32_t* pixels = (uint32_t*)(m_buffer[1].data());

    D3D11_MAPPED_SUBRESOURCE mapped;
    hr = pd3dContext->Map(m_pStaging, 0, D3D11_MAP_READ, 0, &mapped);
    if (hr == S_OK)
    {
        for (int y = 0; y < m_height; y++)
        {
            uint32_t* pRowData = (uint32_t*)mapped.pData + y * (mapped.RowPitch / sizeof(uint32_t));
            for (int x = 0; x < m_width; x++)
            {
                pixels[y * m_width + x] = pRowData[x];
            }
        }

        pd3dContext->Unmap(m_pStaging, 0);
    }
        
    // 3) Query timestamps and write first frame to ffmpeg
    if (m_frame == -1)
    {
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_startTime);
        m_timestamp[0].QuadPart = m_timestamp[1].QuadPart = 0;
        
        fwrite(m_buffer[1].data(), 1, m_buffer[1].size(), m_pFILE);
        m_frame ++;
    }
    else
    {
        m_timestamp[0] = m_timestamp[1];
        QueryPerformanceCounter(&m_timestamp[1]);
        m_timestamp[1].QuadPart -= m_startTime.QuadPart;
    }

    // 4) Write frame(s) to ffmpeg depending on recording mode
    if (m_mode == MODE_FRAME_COPY)
    {
        fwrite(m_buffer[1].data(), 1, m_buffer[1].size(), m_pFILE);
    }
    else
    {
        while (double(m_frame + 1) * m_frequency.QuadPart / m_fps <= m_timestamp[1].QuadPart)
        {
            if (m_mode == MODE_INTERPOLATE)
            {
                double t = (double(m_frame + 1) * m_frequency.QuadPart / m_fps - m_timestamp[0].QuadPart)
                         / (m_timestamp[1].QuadPart - m_timestamp[0].QuadPart);

                // interpolate
                for (size_t i = 0; i < m_buffer[0].size(); i++)
                {
                    m_buffer[2][i] = uint8_t((1 - t) * m_buffer[0][i] + t * m_buffer[1][i]);
                }    

                fwrite(m_buffer[2].data(), 1, m_buffer[2].size(), m_pFILE);
            }
            else if (m_mode == MODE_DUPLICATE)
            {
                fwrite(m_buffer[1].data(), 1, m_buffer[1].size(), m_pFILE);
            }

            m_frame ++;
        }
    }

    return hr;
}

