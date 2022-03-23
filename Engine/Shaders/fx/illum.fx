#include "hlsl_common.fx"

#include "pbr.fx"

//static float gamma = 2.2;
#define gamma 2.2f

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
    //output.Normal = IN.Normal;
	output.Normal    = GetNormal(IN.Normal); // calculate view-space normal
    output.TC = IN.TC;

    return output;
}

[[fn]]
float4 PS(VS_OUTPUT input)
	: SV_Target
{
	static vec3  albedo;
	static float metallic;
	static float roughness;
	static float ao;

    //FIXME: need recognize storage class in shader parser to place this declaration on top level
    float3 diffuseColor = float3(1, 1, 1);

    float4 textureColor;
	float3 lightDir;
	float  lightIntensity;
	float3 color;

	vec3   N       = normalize(input.Normal);
	vec3   V       = normalize(GetEye() - input.Pos);

    for (int i = 0; i < NumLights; i++)
	{
        vec3   N       = normalize(input.Normal);
        vec3   V       = normalize(GetEye() - input.Pos);

        //g_Lights[i]
	}

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor   = float4(0.5, 0.5, 0.5, 1);
	//textureColor = g_FontAtlas.Sample(g_LinearSampler, input.TC);

	// Invert the light direction for calculations.
    lightDir = SunDirection.xyz;

	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(N, lightDir));

	// Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
	color          = saturate(SunColor.rgb * diffuseColor * lightIntensity);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    //color = (color + AmbientStrength) * textureColor;
	color          = color + fresnelSchlick(dot(N, lightDir), float3(0.01, 0.05, 0.01));
	float tmp      = 1.0 / gamma;
	color          = pow(color, vec3(tmp, tmp, tmp));

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
