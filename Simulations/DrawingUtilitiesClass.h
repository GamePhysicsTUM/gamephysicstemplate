#ifndef	DRAWINGUTILITIESCLASS_h
#define DRAWINGUTILITIESCLASS_h

// DirectXEffects
#include <d3dx11effect.h>
// DXUT includes
#include <DXUT.h>
#include <DXUTcamera.h>
// DirectXTK includes
#include "Effects.h"
#include "VertexTypes.h"
#include "PrimitiveBatch.h"
#include "GeometricPrimitive.h"
#include "ScreenGrab.h"
// AntTweakBar includes
#include "AntTweakBar.h"
// vector mat quat
#include "util/vectorbase.h"
#include "util/matrixbase.h"
#include "util/quaternion.h"
using namespace DirectX;
using namespace GamePhysics;

class DrawingUtilitiesClass{

public:
ID3D11Device* g_ppd3Device;
ID3D11DeviceContext* g_pd3dImmediateContext;
// Effect corresponding to "effect.fx"
ID3DX11Effect* g_pEffect;
// Tweak bar
TwBar* g_pTweakBar;
// DXUT camera
// NOTE: CModelViewerCamera does not only manage the standard view transformation/camera position 
//       (CModelViewerCamera::GetViewMatrix()), but also allows for model rotation
//       (CModelViewerCamera::GetWorldMatrix()). 
//       Look out for CModelViewerCamera::SetButtonMasks(...).
CModelViewerCamera g_camera;

// DirectXTK effect, input layout and primitive batch for position/color vertices
// (for drawing multicolored & unlit primitives)
BasicEffect*                          g_pEffectPositionColor;
ID3D11InputLayout*                    g_pInputLayoutPositionColor;
PrimitiveBatch<VertexPositionColor>*  g_pPrimitiveBatchPositionColor;

// DirectXTK effect, input layout and primitive batch for position/normal vertices
// (for drawing unicolor & oriented & lit primitives)
BasicEffect*                          g_pEffectPositionNormal;
ID3D11InputLayout*                    g_pInputLayoutPositionNormal;
PrimitiveBatch<VertexPositionNormal>* g_pPrimitiveBatchPositionNormal;

BasicEffect*                               g_pEffectPositionNormalColor;
ID3D11InputLayout*                         g_pInputLayoutPositionNormalColor;
PrimitiveBatch<VertexPositionNormalColor>* g_pPrimitiveBatchPositionNormalColor;

// DirectXTK simple geometric primitives
std::unique_ptr<GeometricPrimitive> g_pSphere;
std::unique_ptr<GeometricPrimitive> g_pTeapot;

// Constructor
DrawingUtilitiesClass(){
	g_pTweakBar = nullptr;
}


void init(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	g_pd3dImmediateContext = pd3dImmediateContext;
	g_ppd3Device = pd3dDevice;
	// Create DirectXTK geometric primitives for later usage
    g_pSphere = GeometricPrimitive::CreateGeoSphere(g_pd3dImmediateContext, 2.0f, 2, false);
    g_pTeapot = GeometricPrimitive::CreateTeapot(g_pd3dImmediateContext, 1.5f, 8, false);

    // Create effect, input layout and primitive batch for position/color vertices (DirectXTK)
    {
        // Effect
        g_pEffectPositionColor = new BasicEffect(pd3dDevice);
        g_pEffectPositionColor->SetVertexColorEnabled(true); // triggers usage of position/color vertices

        // Input layout
        void const* shaderByteCode;
        size_t byteCodeLength;
        g_pEffectPositionColor->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
        
        pd3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
                                      VertexPositionColor::InputElementCount,
                                      shaderByteCode, byteCodeLength,
                                      &g_pInputLayoutPositionColor);

        // Primitive batch
        g_pPrimitiveBatchPositionColor = new PrimitiveBatch<VertexPositionColor>(pd3dImmediateContext);
    }

