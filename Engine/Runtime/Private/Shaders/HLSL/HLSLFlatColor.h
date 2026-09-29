#pragma once

namespace Shaders
{
static const char* FlatColor = R"HLSL(

struct VSInput
{
  float3 Position : POSITION;
};

struct VSOutput
{
  float4 Position : SV_POSITION;
};

cbuffer FlatColorBuffer : register(b0)
{
  float4 Color;
};

VSOutput VSMain(VSInput input)
{
  VSOutput output;

  output.Position = float4(input.Position, 1.0);

  return output;
}

float4 PSMain(VSOutput input) : SV_TARGET
{
  return Color;
}

)HLSL";
}
