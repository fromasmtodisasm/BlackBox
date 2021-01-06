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
		[[vk::location(0)]]	float3 Pos : POSITION;
		[[vk::location(5)]] float4 Color : COLOR0;
		[[vk::location(2)]] float2 TC : TEXCOORD0;
	};

    VsOutput VSMain(VsInput IN)
    {
        VsOutput Output;
        Output.Position = float4(IN.Pos, 1.0);
        Output.TexCoords = IN.TC;
        Output.Color = IN.Color;
        return Output;
    }  
    Texture2D text : register(t0);
    SamplerState textSampler : register(s0);

    float4 PSMain(VsOutput IN) : SV_Target0
    {    
        return IN.Color * float4(1.0, 1.0, 1.0, text.Sample(textSampler, IN.TexCoords).r);
    }
}

Technique main {
    Pass p0
    {
        /*
        InputLayout
        {
            vec3 aPos : POSITION
            vec4 aCol : COLOR
            vec2 aTex : TEXCOORD
        }
        */
        VertexShader = VSMain
        PixelShader = PSMain
    }
}