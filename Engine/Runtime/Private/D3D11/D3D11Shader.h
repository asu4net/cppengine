#pragma once

#include "Runtime/Shader.h"

class D3D11Shader : public Shader
{
  public:
    D3D11Shader(const ShaderParams& params);
    ~D3D11Shader();
    
    void Use() override;

  private:
};
