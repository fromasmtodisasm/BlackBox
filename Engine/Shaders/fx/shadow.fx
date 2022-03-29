#include "common.fx"

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TC : TEXCOORD0;
};


struct SHADOW_PS_INPUT
{
    float4 Pos : SV_POSITION;
};
 
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
SHADOW_PS_INPUT ShadowMapVS(VS_INPUT IN)
{
    SHADOW_PS_INPUT output;
    output.Pos = mul( IN.Pos, mul( world, lightViewProj ) );
    return output;
}
 
//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------
void ShadowMapPS( SHADOW_PS_INPUT input ) {}

technique Main
{
    pass p0
    {
        VertexShader = ShadowMapVS();
        PixelShader = ShadowMapPS();
    }
}