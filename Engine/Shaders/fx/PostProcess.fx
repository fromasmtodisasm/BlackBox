#include "common.fx"

Texture2D RenderedScene : register(t0);
SamplerState SceneSampler : register(s0);

struct VsOutput
{
    float4 Position : SV_Position;
    float2 TexCoords : TEXCOORD;
    float4 Color : COLOR;
};

struct VsInput
{
    float3 Pos : POSITION;
    float2 TC : TEXCOORD0;
};

[[fn]]
VsOutput VSMain(VsInput IN)
{
    VsOutput Output;
    Output.Position = float4(IN.Pos.x,IN.Pos.y, 0.0, 1.0);
    Output.TexCoords = IN.TC;
    return Output;
}  

[[fn]]
// same function declaration style as vertex shaders
// pixel shaders return the colour value of the pixel (hence the float4)
float4 GrayScalePS(float2 texCoord : TEXCOORD0) : SV_Target0
{
    // sample the texture at the specified texture coordinates
    return GrayScale(RenderedScene.Sample(SceneSampler, texCoord));
}

Technique GrayScale
<
    string description = "";
>
{
    Pass p0
    {
        VertexShader = VSMain();
        PixelShader = GrayScalePS();
    }
}

