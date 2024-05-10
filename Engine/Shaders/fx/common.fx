//#include "FXConstantDefs.cfi"
//#include "AuxGeom.cfx"
//#include "NoDraw.cfx"
//#include "Common.cfx"

//typedef float2 vec2;
//typedef float3 vec3;

#define PERFRAME_SLOT b0
#define LIGHTS_SLOT   b1
#define PERVIEW_SLOT  b2
#define PERDRAW_SLOT  b3
#define MATERIAL_SLOT  b4

float global_float <string desc="Test global variable";> = 0.5;

float global_float2 <string desc="Test global variable";> = float(0);
float2 global_float3 <string desc="Test global variable";> = float2(0,0);
float2 global_float4 <float SomeValue=123456;> = float2(0,0);

///////////////////////////////////
static float PI = 3.14159265f;
///////////////////////////////////

//struct struct_definition
//{
//    float4x4 Projection;
//    float data[3];
//    float data2[3][4];
//};
//
//ConstantBuffer<struct_definition> cbTest;

struct Light
{
	float3 Pos;
	float3 Color;
};


cbuffer PerFrameCB : register(PERFRAME_SLOT)
{
    float4 SunDirection;
    float4 SunColor;
    float4 AmbientStrength;
    float4 LightIntensity;
	int    NumLights;
}

#define NUM_LIGHTS 4
cbuffer LightsCB : register(LIGHTS_SLOT)
{
	Light g_Lights[NUM_LIGHTS];
};

cbuffer PerViewCB : register(PERVIEW_SLOT)
{
    struct PERVIEWCB
    {
        float4x4 Projection;
        float4x4 OrthoProjection;
        float4x4 View;
        float4x4 ViewProjection;
        float3 Eye;
    }perViewCB;
}

cbuffer PerDrawCB : register(PERDRAW_SLOT)
{
    float4x4 Model;
    float4x4 World;
    float4x4 MVP;
    float4x4 MV;
    bool ApplyGrayScale;
};

cbuffer MaterialCB : register(MATERIAL_SLOT)
{
	float3 albedo;
	float  metallic;
	float  roughness;
	float  ao;
}





float3 GetEye()
{
    return perViewCB.Eye;
}


float4x4 GetOrthoProjMat()
{
    return perViewCB.OrthoProjection;
}


float4x4 GetViewMatrix()
{
    return perViewCB.View;
}


float4x4 GetProjMat()
{
    return perViewCB.Projection;
}


float4x4 GetViewProjMat()
{
    return perViewCB.ViewProjection;
}


float3 GetNormal(float3 normal)
{
	float4x4 ModelView = mul(GetViewMatrix(), Model);
	return normalize(mul((float3x3)Model, normal));
}


float4 Transform(in float3 Pos)
{
#if 0
    // NOTE: ok, its worked
    matrix mvp = mul(GetViewProjMat(), World);
    output.Pos = mul(mvp, float4(IN.Pos, 1));
#else
    // NOTE: with first matrix in mul no need transpose matrix on cpu side
    //output.Pos = mul(float4(IN.Pos, 1), MVP);
#endif
    return mul(MVP, float4(Pos, 1));
}


float4 WorldTransofrm(in float3 Pos)
{
    return mul(Model, float4(Pos, 1));
}


// same function declaration style as vertex shaders
// pixel shaders return the colour value of the pixel (hence the float4)
float4 GrayScale(float4 color) : SV_Target0
{
  // greyscale the pixel colour values
  // - perform a dot product between the pixel colour and the specified vector
  // - 0.222, 0.707, 0.071 is found throughout image processing for gray scale effects.
    float3 grey = dot(float3(0.222, 0.707, 0.071), color.rgb);
  
  // return the pixel colour in the form of a float4.          
    return float4(grey.rgb, color.a);
}


DepthStencilState DisableDepth
{
    DepthEnable = false;
    DepthWriteMask = 0;
};

BlendState AlphaBlend
{
    BlendEnable = true;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

RasterizerState NoCull
{
    CullMode = NONE;
    FillMode = SOLID;
    FrontCounterClockwise = false;
    DepthBias = 0;
    DepthBiasClamp = 0;
    SlopeScaledDepthBias = 0;
    DepthClipEnable = true;
    ScissorEnable=false;
    MultisampleEnable = false;
    AntialiasedLineEnable=true;
};

