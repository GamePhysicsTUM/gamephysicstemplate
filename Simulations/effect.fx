//--------------------------------------------------------------------------------------
// File: effect.fx
//
// A basic effect file including a pass for rendering a simple triangle.
//
// Note: All fx-files in the project are automatically compiled by Visual Studio
//       to fxo-files in the output directories "x64\Debug\" and "x64\Release\"
//       (see "HLSL Compiler" in the project properties).
//--------------------------------------------------------------------------------------

//Corresponds to D3D11_RASTERIZER_DESC
RasterizerState CullNone
{
	CullMode = None;
};

//Corresponds to D3D11_DEPTH_STENCIL_DESC
DepthStencilState DepthDefault
{
};

//Corresponds to D3D11_BLEND_DESC
BlendState BlendDisable
{
};



cbuffer cb
{
	float4x4 g_worldViewProj;
}



struct PSIn
{
	float4 pos   : SV_Position;
	float4 color : COLOR;
};


//A simple vertex shader that has harcoded vertex positions 
//in world space for the first three vertices (IDs 0, 1 and 2).
void vsTriangle(uint vertexID : SV_VertexID, out PSIn output)
{
	switch(vertexID)
	{
		case 0:
			output.pos = float4(-0.5, -0.5, 0.0, 1.0);
			output.color = float4(1.0, 0.0, 0.0, 1.0);
			break;
		case 1:
			output.pos = float4(0.0, 0.5, 0.0, 1.0);
			output.color = float4(0.0, 1.0, 0.0, 1.0);
			break;
		default:
			output.pos = float4(0.5, -0.5, 0.0, 1.0);
			output.color = float4(0.0, 0.0, 1.0, 1.0);
			break;
	}

	// transform vertex position into clip space
	output.pos = mul(output.pos, g_worldViewProj);
}

//A simple pixel shader that returns the input vertex color (interpolated output of vertex shader)
float4 psSimple(PSIn input) : SV_Target
{
	return input.color;
}


// Effect technique named "Simple" (technique = collection of passes)
technique11 Simple
{
	// Pass named "P0" for rendering a simple triangle 
	// (a pass corresponds to a rendering pipeline configuration, 
	//  i.e. a set of states and shaders)
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vsTriangle()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, psSimple()));
		SetRasterizerState(CullNone);
		SetDepthStencilState(DepthDefault, 0);
		SetBlendState(BlendDisable, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
	}
}
