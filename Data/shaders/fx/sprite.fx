#include "hlsl_common.fx"
// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

//#include "common.cfi"

// Uniform data
cbuffer CBAuxGeom : register(0)
{
	struct
	{
		float4x4 matViewProj;
		float2	 invScreenDim;
	} cbAuxGeom;
};

cbuffer CBAuxGeomObject : register(0)
{
	struct
	{
		float4x4 matViewProj;
		float4	 auxGeomObjColor;
		float2	 auxGeomObjShading;
		float3	 globalLightLocal;
	} cbAuxGeomObject;
};

// Structure of vertex shader input for geometries
struct SAuxGeomApp2VS
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD0; // unused but needs to be declared under DX10 to be able to share signatures
	float4 col : COLOR0;
};

// Structure of vertex shader input for objects
struct SAuxGeomObjApp2VS
{
	float4 pos : POSITION;
	float3 normal : TEXCOORD0;
};

// Structure of vertex shader output = pixel shader input
struct SAuxVS2PS
{
	float4 pos : SV_Position;
	float4 col : COLOR0;
};

struct SAuxVS2PSWithColor
{
	float4 pos : SV_Position;
	float2 tex : TEXCOORD0;
	float4 col : COLOR0;
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

float3 main(float4 f1, int i2);

/*
float3 main2(float4 f1, int i2) {

}



FatalError
*/

Shader
	{
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

    VsOutput VSMain(VsInput IN)
    {
        VsOutput Output;
        Output.Position = float4(IN.Pos.x,IN.Pos.y, 0.0, 1.0);
        Output.TexCoords = IN.TC;
        Output.Color = IN.Color;
        return Output;
    }  

    float4 Font(VsOutput IN) : SV_Target0
    {    
        return IN.Color * float4(1.0, 1.0, 1.0, text.Sample(textSampler, IN.TexCoords).r);
    }

    float4 TexturedQuad(VsOutput IN) : SV_Target0
    {    
        return text.Sample(textSampler, IN.TexCoords);
    }

}


Technique Font
<
    string description = "This is technique spesialized for font rendering";
>
{
    Pass p0<string script = "Decal";>
    {
        VertexShader = VSMain
        PixelShader = Font
    }
}

Technique TexturedQuad<string description = "This is technique spesialized for textured quad rendering";>
{
    Pass p0
    {
        VertexShader = VSMain
        PixelShader = TexturedQuad
    }
}