    // Create effect, input layout and primitive batch for position/normal vertices (DirectXTK)
    {
        // Effect
        g_pEffectPositionNormal = new BasicEffect(pd3dDevice);
        g_pEffectPositionNormal->EnableDefaultLighting(); // triggers usage of position/normal vertices
        g_pEffectPositionNormal->SetPerPixelLighting(true);

        // Input layout
        void const* shaderByteCode;
        size_t byteCodeLength;
        g_pEffectPositionNormal->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        pd3dDevice->CreateInputLayout(VertexPositionNormal::InputElements,
                                      VertexPositionNormal::InputElementCount,
                                      shaderByteCode, byteCodeLength,
                                      &g_pInputLayoutPositionNormal);

        // Primitive batch
        g_pPrimitiveBatchPositionNormal = new PrimitiveBatch<VertexPositionNormal>(pd3dImmediateContext);
    }

    // Create effect, input layout and primitive batch for position/normal/color vertices (DirectXTK)
    {
        // Effect
        g_pEffectPositionNormalColor = new BasicEffect(pd3dDevice);
        g_pEffectPositionNormalColor->SetPerPixelLighting(true);
        g_pEffectPositionNormalColor->EnableDefaultLighting();     // triggers usage of position/normal/color vertices
        g_pEffectPositionNormalColor->SetVertexColorEnabled(true); // triggers usage of position/normal/color vertices

        // Input layout
        void const* shaderByteCode;
        size_t byteCodeLength;
        g_pEffectPositionNormalColor->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        pd3dDevice->CreateInputLayout(VertexPositionNormalColor::InputElements,
                                      VertexPositionNormalColor::InputElementCount,
                                      shaderByteCode, byteCodeLength,
                                      &g_pInputLayoutPositionNormalColor);

        // Primitive batch
        g_pPrimitiveBatchPositionNormalColor = new PrimitiveBatch<VertexPositionNormalColor>(pd3dImmediateContext);
    }
}

void destroy(){
    SAFE_DELETE (g_pPrimitiveBatchPositionColor);
    SAFE_RELEASE(g_pInputLayoutPositionColor);
    SAFE_DELETE (g_pEffectPositionColor);

    SAFE_DELETE (g_pPrimitiveBatchPositionNormal);
    SAFE_RELEASE(g_pInputLayoutPositionNormal);
    SAFE_DELETE (g_pEffectPositionNormal);

    SAFE_DELETE (g_pPrimitiveBatchPositionNormalColor);
    SAFE_RELEASE(g_pInputLayoutPositionNormalColor);
    SAFE_DELETE (g_pEffectPositionNormalColor);
	
    g_pSphere.reset();
    g_pTeapot.reset();

	SAFE_RELEASE(g_pEffect);
	
    TwDeleteBar(g_pTweakBar);
    g_pTweakBar = nullptr;
	TwTerminate();

}


// Draw the edges of the bounding box [-0.5;0.5]?rotated with the cameras model tranformation.
// (Drawn as line primitives using a DirectXTK primitive batch)
void DrawBoundingBox(ID3D11DeviceContext* pd3dImmediateContext)
{
    // Setup position/color effect
    g_pEffectPositionColor->SetWorld(g_camera.GetWorldMatrix());
    
    g_pEffectPositionColor->Apply(pd3dImmediateContext);
    pd3dImmediateContext->IASetInputLayout(g_pInputLayoutPositionColor);

    // Draw
    g_pPrimitiveBatchPositionColor->Begin();
    
    // Lines in x direction (red color)
    for (int i=0; i<4; i++)
    {
        g_pPrimitiveBatchPositionColor->DrawLine(
            VertexPositionColor(XMVectorSet(-0.5f, (float)(i%2)-0.5f, (float)(i/2)-0.5f, 1), Colors::Red),
            VertexPositionColor(XMVectorSet( 0.6f, (float)(i%2)-0.5f, (float)(i/2)-0.5f, 1), Colors::Red)
        );
    }

    // Lines in y direction
    for (int i=0; i<4; i++)
    {
        g_pPrimitiveBatchPositionColor->DrawLine(
            VertexPositionColor(XMVectorSet((float)(i%2)-0.5f, -0.5f, (float)(i/2)-0.5f, 1), Colors::Green),
            VertexPositionColor(XMVectorSet((float)(i%2)-0.5f,  0.6f, (float)(i/2)-0.5f, 1), Colors::Green)
        );
    }

    // Lines in z direction
    for (int i=0; i<4; i++)
    {
        g_pPrimitiveBatchPositionColor->DrawLine(
            VertexPositionColor(XMVectorSet((float)(i%2)-0.5f, (float)(i/2)-0.5f, -0.5f, 1), Colors::Blue),
            VertexPositionColor(XMVectorSet((float)(i%2)-0.5f, (float)(i/2)-0.5f,  0.6f, 1), Colors::Blue)
        );
    }

    g_pPrimitiveBatchPositionColor->End();
}

