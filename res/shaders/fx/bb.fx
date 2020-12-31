Language HLSL
#include "hlsl_common.fx"
HLSLShader 
{
    #define TM_UNREAL
}

HLSLShader
{
    // Unreal 3, Documentation: "Color Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    float3 unreal(float3 x) {
    return x / (x + 0.155) * 1.019;
    }

    float unreal(float x) {
    return x / (x + 0.155) * 1.019;
    }

    #pragma glslify: export(unreal)

}

HLSLShader
{
    struct Light
    {
        float3 Position;
        //float3
    };
    float diffuse(float3 lightPos, float3 fragPos, float3 N)
    {
        float3 lightDir = normalize(lightPos - fragPos);
        return max(dot(normalize(N), lightDir), 0.0);
    }

    float specular(float3 lightPos, float3 fragPos, float3 eyePos, float3 N)
    {
        float3 reflectedLightVector = normalize(reflect(-(lightPos - fragPos), normalize(N)));
        float3 eyeVector			  = normalize(eyePos - fragPos);
        float s					  = max(dot(eyeVector, reflectedLightVector), 0.0);
        return pow(s, 128);
    }

    float3 ambient()
    {
        return float3(0.05, 0.05, 0.05);
    }

    float3 wo_tonemap(float3 x)
    {
        return x;
    }

    #ifdef TM_UNREAL
    #define tonemap unreal
    #else
    #define tonemap wo_tonemap
    #endif

}

HLSLShader
{
	cbuffer BoundingBoxParams : register(b10)
    {
        float4x4 model;
        float3 lightPos;
        bool bTonemap;
        int dbgmode;
        float alpha;
        float4 color;
        float3 eye;
	};

    struct VS_OUT
    {
        float4 pos : SV_Position;
        float3 fragPos : POSITION;
        float3 N : NORMAL;
    };


}

HLSLShader vert
{
    struct VsInput
	{
		float3 pos : POSITION;
		float3 normal : NORMAL;
	};
    
    VS_OUT main(VsInput IN)
    {
        VS_OUT OUT;
        OUT.pos = mul(mul(GetViewProjMat(), model), float4(IN.pos, 1.0));
        OUT.fragPos = (float3)(model * float4(IN.pos, 1.0));
        OUT.N = mul(((float3x3)model), IN.normal);
        return OUT;
    }
}

HLSLShader frag
{
    #define dbgDIFFUSE 1
    #define dbgSPECULAR 2
    #define dbgNORMAL 3

    struct OutColor
    {
        float3 ambient;
        float3 diffuse;
        float3 specular;
        float3 normal;
    };
    
    float4 main(VS_OUT IN) : SV_Target0
    { 
        OutColor outColor;
        outColor.ambient = float3(0);
        outColor.diffuse = float3(0);
        outColor.specular = float3(0);

        float3 _color = (float3)color;


        outColor.ambient = ambient()*_color;
        outColor.diffuse = (float3)diffuse(lightPos, IN.fragPos, IN.N);//*_color;
        outColor.specular = 0.8*_color*specular(lightPos, IN.fragPos, eye, IN.N);

        float3 result = outColor.ambient + outColor.diffuse + outColor.specular;

        switch (dbgmode)
        {
        case 0:

            break;
        case dbgDIFFUSE:
            result = outColor.diffuse + ambient();	
            break;
        case dbgNORMAL:
            result = 0.5*(normalize(IN.N) + 1);	
            break;
        case dbgSPECULAR:
            result = outColor.specular;	
            break;
        }

        if (bTonemap)
        {
            return float4(tonemap(result), 1);
        }
        else
        {
            return float4(result, 1);
        }
    }

}


technique main
{
    pass p0
    {
        /* 
        InputLayout
        {
            float3 aPos : POSITION
            float3 aN : NORMAL
        }
        */

        VertexShader = vert
        PixelShader = frag
    }
}
