#include "common.fx"

struct VsOutput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct VsInput
{
    float3 Pos : POSITION;
    float4 Color : COLOR0;
    float2 TC : TEXCOORD0;
};

[[fn]]
VsOutput VSMain(VsInput IN)
{
    VsOutput OUT;
    OUT.pos = mul(GetViewProjMat(), float4(IN.Pos, 1.0));
    OUT.color = float4(IN.Color);
    return OUT;
}
 
[[fn]]
float4 PSMain(VsOutput IN) : SV_Target0
{
    return float4(IN.color);
}

// Default technique for auxiliary geometry rendering
technique AuxGeometry
{
    pass p0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
    pass p1
    {
        VertexShader = vert;
        PixelShader = frag;
    }
}
