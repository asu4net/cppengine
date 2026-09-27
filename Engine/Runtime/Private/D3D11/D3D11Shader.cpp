// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"

D3D11Shader::D3D11Shader(const ShaderParams& params)
{
  LOG_INFO("D3D11 Shader created.");
}

D3D11Shader::~D3D11Shader()
{
  LOG_INFO("D3D11 Shader destroyed.");
}

void D3D11Shader::Use()
{
  LOG_INFO("D3D11 Shader used.");
}