// Draw a large, square plane at y=-1 with a checkerboard pattern
// (Drawn as multiple quads, i.e. triangle strips, using a DirectXTK primitive batch)
void DrawFloor(ID3D11DeviceContext* pd3dImmediateContext)
{
    // Setup position/normal/color effect
    g_pEffectPositionNormalColor->SetWorld(XMMatrixIdentity());
    g_pEffectPositionNormalColor->SetEmissiveColor(Colors::Black);
    g_pEffectPositionNormalColor->SetDiffuseColor(0.8f * Colors::White);
    g_pEffectPositionNormalColor->SetSpecularColor(0.4f * Colors::White);
    g_pEffectPositionNormalColor->SetSpecularPower(1000);
    
    g_pEffectPositionNormalColor->Apply(pd3dImmediateContext);
    pd3dImmediateContext->IASetInputLayout(g_pInputLayoutPositionNormalColor);

    // Draw 4*n*n quads spanning x = [-n;n], y = -1, z = [-n;n]
    const float n = 4;
    XMVECTOR normal      = XMVectorSet(0, 1,0,0);
    XMVECTOR planecenter = XMVectorSet(0,-1,0,0);

    g_pPrimitiveBatchPositionNormalColor->Begin();
    for (float z = -n; z < n; z++)
    {
        for (float x = -n; x < n; x++)
        {
            // Quad vertex positions
            XMVECTOR pos[] = { XMVectorSet(x  , -1, z+1, 0),
                               XMVectorSet(x+1, -1, z+1, 0),
                               XMVectorSet(x+1, -1, z  , 0),
                               XMVectorSet(x  , -1, z  , 0) };

            // Color checkerboard pattern (white & gray)
            XMVECTOR color = ((int(z + x) % 2) == 0) ? XMVectorSet(1,1,1,1) : XMVectorSet(0.6f,0.6f,0.6f,1);

            // Color attenuation based on distance to plane center
            float attenuation[] = {
                1.0f - XMVectorGetX(XMVector3Length(pos[0] - planecenter)) / n,
                1.0f - XMVectorGetX(XMVector3Length(pos[1] - planecenter)) / n,
                1.0f - XMVectorGetX(XMVector3Length(pos[2] - planecenter)) / n,
                1.0f - XMVectorGetX(XMVector3Length(pos[3] - planecenter)) / n };

            g_pPrimitiveBatchPositionNormalColor->DrawQuad(
                VertexPositionNormalColor(pos[0], normal, attenuation[0] * color),
                VertexPositionNormalColor(pos[1], normal, attenuation[1] * color),
                VertexPositionNormalColor(pos[2], normal, attenuation[2] * color),
                VertexPositionNormalColor(pos[3], normal, attenuation[3] * color)
            );
        }
    }
    g_pPrimitiveBatchPositionNormalColor->End();    
}

