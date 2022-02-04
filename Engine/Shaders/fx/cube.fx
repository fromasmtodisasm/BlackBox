//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

Shader
{
	//--------------------------------------------------------------------------------------
	// Constant Buffer Variables
	//--------------------------------------------------------------------------------------
	cbuffer cbChangesEveryFrame : register(b2)
	{
		float4x4 World;
		float4x4 View;
		float4x4 Projection;
	};

	//--------------------------------------------------------------------------------------
	struct VS_OUTPUT
	{
		float4 Pos : SV_POSITION;
		float4 Color : COLOR0;
	};

	//--------------------------------------------------------------------------------------
	// Vertex Shader
	//--------------------------------------------------------------------------------------
	VS_OUTPUT VS(float4 Pos
				 : POSITION, float4 Color
				 : COLOR)
	{
		VS_OUTPUT output = (VS_OUTPUT)0;
		output.Pos		 = mul(Pos, World);
		output.Pos		 = mul(output.Pos, View);
		output.Pos		 = mul(output.Pos, Projection);
		output.Color	 = Color;
		return output;
	}

	//--------------------------------------------------------------------------------------
	// Pixel Shader
	//--------------------------------------------------------------------------------------
	float4 PS(VS_OUTPUT input) : SV_Target
	{
		return input.Color;
	}
}

//--------------------------------------------------------------------------------------
technique main
{
    pass p0
    {
        VertexShader = VS
        PixelShader = PS
    }
}


