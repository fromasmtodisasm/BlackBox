Language HLSL
#include "hlsl_common.fx"

HLSLShader
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

}

HLSLShader vert
{
	
    struct VsInput
	{
		[[vk::location(0)]]	float3 Pos : POSITION;
		[[vk::location(2)]] float2 TC : TEXCOORD0;
	};


    VsOutput main(VsInput IN)
    {
		VsOutput OUT;
        OUT.pos = mul(mul(GetViewProjMat(), model), float4(IN.Pos, 1.0));
        OUT.tc = mul(uv_projection, float4(IN.TC, 1.0, 1.0)).xy;
    	return OUT;
    }
}

HLSLShader frag
{
    //uniform sampler2D screenTexture;
    Texture2D screenTexture : register(t1);
    SamplerState linearSampler : register(s1);

    //sampler2D screenTexture;
    float4 main(VsOutput IN) : SV_Target0
    { 
        float2 uv = IN.tc; 
        return screenTexture.Sample(linearSampler, IN.tc);//*color;
        //return tex2D(screenTexture, float2(0.5,0.5))*color;
        return float4(color);
    }
}

technique main
{
    pass p0
    {
        /*
        InputLayout
        {
            vec3 aPos : POSITION
            vec2 aTexCoords : TEXCOORD
        }
        */

        VertexShader = vert
        PixelShader = frag
    }
}
