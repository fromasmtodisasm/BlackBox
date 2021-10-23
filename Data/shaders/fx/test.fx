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
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TC : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TC : TEXCOORD0;
};

Texture2D g_FontAtlas : register(t0);
SamplerState g_LinearSampler : register(s0);

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(
    VS_INPUT IN
)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(IN.Pos, World );
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    //output.Pos = mul( Pos, MVP );
    output.Normal = IN.Normal;
    output.TC = IN.TC;

    return output;
}

static float  ambientStrength = 0.3;
static float3 lightColor	  = float3(1, 1, 1);

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input)
	: SV_Target
{
	float3 diffuseColor = float3(1, 1, 1);

	float4 textureColor;
	float3 lightDir;
	float  lightIntensity;
	float4 color;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = g_FontAtlas.Sample(g_LinearSampler, input.TC);

	// Invert the light direction for calculations.
	lightDir = normalize(float3(2,3,4));

	// Calculate the amount of light on this pixel.
	lightIntensity = saturate(dot(input.Normal, lightDir));

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	color = float4(saturate(diffuseColor * lightIntensity),1);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	color = (color + 0.2) * textureColor;

	return color;
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
