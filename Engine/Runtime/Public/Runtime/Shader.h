#pragma once

enum class ShaderStage
{
  None,
  Vertex,
  Pixel
  // @Pending: Add Compute support.
}

struct ShaderParams
{
  std::string_view name;
  std::string_view source;
  ShaderStage stage = ShaderStage::None;
  GraphicsHandle device;
};

class Shader
{
  public:
    virtual ~Shader() = default;
};
