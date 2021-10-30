#include "hlsl_common.fx"
// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

Texture2D text : register(t0);
SamplerState textSampler : register(s0);

struct VsOutput
{
    float4 Position : SV_Position;
    float2 TexCoords : TEXCOORD;
    float4 Color : COLOR;
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
    VsOutput Output;
    Output.Position = float4(IN.Pos.x,IN.Pos.y, 0.0, 1.0);
    Output.TexCoords = IN.TC;
    Output.Color = IN.Color;
    return Output;
}  

[[fn]]
float4 Font(VsOutput IN) : SV_Target0
{    
    // {{{{{{{{{{{{{{{{{
    return IN.Color * float4(1.0, 1.0, 1.0, text.Sample(textSampler, IN.TexCoords).r);
}

[[fn]]
float4 TexturedQuad(VsOutput IN) : SV_Target0
{    
    return text.Sample(textSampler, IN.TexCoords);
}

Technique Font
<
    string description = "This is technique spesialized for font rendering";
>
{
    Pass p0<string script = "Decal";>
    {
        VertexShader = VSMain();
        PixelShader = Font();
    }
}

Technique TexturedQuad<string description = "This is technique spesialized for textured quad rendering";>
{
    Pass p0
    {
        VertexShader = VSMain();
        PixelShader = TexturedQuad();
    }
}

