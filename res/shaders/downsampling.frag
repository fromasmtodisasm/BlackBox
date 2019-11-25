#version 450 core
//layout (pixel_center_integer) in vec4 gl_FragCoord;
///////////////////////
layout(location = 0) out vec4 frag_color;

///////////////////////
uniform sampler2D image;
uniform float weight[2] = float[](0.125, 0.5);
uniform float offset = -3.0;
uniform float vx = 1;
uniform float vy = 1;
uniform int rx;
uniform int ry;

// Todo: specify calculation of offset for box filter
vec3 offsets[13] = vec3[](
  vec3(1, 1, weight[0]),
  vec3(3, 1, weight[0]),
  vec3(5, 1, weight[0]),

  vec3(2, 2, weight[1]),
  vec3(4, 2, weight[1]),

  vec3(1, 3, weight[0]),
  vec3(3, 3, weight[0]),
  vec3(5, 3, weight[0]),

  vec3(2, 4, weight[1]),
  vec3(4, 4, weight[1]),

  vec3(1, 5, weight[0]),
  vec3(3, 5, weight[0]),
  vec3(5, 5, weight[0])

  );

#define Sample(uv) textureLod(image, uv, 0)

vec4 get_texel(int i)
{
  vec4 result = vec4(0);
  vec2 tex_size = 1.0 / textureSize(image, 0); // gets size of single texel

  vec2 coord = (2 * (gl_FragCoord.xy) + (offsets[i].xy + offset));
  vec2 texel = clamp(coord, vec2(1), vec2(rx, ry) - 1) * tex_size;
  if (all(greaterThan(coord, vec2(rx, ry) - 1)))
  {
    vec2 coord = (2 * (gl_FragCoord.xy) + (offsets[i].xy + 2 * offset));
    texel = floor(clamp(coord, vec2(1), vec2(rx, ry) - 1)) * tex_size;
    result = Sample(texel);
  }
  else
  {
    result = Sample(texel) * offsets[i].z;
  }

  return result;
}

/*
  TexCoords - coordinates current pixel in out framebuffer
  image - n + 1 mip level
  texel_scale - step in relative units in input image -- key of sampling
*/

vec4 tm(vec4 a) {
  return a / (1.0 + a);
}

vec4 itm(vec4 a) {
  return a / (1.0 - a);
}

vec4 box4(vec4 a, vec4 b, vec4 c, vec4 d) {
  vec4 _a = tm(a);
  vec4 _b = tm(b);
  vec4 _c = tm(c);
  vec4 _d = tm(d);

  return itm( (_a + _b + _c + _d ) / (4.0 + a + b + c + d) );
}

vec4 downsampling(vec2 uv)
{
  vec4 result = vec4(0);
#if 0
  for (int i = 0; i < 13; i++)
  {
    result += get_texel(i);
  }
#else
    vec4 c0 = get_texel(0);
    vec4 c1 = get_texel(1);
    vec4 c2 = get_texel(2);
    vec4 c3 = get_texel(3);
    vec4 c4 = get_texel(4);
    vec4 c5 = get_texel(5);
    vec4 c6 = get_texel(6);
    vec4 c7 = get_texel(7);
    vec4 c8 = get_texel(8);
    vec4 c9 = get_texel(9);
    vec4 c10 = get_texel(10);
    vec4 c11 = get_texel(11);
    vec4 c12 = get_texel(12);

    result = box4(c0, c1, c5, c6) * 0.125 +
    box4(c1, c2, c6, c7) * 0.125 +
    box4(c5, c6, c10, c11) * 0.125 +
    box4(c6, c7, c11, c12) * 0.125 +
    box4(c3, c4, c8, c9) * 0.5;

#endif
  return result;
}

void main()
{
  vec2 uv = 2 * (gl_FragCoord.xy) / vec2(textureSize(image, 0));
  frag_color = downsampling(uv);
}
