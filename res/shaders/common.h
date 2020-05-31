#define LAYOUT(n) layout (location = n)
#define POSITION_ATTR(attr) LAYOUT(0) in vec3 attr
#define NORMAL_ATTR(attr)   LAYOUT(1) in vec3 attr
#define TC_ATTR(attr)       LAYOUT(2) in vec2 attr
#define COLOR_ATTR(attr)    LAYOUT(5) in vec4 attr

#if 0
layout (std140) uniform PerFrameState
{
  mat4 View;
  mat4 Projection;
};
#endif
