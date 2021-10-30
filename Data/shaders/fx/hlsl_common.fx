//#include "FXConstantDefs.cfi"
//#include "AuxGeom.cfx"
//#include "NoDraw.cfx"
//#include "Common.cfx"

#define PERFRAME_SLOT b0
#define PERVIEW_SLOT b1
#define PERINSTANCE_SLOT c2

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
    float deltaTime;
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


