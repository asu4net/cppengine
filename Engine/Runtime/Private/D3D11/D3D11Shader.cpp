// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"
D3D11Shader::D3D11Shader(const ShaderParams& params)
{
  auto* device = D3D11GraphicsDevice::SafeGetNativeDevice(params.deviceHandle);
  if (device == nullptr)
  {
    return;
  }

  if (params.stage == ShaderStage::None)
  {
    LOG_ERR("D3D11 error: Shader stage unespecified whem creating shader");
    return;
  }

  ID3DBlob* errors = nullptr;

  HRESULT result = D3DCompile(
    params.source.data(),                  // SrcData.
    params.source.size(),                  // SrcDataSize.
    params.name.data(),                    // SourceName.
    nullptr,                               // Defines.
    nullptr,                               // Include.
    StageToEntryPoint(m_ShaderData.stage); // EntryPoint
    StageToTarget(m_ShaderData.stage);     // EntryPoint
    D3DCOMPILE_ENABLE_STRICTNESS,          // Flags1.
    0,                                     // Flags2.
    &m_ShaderData.bytecode,                // Bytecode.
    &errors                                // Errors.
  );

  if (FAILED(result))
  {
    if (errors != nullptr)
    {
      const char* message = static_cast<const char*>(errors->GetBufferPointer());
      LOG_ERR("D3D11 Shader compilation failed: {}", message);
      return;
    }
  }

  m_ShaderData.stage = params.stage;
  bool success = CreateShader(device, m_ShaderData);
  if (!success)
  {
    LOG_ERR("D3D11 Unknown error creating the shader");
  }

  LOG_INFO("D3D11 Shader created.");
}

D3D11Shader::~D3D11Shader()
{
  switch (m_ShaderData.stage)
  {
    case ShaderStage::Vertex:
      m_ShaderData.vs->Release();
      break;
    case ShaderStage::Pixel:
      m_ShaderData.ps->Release();
      m_ShaderData = {};
      break;
  };

  m_ShaderData = {};
  LOG_INFO("D3D11 Shader destroyed.");
}

// @Static:

const char* D3D11Shader::StageToEntryPoint(ShaderStage stage)
{
  switch (stage)
  {
    case ShaderStage::Vertex: return "VSMain";
    case ShaderStage::Pixel:  return "PSMain";
    case default: return "";
  };
}

const char* D3D11Shader::StageToTarget(ShaderStage stage)
{
  switch (stage)
  {
    case ShaderStage::Vertex: return "vs_5_0";
    case ShaderStage::Pixel:  return "ps_5_0";
    case default: return "";
  };
}

void D3D11Shader::CreateShader(ID3D11Device* device, D3D11ShaderData& shaderData)
{
  switch (shaderData.stage)
  {
    case ShaderStage::Vertex:
      device->CreateVertexShader(
        shaderData.bytecode->GetBufferPointer(),
        shaderData.bytecode->GetBufferSize(),
        nullptr,
        &shaderData.vs
      );
      return true;
    case ShaderStage::Pixel:
      device->CreatePixelShader(
        shaderData.bytecode->GetBufferPointer(),
        shaderData.bytecode->GetBufferSize(),
        nullptr,
        &shaderData.ps
      );
      return true;
    case default: return false;
  };
}
