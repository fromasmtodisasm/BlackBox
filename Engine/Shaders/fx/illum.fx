#include "common.fx"

#include "shadeLib.fx"

//static float gamma = 2.2;
#define gamma 2.2f

//#define WorldPos input.WorldPos

// Shader global descriptions
float Script : STANDARDSGLOBAL
<
    string Script =
        // Determines if the shader will be made visible to UI elements
        // For example, if set the shader will be available in the Material Editor
        "Public;"
        // If set when SupportsFullDeferredShading is not set, we go through the tiled forward pass
        // In the tiled forward case, we will execute the pass defined in this file - and not Common*Pass
        "SupportsDeferredShading;"
        // Determines that the shader supports fully deferred shading, so will not go through the forward pass
        "SupportsFullDeferredShading;";
>;

//FIXME: shader parser
//StructuredBuffer<float4x4> Transform;

//namespace test
//{
//#define ALBEDO static vec3 albedo;
//#define METALIC static float metallic;
//#define ROUGHNESS static float roughness;
//#define AO static float ao;
//
//ALBEDO    
//METALIC   
//ROUGHNESS 
//AO        
	//static vec3  albedo;
	//static float metallic;
	//static float roughness;
	//static float ao;
//}


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TC : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float3 WorldPos : WORLDPOS;
    float3 Normal : NORMAL;
    float2 TC : TEXCOORD0;
};

Texture2D g_FontAtlas : register(t0);
SamplerState g_LinearSampler : register(s0);

[[fn]]
VS_OUTPUT VS(
    VS_INPUT IN
)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = Transform(IN.Pos);
    output.WorldPos = WorldTransofrm(IN.Pos).xyz;
    //output.Normal = IN.Normal;
	output.Normal    = GetNormal(IN.Normal); // calculate view-space normal
    output.TC = IN.TC;

    return output;
}

[[fn]]
float4 PS(VS_OUTPUT input) : SV_Target
{
	//typedef float2 vec2;
	//typedef float3 vec3;
	//tmp
	float  ao        = 1.f;
	float3 albedo    = float3(0.5, 0, 0);
	float  metalic   = 0.99;
	float  roughness = 0.01;
	/////////////////
	float3 WorldPos  = input.WorldPos;

	float3 N  = normalize(input.Normal);
	float3 V  = normalize(GetEye() - WorldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	float3 F0 = float3(0.04, 0.04, 0.04);
	//float3 F0 = float3(0.3, 0.3, 0.3);
	F0        = lerp(F0, albedo, metallic);

	float3 Lo = float3(0, 0, 0);
	for (int i = 0; i < 4; ++i)
	{
		float3 L           = normalize(g_Lights[i].Pos - WorldPos);
		float3 H           = normalize(V + L);

		float  distance    = length(g_Lights[i].Pos - WorldPos);
		float  attenuation = 1.0 / (distance * distance);
		float3 radiance    = g_Lights[i].Color * attenuation;

		// Cook-Torrance BRDF
		float  NDF         = DistributionGGX(N, H, roughness);
		float  G           = GeometrySmith(N, V, L, roughness);
		float3 F           = fresnelSchlick(max(dot(H, V), 0.0), F0);

		float3 kS          = F;
		float3 kD          = float3(1, 1, 1) - kS;
		kD *= 1.0 - metallic;

		float3 numerator   = NDF * G * F;
		float  denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		float3 specular    = numerator / max(denominator, 0.001);

		float  NdotL       = max(dot(N, L), 0.0);
		// add to outgoing radiance Lo
		Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}

	float3 ambient = float3(0.03, 0.03, 0.03) * albedo * ao;
	float3 color   = ambient + Lo;

	color          = color / (color + float3(1.0, 1.0, 1.0));
	float invGamma = 1.0 / 2.2;
	color          = pow(color, float3(invGamma, invGamma, invGamma));

	return float4(color, 1);
}

//--------------------------------------------------------------------------------------
//technique10 Render
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_4_0, VS()));
//        SetGeometryShader(NULL);
//        SetPixelShader(CompileShader(ps_4_0, PS()));
//    }
//}
technique Render
{
    pass P0
    {
        VertexShader = VS;
        PixelShader = PS;
    }
}

technique GrayScaleT
{
    pass P0
    {
        VertexShader = VS();
        PixelShader = GrayScale();
    }
}
