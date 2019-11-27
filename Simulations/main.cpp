#include <sstream>
#include <iomanip>
#include <random>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
// DrawingUtilitiesClass
#include "DrawingUtilitiesClass.h"
// Simulator
#include "Simulator.h"
// DirectX includes
#include <DirectXMath.h>
#include "util/vectorbase.h"
// Internal includes
#include "util/util.h"
#include "util/FFmpeg.h"

using namespace DirectX;
using namespace GamePhysics;

//#define ADAPTIVESTEP

//#define TEMPLATE_DEMO
//#define MASS_SPRING_SYSTEM
#define RIGID_BODY_SYSTEM
//#define SPH_SYSTEM

#ifdef TEMPLATE_DEMO
#include "TemplateSimulator.h"
#endif
#ifdef MASS_SPRING_SYSTEM
#include "MassSpringSystemSimulator.h"
#endif
#ifdef RIGID_BODY_SYSTEM
#include "RigidBodySystemSimulator.h"
#endif
#ifdef SPH_SYSTEM
//#include "SPHSystemSimulator.h"
#endif

DrawingUtilitiesClass * g_pDUC;
Simulator * g_pSimulator;
float 	g_fTimestep = 0.001;
#ifdef ADAPTIVESTEP
float   g_fTimeFactor = 1;
#endif
bool  g_bDraw = true;
int g_iTestCase = 0;
int g_iPreTestCase = -1;
bool  g_bSimulateByStep = false;
bool firstTime = true;
// Video recorder
FFmpeg* g_pFFmpegVideoRecorder = nullptr;


void initTweakBar(){
	g_pDUC->g_pTweakBar = TwNewBar("TweakBar");
	TwDefine(" TweakBar color='0 128 128' alpha=128 ");
	TwType TW_TYPE_TESTCASE = TwDefineEnumFromString("Test Scene", g_pSimulator->getTestCasesStr());
	TwAddVarRW(g_pDUC->g_pTweakBar, "Test Scene", TW_TYPE_TESTCASE, &g_iTestCase, "");
	// HINT: For buttons you can directly pass the callback function as a lambda expression.
	TwAddButton(g_pDUC->g_pTweakBar, "Reset Scene", [](void * s){ g_iPreTestCase = -1; }, nullptr, "");
	TwAddButton(g_pDUC->g_pTweakBar, "Reset Camera", [](void * s){g_pDUC->g_camera.Reset();}, nullptr,"");
	// Run mode, step by step, control by space key
	TwAddVarRW(g_pDUC->g_pTweakBar, "RunStep", TW_TYPE_BOOLCPP, &g_bSimulateByStep, "");
	TwAddVarRW(g_pDUC->g_pTweakBar, "Draw Simulation",  TW_TYPE_BOOLCPP, &g_bDraw, "");
	TwAddVarRW(g_pDUC->g_pTweakBar, "Timestep", TW_TYPE_FLOAT, &g_fTimestep, "step=0.0001 min=0.0001");
#ifdef ADAPTIVESTEP
	TwAddVarRW(g_pDUC->g_pTweakBar, "Time Factor", TW_TYPE_FLOAT, &g_fTimeFactor, "step=0.01   min=0.01");
#endif
}


// ============================================================
// DXUT Callbacks
// ============================================================


