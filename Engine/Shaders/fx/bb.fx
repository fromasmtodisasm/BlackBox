#include "common.fx"

#define TM_UNREAL
// Unreal 3, Documentation: "Color Grading"
// Adapted to be close to Tonemap_ACES, with similar range
// Gamma 2.2 correction is baked in, don't use with sRGB conversion!
[[fn]]
float3 unreal(float3 x) {
    return x / (x + 0.155) * 1.019;
}

[[fn]]
float unreal(float x) {
    return x / (x + 0.155) * 1.019;
}





[[fn]]
float diffuse(float3 lightPos, float3 fragPos, float3 N)
{
    float3 lightDir = normalize(lightPos - fragPos);
    return max(dot(normalize(N), lightDir), 0.0);
}

[[fn]]
float specular(float3 lightPos, float3 fragPos, float3 eyePos, float3 N)
{
    float3 reflectedLightVector = normalize(reflect(-(lightPos - fragPos), normalize(N)));
    float3 eyeVector			  = normalize(eyePos - fragPos);
    float s					  = max(dot(eyeVector, reflectedLightVector), 0.0);
    return pow(s, 128);
}

[[fn]]
float3 ambient()
{
    return float3(0.05, 0.05, 0.05);
}

[[fn]]
float3 wo_tonemap(float3 x)
{
    return x;
}

//#ifdef TM_UNREAL
//#define tonemap unreal
//#else
//#define tonemap wo_tonemap
//#endif

cbuffer BoundingBoxParams : register(b10)
{
    float4x4 model;
    float3 lightPos;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 fragPos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};


struct VsInput
{
    /*[[vk::location(0)]]*/ float3 pos : POSITION;
    /*[[vk::location(1)]]*/ float3 normal : NORMAL;
    /*[[vk::location(5)]]*/ float4 color : COLOR;
};

[[fn]] 
VS_OUT VSMain(VsInput IN)
{
	VS_OUT OUT;
	OUT.pos		= mul(mul(GetViewProjMat(), model), float4(IN.pos, 1.0));
	OUT.fragPos = (float3)(mul(model, float4(IN.pos, 1.0)));
	OUT.normal	= mul(((float3x3)model), IN.normal);
	OUT.color	= IN.color;
	return OUT;
}

struct OutColor
{
	float3 ambient;
	float3 diffuse;
	float3 specular;
	float3 normal;
};

[[fn]]
float4 PSMain(VS_OUT IN)
	: SV_Target0
{
	OutColor outColor = (OutColor)0;
	outColor.ambient  = float3(0, 0, 0);
	outColor.diffuse  = float3(0, 0, 0);
	outColor.specular = float3(0, 0, 0);
	float3 _color	  = IN.color.xyz;

	outColor.ambient  = ambient() * _color;
	outColor.diffuse  = (float3)diffuse(lightPos, IN.fragPos, IN.normal); //*_color;
	outColor.specular = 0.8 * _color * specular(lightPos, IN.fragPos, perViewCB.Eye, IN.normal);

	float3 result = outColor.ambient + outColor.diffuse + outColor.specular;

	//result = outColor.diffuse + ambient();

	//return float4(tonemap(result), 1);
	return float4(result, 1);
}



technique main
{
    pass p0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}
