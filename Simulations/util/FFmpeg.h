#ifndef __FFmpeg_h__
#define __FFmpeg_h__

#include <stdio.h>
#include <cstdint>
#include <vector>
#include <Windows.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

// Simple FFmpeg-based video recorder (http://www.ffmpeg.org) for directly
// writing out h264 encoded videos.
// Only works if "ffmpeg.exe" can be found (e.g. in system path), 
// else all calls will return immediately and this class won't do anything.
// FFmpeg windows binaries can be downloaded here: http://ffmpeg.zeranoe.com/builds/
class FFmpeg
{
public:
    // FFmpeg Recording mode
    enum MODE
    {
        MODE_FRAME_COPY,  // Every added frame is copied to a single video frame
        MODE_DUPLICATE,   // Realtime recording:   duplicate and skip added frames as necessary (faster than MODE_INTERPOLATE)
        MODE_INTERPOLATE, // Realtime recording: interpolate and skip added frames as necessary
    };

    // Create FFmpeg video recorder
    // fps: fps of output video
    // crf: constant rate factor
    //      The range of the quantizer scale is 0-51: where 0 is lossless, 23 is codec default,
    //      and 51 is worst possible. A lower value is a higher quality and a subjectively
    //      sane range is 18-28. Consider 18 to be visually lossless or nearly so.
    // mode: recording mode
    FFmpeg(int fps = 24, int crf = 21, MODE mode = MODE_FRAME_COPY);
    ~FFmpeg();

    // Start recording to 'filename' by spawning a ffmpeg subprocess.
    // Render target view needed to query information about render target (like e.g. resolution).
    HRESULT StartRecording(ID3D11Device* pd3dDevice, ID3D11RenderTargetView* pRenderTargetView, char* filename);

    // Add frame from given render target view
    HRESULT AddFrame(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRenderTargetView);
    
    // Stop recording and stop ffmpeg subprocess
    void StopRecording();

private:
    FILE*                m_pFILE;
    ID3D11Texture2D*     m_pStaging;
    std::vector<uint8_t> m_buffer[3];
    LARGE_INTEGER        m_timestamp[2];
    LARGE_INTEGER        m_frequency;
    LARGE_INTEGER        m_startTime;
    int                  m_width;
    int                  m_height;
    MODE                 m_mode;
    int                  m_fps;
    int                  m_crf;
    int                  m_frame;

    static bool ms_bFFmpegInstalled;
};

#endif