//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D11DeviceAcceptable( const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
                                       DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependent on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11CreateDevice( ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	
	HRESULT hr;
    ID3D11DeviceContext* pd3dImmediateContext = DXUTGetD3D11DeviceContext();
	g_pDUC->init(pd3dDevice,pd3dImmediateContext);
    std::wcout << L"Device: " << DXUTGetDeviceStats() << std::endl;
    // Load custom effect from "effect.fxo" (compiled "effect.fx")
	std::wstring effectPath = GetExePath() + L"effect.fxo";
	if(FAILED(hr = D3DX11CreateEffectFromFile(effectPath.c_str(), 0, pd3dDevice, &(g_pDUC->g_pEffect))))
	{
        std::wcout << L"Failed creating effect with error code " << int(hr) << std::endl;
		return hr;
	}
    // Init AntTweakBar GUI
	TwInit(TW_DIRECT3D11, pd3dDevice);
	g_pDUC->g_pTweakBar = TwNewBar("TweakBar");
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11DestroyDevice( void* pUserContext )
{
	g_pDUC->destroy();
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                                          const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	g_pDUC->updateScreenSize(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11ReleasingSwapChain( void* pUserContext )
{
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    HRESULT hr;

	if(bKeyDown)
	{
		switch(nChar)
		{
            // RETURN: toggle fullscreen
			case VK_RETURN :
			{
				if(bAltDown) DXUTToggleFullScreen();
				break;
			}
            // F8: Take screenshot
			case VK_F8:
			{
                // Save current render target as png
                static int nr = 0;
				std::wstringstream ss;
				ss << L"Screenshot" << std::setfill(L'0') << std::setw(4) << nr++ << L".png";

                ID3D11Resource* pTex2D = nullptr;
                DXUTGetD3D11RenderTargetView()->GetResource(&pTex2D);
                SaveWICTextureToFile(DXUTGetD3D11DeviceContext(), pTex2D, GUID_ContainerFormatPng, ss.str().c_str());
                SAFE_RELEASE(pTex2D);

                std::wcout << L"Screenshot written to " << ss.str() << std::endl;
				break;
			}
            // F10: Toggle video recording
            case VK_F10:
            {
                if (!g_pFFmpegVideoRecorder) {
                    g_pFFmpegVideoRecorder = new FFmpeg(25, 21, FFmpeg::MODE_INTERPOLATE);
                    V(g_pFFmpegVideoRecorder->StartRecording(DXUTGetD3D11Device(), DXUTGetD3D11RenderTargetView(), "output.avi"));
                } else {
                    g_pFFmpegVideoRecorder->StopRecording();
                    SAFE_DELETE(g_pFFmpegVideoRecorder);
                }
            }			    
		}
	}
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
                       bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
                       int xPos, int yPos, void* pUserContext )
{
	if (bLeftButtonDown)
	{
		g_pSimulator->onClick(xPos,yPos);
	}
	else
	{
		g_pSimulator->onMouse(xPos, yPos);
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Send message to AntTweakbar first
    if (TwEventWin(hWnd, uMsg, wParam, lParam))
    {
        *pbNoFurtherProcessing = true;
        return 0;
    }

    // If message not processed yet, send to camera
	if(g_pDUC->g_camera.HandleMessages(hWnd,uMsg,wParam,lParam))
    {
        *pbNoFurtherProcessing = true;
		return 0;
    }

	return 0;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double dTime, float fElapsedTime, void* pUserContext )
{
	UpdateWindowTitle(L"Demo");
	g_pDUC->update(fElapsedTime);
	if (g_iPreTestCase != g_iTestCase){// test case changed
		// clear old setup and build up new setup
		if(g_pDUC->g_pTweakBar != nullptr) {
			TwDeleteBar(g_pDUC->g_pTweakBar);
			g_pDUC->g_pTweakBar = nullptr;
		}
		initTweakBar();
		g_pSimulator->notifyCaseChanged(g_iTestCase);
		g_pSimulator->initUI(g_pDUC);
		g_iPreTestCase = g_iTestCase;
	}
	if(!g_bSimulateByStep){
#ifdef ADAPTIVESTEP
		g_pSimulator->externalForcesCalculations(fElapsedTime);
		static float timeAcc = 0;
		timeAcc += fElapsedTime * g_fTimeFactor;
		int maxIter = 10;
		while (timeAcc > g_fTimestep)
		{
			if (maxIter > 0)
			{
				g_pSimulator->simulateTimestep(g_fTimestep);
				maxIter--;
			}
			timeAcc -= g_fTimestep;
		}
#else
		g_pSimulator->externalForcesCalculations(g_fTimestep);
		g_pSimulator->simulateTimestep(g_fTimestep);
#endif
	}else{
		if(DXUTIsKeyDown(VK_SPACE))
			g_pSimulator->simulateTimestep(g_fTimestep);
		if(DXUTIsKeyDown('S') && firstTime)
		{
			g_pSimulator->simulateTimestep(g_fTimestep);
			firstTime = false;
		}else{
			if(!DXUTIsKeyDown('S')) 
				firstTime = true;
		}
	}
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D11FrameRender( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
                                  double fTime, float fElapsedTime, void* pUserContext )
{
    HRESULT hr;

	// Clear render target and depth stencil
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearRenderTargetView( pRTV, ClearColor );
	pd3dImmediateContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );

    // Draw floor
    g_pDUC->DrawFloor(pd3dImmediateContext);

    // Draw axis box
     g_pDUC->DrawBoundingBox(pd3dImmediateContext);

	// Draw Simulator
	if(g_bDraw)g_pSimulator->drawFrame(pd3dImmediateContext);

	// Draw GUI
    TwDraw();

    if (g_pFFmpegVideoRecorder) 
    {
        V(g_pFFmpegVideoRecorder->AddFrame(pd3dImmediateContext, DXUTGetD3D11RenderTargetView()));
    }
}

//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable run-time memory check for debug builds.
	// (on program exit, memory leaks are printed to Visual Studio's Output console)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

#ifdef _DEBUG
	std::wcout << L"---- DEBUG BUILD ----\n\n";
#endif

	// Set general DXUT callbacks
	DXUTSetCallbackMsgProc( MsgProc );
	DXUTSetCallbackMouse( OnMouse, true );
	DXUTSetCallbackKeyboard( OnKeyboard );

	DXUTSetCallbackFrameMove( OnFrameMove );
	DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

	// Set the D3D11 DXUT callbacks
	DXUTSetCallbackD3D11DeviceAcceptable( IsD3D11DeviceAcceptable );
	DXUTSetCallbackD3D11DeviceCreated( OnD3D11CreateDevice );
	DXUTSetCallbackD3D11SwapChainResized( OnD3D11ResizedSwapChain );
	DXUTSetCallbackD3D11FrameRender( OnD3D11FrameRender );
	DXUTSetCallbackD3D11SwapChainReleasing( OnD3D11ReleasingSwapChain );
	DXUTSetCallbackD3D11DeviceDestroyed( OnD3D11DestroyDevice );
	// Init Drawing Class
	g_pDUC = new DrawingUtilitiesClass();
    // Init camera
 	XMFLOAT3 eye(0.0f, 0.0f, -2.0f);
	XMFLOAT3 lookAt(0.0f, 0.0f, 0.0f);
	g_pDUC->g_camera.SetViewParams(XMLoadFloat3(&eye), XMLoadFloat3(&lookAt));
	g_pDUC-> g_camera.SetButtonMasks(MOUSE_MIDDLE_BUTTON, MOUSE_WHEEL, MOUSE_RIGHT_BUTTON);


#ifdef TEMPLATE_DEMO
	g_pSimulator= new TemplateSimulator();
#endif
#ifdef MASS_SPRING_SYSTEM
	g_pSimulator= new MassSpringSystemSimulator();
#endif
#ifdef RIGID_BODY_SYSTEM
	g_pSimulator= new RigidBodySystemSimulator();
#endif
#ifdef SPH_SYSTEM
	//g_pSimulator= new SPHSystemSimulator();
#endif
	g_pSimulator->reset();

    // Init DXUT and create device
	DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
	//DXUTSetIsInGammaCorrectMode( false ); // true by default (SRGB backbuffer), disable to force a RGB backbuffer
	DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
	DXUTCreateWindow( L"Demo" );
	DXUTCreateDevice( D3D_FEATURE_LEVEL_11_0, true, 1280, 960 );
    
	DXUTMainLoop(); // Enter into the DXUT render loop
	DXUTShutdown(); // Shuts down DXUT (includes calls to OnD3D11ReleasingSwapChain() and OnD3D11DestroyDevice())
	
	return DXUTGetExitCode();
}
