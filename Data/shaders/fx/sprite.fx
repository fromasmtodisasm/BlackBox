#include "hlsl_common.fx"

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

float AnimSpeed <
	//register  = REG_PM_PARAM_0.y;
	string UIName = "Perturbation anim speed";

	string UIWidget = "slider";
	float  UIMin	= -10.0;
	float  UIMax	= 10.0;
	float  UIStep	= 0.005;
	>				= 0.2;

float PerturbationScale <
	//register  = REG_PM_PARAM_0.z;
	string UIName = "Perturbation tilling";

	string UIWidget = "slider";
	float  UIMin	= 0.0;
	float  UIMax	= 32.0;
	float  UIStep	= 0.005;
	>				= 0.5;

float3 main(float4 f1, int i2);

float3 main2(float4 f1, int i2) {}



FatalError

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

