#include "hlsl_common.fx"
// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

// Uniform data
cbuffer CBAuxGeom : register(PERINSTANCE_SLOT)
{
	struct
	{
		float4x4 matViewProj;
		float2	 invScreenDim;
	} cbAuxGeom;
};

cbuffer CBAuxGeomObject : register(PERINSTANCE_SLOT)
{
	struct
	{
		float4x4 matViewProj;
		float4	 auxGeomObjColor;
		float2	 auxGeomObjShading;
		float3	 globalLightLocal;
	} cbAuxGeomObject;
};


float RefrBumpScale <
	//register  = REG_PM_PARAM_0.x;
string UIHelp = "Set refraction bump scale \nMin value = 0, Max value = 2.0 \nCorrect name - RefrBumpScale";
string UIName = "Refraction Bump Scale";

string UIWidget = "slider";
float  UIMin	= 0.0;
float  UIMax	= 2.0;
float  UIStep	= 0.1;
>				= 0.1;

[[fn]]
float3 main(in float4 f1, out int i2);

[[fn]]
float3 main(in float4 f1, out int i2);

[[fn]]
float3 main2(float4 f1, out int i2) {
    return float3(0);
}

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
        VertexShader = VSMain;
        PixelShader = Font;
    }
}

Technique TexturedQuad<string description = "This is technique spesialized for textured quad rendering";>
{
    Pass p0
    {
        VertexShader = VSMain;
        PixelShader = TexturedQuad;
    }
}

