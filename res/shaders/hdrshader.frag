#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom = true;
uniform float exposure;
uniform float bloom_exposure = 0.8;
uniform vec2 scale = vec2(1,1);

float float_to_sRGB(float val)
{ 
    if( val < 0.0031308f )
        val *= 12.92f;
    else
        val = 1.055f * pow(val,1.0f/2.4f) - 0.055f;
    return val;
}

//uniform sampler2D mainMap;
//uniform sampler2D blurMap;

vec3 tonemap_reinhard(vec3 color)
{
	return (vec3(1.0) - exp(-color * exposure))*0.6;
}

vec3 tonemap2(vec3 base, vec3 blur)
{
    const vec3  luminance = vec3 ( 0.3, 0.59, 0.11 );
    float       l         = dot ( luminance, base );
    float       scale     = l / ( 1.0 + l );
    vec3 color = base * scale + bloom_exposure * blur;

	return color;
}

void main()
{             
	vec2 bloom_size = 1.0/textureSize(bloomBlur,0);
	vec2 hdr_size = 1.0/textureSize(scene,0);
    vec3 hdrColor = textureLod(scene, (TexCoords - hdr_size)*scale, 0).rgb;      
    vec3 bloomColor = textureLod(bloomBlur, clamp((TexCoords - bloom_size)*scale, vec2(0.5)*bloom_size, vec2(1.0)), 0).rgb;
	vec3 result = vec3(0);
	vec3 color = vec3(hdrColor);
	#if 0
    if(bloom)
        color = hdrColor + bloomColor * bloom_exposure; // additive blending
	#endif
	//result = tonemap2(hdrColor, bloomColor);	
	result = tonemap_reinhard(color);
	result = vec3(float_to_sRGB(result.x), float_to_sRGB(result.y), float_to_sRGB(result.z));
    FragColor = vec4(result, 1.0);
}
