#ifdef BB_PARSER
Shader {
#endif

#define PERFRAME_SLOT b0
#define PERVIEW_SLOT b1
#define PERINSTANCE_SLOT c2

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
    }PerViewCB;
}

float4x4 GetOrthoProjMat()
{
    return PerViewCB.OrthoProjection;
}

float4x4 GetProjMat()
{
    return PerViewCB.Projection;
}

float4x4 GetViewProjMat()
{
    return PerViewCB.ViewProjection;
}

#ifdef BB_PARSER
}
#endif
