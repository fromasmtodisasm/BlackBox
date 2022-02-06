#version 460 core
#extension GL_NV_gpu_shader5 : enable
#extension GL_NV_shader_buffer_load : enable
#extension GL_ARB_gpu_shader_fp64 : enable
///////////////////////
#define pixout vec3
#define GetLuminance luma
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
bool full_karis = false;
uniform int pass;

// Luma function with Rec.709 HDTV Standard
float luma(vec3 c)
{
#if LINEAR_COLOR
  c = LinearToGammaSpace(c);
#endif
  return dot(c, vec3(0.2126, 0.7152, 0.0722));
}

// Todo: specify calculation of offset for box filter
vec3 offsets[13] = vec3[](
  vec3(1, 1, weight[0]), // 0
  vec3(3, 1, weight[0]), // 1
  vec3(5, 1, weight[0]), // 2

  vec3(2, 2, weight[1]), // 3
  vec3(4, 2, weight[1]), // 4

  vec3(1, 3, weight[0]), // 5
  vec3(3, 3, weight[0]), // 6
  vec3(5, 3, weight[0]), // 7

  vec3(2, 4, weight[1]), // 8
  vec3(4, 4, weight[1]), // 9

  vec3(1, 5, weight[0]), // 10
  vec3(3, 5, weight[0]), // 11
  vec3(5, 5, weight[0])  // 12

  );

#define Sample(uv) textureLod(image, uv, 0)

vec4 get_texel_original(int i)
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
    result = Sample(texel + 0/*.25*/*tex_size);
  }

  return result;
}

