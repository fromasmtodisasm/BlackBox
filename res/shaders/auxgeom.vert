#version 460 core
#extension GL_ARB_separate_shader_objects : enable
#include "common.h"

#if 0
#pragma attribute(position, aPos)
#pragma attribute(color, aColor)
#pragma attribute(tc, aTC)
#else
POSITION_ATTR(aPos);
COLOR_ATTR(aColor);
TC_ATTR(aTC);
#endif

layout(binding = 0) uniform MatrixBlock
{
  mat4 projection;
  mat4 view;
  mat4 model;
};

layout(location = 0) out vec4 v_color;

struct VS_OUT
{
  vec4 color;
  vec4 pos;
};

VS_OUT vert()
{
  VS_OUT res;

  return res;
}

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_Position = projection * view * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
	//v_color = aColor;
	v_color = vec4(aColor);
} 
