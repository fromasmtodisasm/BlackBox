GLSLShader 
{
    #version 330 core
    #define TM_UNREAL
}

GLSLShader
{
    // Unreal 3, Documentation: "Color Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    vec3 unreal(vec3 x) {
    return x / (x + 0.155) * 1.019;
    }

    float unreal(float x) {
    return x / (x + 0.155) * 1.019;
    }

    #pragma glslify: export(unreal)

}


GLSLShader
{
    struct Light
    {
        vec3 Position;
        //vec3
    };
    float diffuse(vec3 lightPos, vec3 fragPos, vec3 N)
    {
        vec3 lightDir = normalize(lightPos - fragPos);
        return max(dot(normalize(N), lightDir), 0.0);
    }

    float specular(vec3 lightPos, vec3 fragPos, vec3 eyePos, vec3 N)
    {
        vec3 reflectedLightVector = normalize(reflect(-(lightPos - fragPos), normalize(N)));
        vec3 eyeVector			  = normalize(eyePos - fragPos);
        float s					  = max(dot(eyeVector, reflectedLightVector), 0.0);
        return pow(s, 128);
    }

    vec3 ambient()
    {
        return vec3(0.05, 0.05, 0.05);
    }

    vec3 wo_tonemap(vec3 x)
    {
        return x;
    }

    #ifdef TM_UNREAL
    #define tonemap unreal
    #else
    #define tonemap wo_tonemap
    #endif

}

GLSLShader vert
{
    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    out VS_OUT
    {
        vec3 fragPos;
        vec3 N;
    }vs_out;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.N = mat3(transpose(inverse(model))) * aN;
    }

}

GLSLShader frag
{
    out vec4 FragColor;
    #if 0
    #include "lighting.h"
    #endif

    uniform float alpha;
    uniform vec4 color;
    uniform vec3 lightPos;
    uniform vec3 eye;
    uniform int fid;
    uniform mat4 projection;

    uniform bool bTonemap;

    #define dbgDIFFUSE 1
    #define dbgSPECULAR 2
    #define dbgNORMAL 3

    uniform int dbgmode = 0;

    in VS_OUT
    {
        vec3 fragPos;
        vec3 N;
    }vs_out;

    struct OutColor
    {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 normal;
    };
    
    void main()
    { 
        struct OutColor outColor;
        outColor.ambient = vec3(0);
        outColor.diffuse = vec3(0);
        outColor.specular = vec3(0);

        vec4 p = gl_FragCoord;
        vec3 _color = vec3(color);


        outColor.ambient = ambient()*_color;
        outColor.diffuse = vec3(diffuse(lightPos, vs_out.fragPos, vs_out.N));//*_color;
        outColor.specular = 0.8*_color*specular(lightPos, vs_out.fragPos, eye, vs_out.N);

        vec3 result = outColor.ambient + outColor.diffuse + outColor.specular;

        switch (dbgmode)
        {
        case 0:

            break;
        case dbgDIFFUSE:
            result = outColor.diffuse + ambient();	
            break;
        case dbgNORMAL:
            result = 0.5*(normalize(vs_out.N) + 1);	
            break;
        case dbgSPECULAR:
            result = outColor.specular;	
            break;
        }

        if (bTonemap)
        {
            FragColor = vec4(tonemap(result), 1);
        }
        else
        {
        FragColor = vec4(result, 1);
        }
    }

}


Technique main
{
    Pass p0
    {
        InputLayout
        {
            vec3 aPos : POSITION
            vec3 aN : NORMAL
        }

        VertexShader = vert
        PixelShader = frag
    }
}