vec4 get_texel(vec2 offset)
{
  vec4 result = vec4(0);
  vec2 tex_size = 1.0 / textureSize(image, 0); // gets size of single texel

  const vec2 dev = vec2(0.5);
  vec2 coord = (2 * (gl_FragCoord.xy + dev) + offset);
  //vec2 texel = clamp(coord, vec2(1), vec2(rx, ry) - 1) * tex_size;
  vec2 texel = coord * tex_size;
  /*
  if (all(greaterThan(coord, vec2(rx, ry) - 1)))
  {
    vec2 coord = (2 * (gl_FragCoord.xy + dev) + offset);
    texel = clamp(coord, vec2(1), vec2(rx, ry) - 1) * tex_size;
    result = Sample(texel);
  }
  else
  {
    result = Sample(texel);
  }
  */

  //return result;
  return Sample(texel);

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

float calc_weight(vec3 c)
{
  return 1 / (1 + luma(c));
}

vec4 func(vec4 c)
{
  return c * calc_weight(c.xyz);
}

vec3 downsampling(vec2 uv)
{
#if 0
  vec4 result1 = vec4(0);
  vec4 result = vec4(0);
#if 0
  for (int i = 0; i < 13; i++)
  {
    result += get_texel(i);
  }
#else
    vec4 c0 = get_texel_original(0);
    vec4 c1 = get_texel_original(1);
    vec4 c2 = get_texel_original(2);
    vec4 c3 = get_texel_original(3);
    vec4 c4 = get_texel_original(4);
    vec4 c5 = get_texel_original(5);
    vec4 c6 = get_texel_original(6);
    vec4 c7 = get_texel_original(7);
    vec4 c8 = get_texel_original(8);
    vec4 c9 = get_texel_original(9);
    vec4 c10 = get_texel_original(10);
    vec4 c11 = get_texel_original(11);
    vec4 c12 = get_texel_original(12);

    vec4 results[13];
    if (full_karis)
    {
      //result1 = 
        results[0] = func(c0) ;
        results[1] = func(c1) ;
        results[2] = func(c2) ;
        results[3] = func(c3) ;
        results[4] = func(c4) ;
        results[5] = func(c5) ;
        results[6] = func(c6) ;
        results[7] = func(c7) ;
        results[8] = func(c8) ;
        results[9] = func(c9) ;
        results[10] = func(c10) ;
        results[11] = func(c11) ;
        results[12] = func(c12);
        
        for (int i = 0; i < 13; i++)
        {
          result1 += results[i];
        }

      float all_weights;
      if (pass == 0)
      {
        all_weights = 
          calc_weight(vec3(c0)) +
          calc_weight(vec3(c1)) +
          calc_weight(vec3(c2)) +
          calc_weight(vec3(c3)) +
          calc_weight(vec3(c4)) +
          calc_weight(vec3(c5)) +
          calc_weight(vec3(c6)) +
          calc_weight(vec3(c7)) +
          calc_weight(vec3(c8)) +
          calc_weight(vec3(c9)) +
          calc_weight(vec3(c10 )) +
          calc_weight(vec3(c11)) +
          calc_weight(vec3(c12));
          result = result1 / all_weights;
      }
      else
      {
        for (int i = 0; i < 13; i++)
        {
          result += results[i] * offsets[i].z;
        }
      }


    }
    else
    {
      if (pass < 2)
      {
        result = 
          box4(c0, c1, c5, c6) * 0.125 +
          box4(c1, c2, c6, c7) * 0.125 +
          box4(c5, c6, c10, c11) * 0.125 +
          box4(c6, c7, c11, c12) * 0.125 +
          box4(c3, c4, c8, c9) * 0.5;
      }
      else
      {
        for (int i = 0; i < 13; i++)
        {
          result += results[i] /13; //*offsets[i].z / 3.125;
        }
      }

    }
  return vec3(result);
#endif
#else
  // High quality downsampling filter to reduce bloom flickering
   // Filter combines five 4x4 blocks (sampled bilinearly)
   // Reduces fireflies by applying tonemapping before averaging samples for each block

  pixout OUT = (pixout)0;

  const bool bKillFireflies = true;

  //const vec2 TexSize = 1 / (PS_ScreenSize.xy * 2);

  vec3 blockTL = vec3(0), blockTR = vec3(0), blockBR = vec3(0), blockBL = vec3(0);
  vec3 tex;

  tex = get_texel(vec2(-2, -2)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockTL += tex;

  tex = get_texel(vec2(0, -2)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockTL += tex; blockTR += tex;

  tex = get_texel(vec2(2, -2)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockTR += tex;

  tex = get_texel(vec2(-2, 0)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockTL += tex; blockBL += tex;

  tex = get_texel(vec2(0, 0)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockTL += tex; blockTR += tex; blockBR += tex; blockBL += tex;

  tex = get_texel(vec2(2, 0)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockTR += tex; blockBR += tex;

  tex = get_texel(vec2(-2, 2)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockBL += tex;

  tex = get_texel(vec2(0, 2)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockBL += tex; blockBR += tex;

  tex = get_texel(vec2(2, 2)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockBR += tex;

  vec3 blockCC = vec3(0);
  tex = get_texel(vec2(-1, -1)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockCC += tex;
  tex = get_texel(vec2(1, -1)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockCC += tex;
  tex = get_texel(vec2(1, 1)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockCC += tex;
  tex = get_texel(vec2(-1, 1)).rgb;
  if (bKillFireflies) tex /= 1 + GetLuminance(tex);
  blockCC += tex;

  blockTL /= 4; blockTR /= 4; blockBR /= 4; blockBL /= 4; blockCC /= 4;

  if (bKillFireflies)
  {
    // Convert back to uncompressed/linear range
    blockTL /= (1 - GetLuminance(blockTL));
    blockTR /= (1 - GetLuminance(blockTR));
    blockBR /= (1 - GetLuminance(blockBR));
    blockBL /= (1 - GetLuminance(blockBL));
    blockCC /= (1 - GetLuminance(blockCC));
  }

  OUT.rgb = 0.5 * blockCC + 0.125 * (blockTL + blockTR + blockBR + blockBL);
  return OUT;
#endif
}

void main()
{
  vec2 uv = 2 * (gl_FragCoord.xy) / vec2(textureSize(image, 0));
  frag_color = vec4(downsampling(uv), 1);
}

