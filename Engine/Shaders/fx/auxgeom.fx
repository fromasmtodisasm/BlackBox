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


VsOutput VSMain(VsInput IN)
{
    VsOutput OUT;
    OUT.pos = mul(GetViewProjMat(), float4(IN.Pos, 1.0));
    OUT.color = float4(IN.Color);
    return OUT;
}
 

float4 PSMain(VsOutput IN) : SV_Target0
{
    return float4(IN.color);
}

BlendState BlendState1
{
    BlendEnable = true;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
};;

DepthStencilState DisableDepth
{
    DepthEnable = false;
    DepthWriteMask = 1;
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

// Default technique for auxiliary geometry rendering
technique AuxGeometry
{
    pass p0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;

        ZEnable = false;
        ZWriteEnable = false;

        BlendEnable = true;
        SrcBlend = SRC_ALPHA;
        DestBlend = INV_SRC_ALPHA;
    }
}
