#pragma once

namespace Shaders
{
static const char* TestTriangle = R"GLSL(

#ifdef VERT

layout(location = 0) in vec3 a_Position;

void main()
{
  gl_Position = vec4(a_Position, 1.0);
}

#endif

#ifdef FRAG

out vec4 o_Color;

void main()
{
  o_Color = vec4(1.0, 1.0, 1.0, 1.0);
}

#endif

)GLSL";
}

