#pragma once

namespace Shaders
{
static const char* FlatColor = R"GLSL(

#ifdef VERT

layout(location = 0) in vec3 a_Position;

void main()
{
  gl_Position = vec4(a_Position, 1.0);
}

#endif

#ifdef FRAG

layout(std140, binding = 0) uniform FlatColorBuffer
{
  vec4 u_Color;
};

out vec4 o_Color;

void main()
{
  o_Color = u_Color;
}

#endif

)GLSL";
}