void updateScreenSize(int width, int height)
{
	 // Update camera parameters
	g_camera.SetWindow(width, height);
	g_camera.SetProjParams(XM_PI / 4.0f, float(width) / float(height), 0.1f, 100.0f);

    // Inform AntTweakBar about back buffer resolution change
  	TwWindowSize(width,height);
}

void update(float fElapsedTime)
{
	    // Move camera
    g_camera.FrameMove(fElapsedTime);

    // Update effects with new view + proj transformations
    g_pEffectPositionColor->SetView       (g_camera.GetViewMatrix());
    g_pEffectPositionColor->SetProjection (g_camera.GetProjMatrix());

    g_pEffectPositionNormal->SetView      (g_camera.GetViewMatrix());
    g_pEffectPositionNormal->SetProjection(g_camera.GetProjMatrix());

	g_pEffectPositionNormalColor->SetView(g_camera.GetViewMatrix());
	g_pEffectPositionNormalColor->SetProjection(g_camera.GetProjMatrix());
}

void setUpLighting(Vec3 EmissiveColor, Vec3 SpecularColor, float SpecularPower, Vec3 DiffuseColor)
{
	XMVECTOR c1 = EmissiveColor.toDirectXVector();
	XMVECTOR c2 = SpecularColor.toDirectXVector();
	XMVECTOR c3 = DiffuseColor.toDirectXVector();
	g_pEffectPositionNormal->SetEmissiveColor(c1);
	g_pEffectPositionNormal->SetSpecularColor(c2);
	g_pEffectPositionNormal->SetSpecularPower(SpecularPower);
    g_pEffectPositionNormal->SetDiffuseColor(c3);  
}
void drawSphere(Vec3 pos, Vec3 scale)
{	
	drawSphere(pos.toDirectXVector(), scale.toDirectXVector());
}
void drawSphere(const XMVECTOR pos, const XMVECTOR scale)
{	
	// Setup position/normal effect (per object variables)
	XMMATRIX s    = XMMatrixScaling(XMVectorGetX(scale), XMVectorGetY(scale),XMVectorGetZ(scale));
    XMMATRIX t    = XMMatrixTranslation(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos));
    g_pEffectPositionNormal->SetWorld(s * t * g_camera.GetWorldMatrix());
    // Draw
    // NOTE: The following generates one draw call per object, so performance will be bad for n>>1000 or so
    g_pSphere->Draw( g_pEffectPositionNormal,  g_pInputLayoutPositionNormal);
}

void drawTeapot(Vec3 pos,Vec3 rot,Vec3 scale)
{	
	drawTeapot(pos.toDirectXVector(),rot.toDirectXVector(),scale.toDirectXVector());
}
void drawTeapot(const XMVECTOR pos,const XMVECTOR rot,const XMVECTOR scale)
{	
	// Setup position/normal effect (per object variables)
	XMMATRIX s    = XMMatrixScaling(XMVectorGetX(scale), XMVectorGetY(scale),XMVectorGetZ(scale));
    XMMATRIX t    = XMMatrixTranslation(XMVectorGetX(pos), XMVectorGetY(pos), XMVectorGetZ(pos));
	XMMATRIX r = XMMatrixRotationRollPitchYaw(XMVectorGetX(rot), XMVectorGetX(rot), XMVectorGetX(rot));
    g_pEffectPositionNormal->SetWorld(r * s * t * g_camera.GetWorldMatrix());
    // Draw
    // NOTE: The following generates one draw call per object, so performance will be bad for n>>1000 or so
	g_pTeapot->Draw( g_pEffectPositionNormal,  g_pInputLayoutPositionNormal);
}

