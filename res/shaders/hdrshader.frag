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

void main()
{             
    const float gamma = 2.2;
	vec2 bloom_size = 1.0/textureSize(bloomBlur,0);
    vec3 hdrColor = texture(scene, TexCoords*scale).rgb;      
    vec3 bloomColor = texture(bloomBlur, clamp((TexCoords - bloom_size)*scale, vec2(0.5)*bloom_size, vec2(1.0))).rgb;
    if(bloom)
        hdrColor += bloomColor * bloom_exposure; // additive blending
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	result = vec3(float_to_sRGB(result.x), float_to_sRGB(result.y), float_to_sRGB(result.z));
    FragColor = vec4(result, 1.0);
}
