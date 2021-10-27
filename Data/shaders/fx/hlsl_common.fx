//   sdf
[[fn]]
float4x4 TestFunc()
{
    // lkjsdf 
    // lkjsdf }}}}}}}}}}}}}}
    return PerViewCB.ViewProjection;
}
//#define PERFRAME_SLOT b0
//#define PERVIEW_SLOT b1
//#define PERINSTANCE_SLOT c2

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

[[fn]]
float4x4 GetOrthoProjMat()
{
    
    return PerViewCB.OrthoProjection;
}

[[fn]]
float4x4 GetProjMat()
{
    return PerViewCB.Projection;
}

[[fn]]
float4x4 GetViewProjMat()
{
    return PerViewCB.ViewProjection;
}


