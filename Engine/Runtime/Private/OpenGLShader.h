#pragma once

#include "Runtime/Shader.h"

class OpenGLShader : public Shader
{
  public:
    OpenGLShader(const ShaderParams& params);
    ~OpenGLShader();
    
    void Use() override;

  private:
};
