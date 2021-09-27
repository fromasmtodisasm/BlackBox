//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer cbChangesEveryFrame : register(b2)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 MVP;
};


//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float2 TC : TEXCOORD0;
};

Texture2D g_FontAtlas : register(t0);
SamplerState g_LinearSampler : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(
    float4 Pos : POSITION
    , float4 Color : COLOR
    , float2 TC: TEXCOORD0 
)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //output.Pos = mul( Pos, World );
    //output.Pos = mul( output.Pos, View );
    output.Pos = mul(Pos, View);
    output.Pos = mul(output.Pos, Projection);
    //output.Pos = mul( Pos, MVP );
    output.Color = Color;
    output.TC = TC;

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    //return input.Color;
    return g_FontAtlas.Sample(g_LinearSampler, input.TC);
}


//--------------------------------------------------------------------------------------
technique10 Render
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}

