//#include "FXConstantDefs.cfi"
//#include "AuxGeom.cfx"
//#include "NoDraw.cfx"
//#include "Common.cfx"

#define PERFRAME_SLOT b0
#define PERVIEW_SLOT b1
#define PERDRAW_SLOT b2

float global_float <string desc="Test global variable";> = 0.5;

float global_float2 <string desc="Test global variable";> = float(0);
float2 global_float3 <string desc="Test global variable";> = float2(0,0);

//struct struct_definition
//{
//    float4x4 Projection;
//    float data[3];
//    float data2[3][4];
//};
//
//ConstantBuffer<struct_definition> cbTest;

cbuffer PerFrameCB : register(PERFRAME_SLOT)
{
    float4 SunDirection;
    float4 SunColor;
    float4 AmbientStrength;
    float4 LightIntensity;
}
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
    float4x4 World;
    float4x4 MVP;
    bool ApplyGrayScale;
};


[[fn]]
float4x4 GetOrthoProjMat()
{
    return perViewCB.OrthoProjection;
}

[[fn]]
float4x4 GetProjMat()
{
    return perViewCB.Projection;
}

[[fn]]
float4x4 GetViewProjMat()
{
    return perViewCB.ViewProjection;
}

[[fn]]
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

[[fn]]
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

