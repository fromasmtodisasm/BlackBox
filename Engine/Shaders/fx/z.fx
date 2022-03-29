#include "common.fx"

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TC : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
};
 
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT IN)
{
    VS_OUTPUT output;
    output.Pos = Transform(IN.Pos);
    return output;
}
 
//--------------------------------------------------------------------------------------
// Pixel Shaders
//--------------------------------------------------------------------------------------
void PS(VS_OUTPUT input) {}

technique Main
{
    pass p0
    {
        VertexShader = VS();
        PixelShader = PS();
    }
}
