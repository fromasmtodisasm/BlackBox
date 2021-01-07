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
			float3 eye;
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