void drawRigidBody(const XMMATRIX& m_objToWorld)
{
	g_pEffectPositionNormal->SetWorld(m_objToWorld);

	g_pEffectPositionNormal->Apply(g_pd3dImmediateContext);
	g_pd3dImmediateContext->IASetInputLayout(g_pInputLayoutPositionNormal);

	// front
	g_pPrimitiveBatchPositionNormal->Begin();
	g_pPrimitiveBatchPositionNormal->DrawQuad(
		VertexPositionNormal(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0,0,-1)),
		VertexPositionNormal(XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0,0,-1)),
		VertexPositionNormal(XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0,0,-1)),
		VertexPositionNormal(XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0,0,-1))
	);
	// back
	g_pPrimitiveBatchPositionNormal->DrawQuad(
		VertexPositionNormal(XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0,0,1)),
		VertexPositionNormal(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0,0,1)),
		VertexPositionNormal(XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0,0,1)),
		VertexPositionNormal(XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0,0,1))
	);
	// left
	g_pPrimitiveBatchPositionNormal->DrawQuad(
		VertexPositionNormal(XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(-1,0,0)),
		VertexPositionNormal(XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(-1,0,0)),
		VertexPositionNormal(XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(-1,0,0)),
		VertexPositionNormal(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(-1,0,0))
	);
	// right
	g_pPrimitiveBatchPositionNormal->DrawQuad(
		VertexPositionNormal(XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1,0,0)),
		VertexPositionNormal(XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1,0,0)),
		VertexPositionNormal(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1,0,0)),
		VertexPositionNormal(XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1,0,0))
	);
	// top
	g_pPrimitiveBatchPositionNormal->DrawQuad(
		VertexPositionNormal(XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0,1,0)),
		VertexPositionNormal(XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0,1,0)),
		VertexPositionNormal(XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0,1,0)),
		VertexPositionNormal(XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0,1,0))
	);
	// bottom
	g_pPrimitiveBatchPositionNormal->DrawQuad(
		VertexPositionNormal(XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0,-1,0)),
		VertexPositionNormal(XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0,-1,0)),
		VertexPositionNormal(XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0,-1,0)),
		VertexPositionNormal(XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0,-1,0))
	);
	g_pPrimitiveBatchPositionNormal->End();

}

void drawRigidBody(Mat4 m_objToWorld)
{
	drawRigidBody(m_objToWorld.toDirectXMatrix());
}

void DrawTriangleUsingShaders()
{
	XMMATRIX world = g_camera.GetWorldMatrix();
	XMMATRIX view  = g_camera.GetViewMatrix();
	XMMATRIX proj  = g_camera.GetProjMatrix();
	XMFLOAT4X4 mViewProj;
	XMStoreFloat4x4(&mViewProj, world * view * proj);
	g_pEffect->GetVariableByName("g_worldViewProj")->AsMatrix()->SetMatrix((float*)mViewProj.m);
	g_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, g_pd3dImmediateContext);
    
	g_pd3dImmediateContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	g_pd3dImmediateContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);
	g_pd3dImmediateContext->IASetInputLayout(nullptr);
	g_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pd3dImmediateContext->Draw(3, 0);
}

void beginLine()
{
	g_pEffectPositionColor->SetWorld(g_camera.GetWorldMatrix());
    g_pEffectPositionColor->Apply(g_pd3dImmediateContext);
	g_pd3dImmediateContext->IASetInputLayout(g_pInputLayoutPositionColor);
	// Draw
    g_pPrimitiveBatchPositionColor->Begin();
}
void drawLine(Vec3 pos1,Vec3 color1, Vec3 pos2,Vec3 color2)
{
	drawLine(pos1.toDirectXVector(),color1.toDirectXVector(),pos2.toDirectXVector(),color2.toDirectXVector());
}
void drawLine(const XMVECTOR pos1,const XMVECTOR &color1, const XMVECTOR pos2,const XMVECTOR &color2)
{
        g_pPrimitiveBatchPositionColor->DrawLine(
			VertexPositionColor(pos1, color1),
            VertexPositionColor(pos2, color2)
        );
}
void endLine()
{
	g_pPrimitiveBatchPositionColor->End();
}
};
#endif