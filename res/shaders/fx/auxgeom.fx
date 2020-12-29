Language HLSL
//#include "common.fx"

HLSLShader
{

	cbuffer perFrameCB : register(b0)
	{
		float deltaTime;
	}
	cbuffer perViewCB : register(b2)
	{
		struct PerViewCB
		{
			float4x4 projection;
			float4x4 ortho_projection;
			float4x4 view;
			float4x4 view_proj;
		}perViewCB;
	}

	float4x4 GetOrthoProjMat()
	{
		return perViewCB.ortho_projection;
	}

	float4x4 GetProjMat()
	{
		return perViewCB.projection;
	}

	float4x4 GetViewProjMat()
	{
		return perViewCB.view_proj;
	}
}
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
		OUT.pos = mul(GetProjMat(), float4(IN.Pos, 1.0));
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

technique Test
{
  pass p0
  {
    VertexShader = vert
    PixelShader = frag
  }
}