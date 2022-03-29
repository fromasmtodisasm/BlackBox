#include "common.fx"

Shader
{
	struct VsOutput
	{
		float4 pos : SV_POSITION;
		float2 tc: TEXCOORD0;
	};

	cbuffer ScreenBuffer : register(b11)
    {
        float4x4 model;
        float4x4 uv_projection;
        float4 color;
        float alpha;
	};

    struct VsInput
	{
		float3 Pos : POSITION;
		float2 TC : TEXCOORD0;
	};


    VsOutput VSMain(VsInput IN)
    {
		VsOutput OUT;
        OUT.pos = mul(mul(GetOrthoProjMat(), model), float4(IN.Pos, 1.0));
        OUT.tc = mul(uv_projection, float4(IN.TC, 1.0, 1.0)).xy;
    	return OUT;
    }

    Texture2D screenTexture : register(t0);
    SamplerState linearSampler : register(s0);

    float4 PSMain(VsOutput IN) : SV_Target0
    { 
        float2 uv = IN.tc; 
        //return screenTexture.Sample(linearSampler, IN.tc)*float4(color.rgb, alpha);
        return float4(1,1,1,1);
    }
}

Technique main
{
    pass p0
    {
        VertexShader = VSMain
        PixelShader = PSMain
    }
}
