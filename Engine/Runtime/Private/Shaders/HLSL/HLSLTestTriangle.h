#pragma once

namespace Shaders
{
static const char* TestTriangle = R"HLSL(

float4 VSMain(float2 pos: POSITION) : SV_POSITION
{
  return float4(pos.x, pos.y, 0.0f, 1.0f);
}

float4 PSMain() : SV_TARGET
{
  return float4(1.0, 1.0, 1.0, 1.0);
}

)HLSL";
}
