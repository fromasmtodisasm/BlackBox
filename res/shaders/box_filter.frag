#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec2 texelSize;
uniform bool use_box_filter = true;
uniform bool default_filter = true;

uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

vec4 Sample(vec2 uv) {
	return texture2D(image, uv);
}

vec3 Blur()
{
     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
     vec3 result = texture(image, TexCoords).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
             result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }

	 return result;

}

vec4 SampleBox(vec2 uv) 
{
	//vec4 o = (1.0 / textureSize(image, 0)).xyxy * vec2(-1, 1).xxyy;
	vec2 tex_offset = (1.0 / textureSize(image, 0));
	vec3 result;
	/*
	vec4 s =
		Sample(uv + o.xy) + Sample(uv + o.zy) +
		Sample(uv + o.xw) + Sample(uv + o.zw);
	*/
	if (default_filter)
	{
		result = Sample(uv).rgb;
	}
	else if (use_box_filter) {
		result += Sample(uv + vec2(0.0, 0.0)).rgb;
		result += Sample(uv + vec2(tex_offset.x, 0.0)).rgb;
		result += Sample(uv + vec2(0.0, tex_offset.y)).rgb;
		result += Sample(uv + vec2(tex_offset.x, tex_offset.y)).rgb;
		result *= 0.25;
	}
	else {
		//result += Sample(uv).rgb;
		result = Blur();

	}
	return vec4(result,1);
}

void main()
{
	vec4 c = vec4(0);
	FragColor = SampleBox(TexCoords) + c;
}