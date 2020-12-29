Language HLSL
#include "hlsl_common.fx"

HLSLShader
{
	struct VsOutput
	{
		float4 pos : SV_POSITION;
		[[vk::location(0)]] float4 color: COLOR;
	};
}

HLSLShader vert
{
	struct VsInput
	{
		[[vk::location(0)]]	float3 Pos : POSITION;
		[[vk::location(5)]] float4 Color : COLOR0;
		[[vk::location(2)]] float2 TC : TEXCOORD0;
	};

	VsOutput main(VsInput IN)
	{
		VsOutput OUT;
		OUT.pos = mul(GetViewProjMat(), float4(IN.Pos, 1.0));
		OUT.color = float4(IN.Color);
		return OUT;
	}
 
}

HLSLShader frag
{
	float4 main(VsOutput IN) : SV_Target0
	{ 
		return float4(IN.color);
	}
}

// Default technique for auxiliary geometry rendering
technique AuxGeometry
{
  pass p0
  {
	/*
	InputLayout
	{
		vec3 aPos : POSITION
		vec4 aColor : COLOR
		vec2 aTC : TEXCOORD
	}
	*/
	
    VertexShader = vert
    PixelShader = frag
  }
  pass p1
  {
    VertexShader = vert
    PixelShader = frag
  }
}
