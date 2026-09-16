#pragma once

struct ShaderParams
{
  std::string_view source;
};

class Shader
{
  public:
    virtual ~Shader() = default;

    virtual void Use() = 0;
};
