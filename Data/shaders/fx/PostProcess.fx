#include "hlsl_common.fx"

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
    float4 tex = RenderedScene.Sample(SceneSampler, texCoord);
     
  // greyscale the pixel colour values
  // - perform a dot product between the pixel colour and the specified vector
  // - 0.222, 0.707, 0.071 is found throughout image processing for gray scale effects.
    float4 grey = dot(float3(0.222, 0.707, 0.071), tex);
  
  // return the pixel colour in the form of a float4.          
    return grey;
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

