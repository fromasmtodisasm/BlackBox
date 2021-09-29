Language HLSL

#include "hlsl_common.fx"

HLSLShader
{
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
    Texture2D text : register(t0);
    SamplerState textSampler : register(s0);

    float4 PSMain(VsOutput IN) : SV_Target0
    {    
        return IN.Color * float4(1.0, 1.0, 1.0, text.Sample(textSampler, IN.TexCoords).r);
        //return float4(1.0, 1.0, 1.0, text.Sample(textSampler, IN.TexCoords).r);
        //return float4(1.0, 1.0, 1.0, 1.0);
    }
}

Technique main {
    Pass p0
    {
        VertexShader = VSMain
        PixelShader = PSMain
    }
}