#pragma once

#include "Runtime/Shader.h"

struct D3D11ShaderData 
{
  ShaderStage stage = ShaderStage::None;  
  union 
  {
    ID3D11VertexShader* vs;
    ID3D11PixelShader*  ps;
  };
  ID3DBlob* bytecode;
}

class D3D11Shader : public Shader
{
  public:
    D3D11Shader(const ShaderParams& params);
    ~D3D11Shader();

    D3D11Shader(const D3D11Shader&) = delete;
    D3D11Shader& operator=(const D3D11Shader&) noexcept = delete;
    D3D11Shader(D3D11Shader&&) = delete;
    D3D11Shader& operator=(D3D11Shader&&) noexcept = delete;

    const ID3D11ShaderData& GetNativeShaderData() const { return m_ShaderData; }

private:
    static bool CreateShader(ID3D11Device* device, D3D11ShaderData& shaderData);
    static const char* StageToEntryPoint(ShaderStage stage);
    static const char* StageToTarget(ShaderStage stage);

    ID3D11ShaderData m_ShaderData;
};
