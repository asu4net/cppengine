// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"

D3D11BufferLayout::D3D11BufferLayout(const BufferLayoutParams& params)
{
  auto* device = D3D11GraphicsDevice::SafeGetNativeDevice(params.deviceHandle);
  if (device == nullptr)
  {
    return;
  }

  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();

  bool shaderValid = graphicsStorage.IsValid<Shader>(params.shaderHandle);
  ASSERT(shaderValid);
  if (!shaderValid)
  {
    LOG_ERR("D3D11 error: D3D11BufferLayout requires a valid shader to be constructed.");
    return;
  }

  auto* baseShader = graphicsStorage.Get<Shader>(params.shaderHandle);
  ASSERT(baseShader != nullptr);
  auto* shader = static_cast<D3D11Shader*>(baseShader);
  if (shader.stage != ShaderStage::Vertex)
  {
    LOG_ERR("D3D11 error: D3D11BufferLayout unsupported shader stage.");
    return;
  }

  constexpr std::size_t MAX_INPUT_ELEMS = 30;
  D3D11_INPUT_ELEMENT_DESC inputElements[MAX_INPUT_ELEMS];
  std::size_t inputElementCount = 0;
  if (params.layout.size() > MAX_INPUT_ELEMS)
  {
    LOG_ERR("D3D11 error: D3D11BufferLayout element count limit exceeded. Max is: {}", MAX_INPUT_ELEMENTS);
    return;
  }

  // Create the input layout.
  for (std::size_t i = 0; i < params.layout.size(); ++i)
  {
    inputElements[i] = 
    {
      "POSITION",
      0,
      DXGI_FORMAT_R32G32_FLOAT,
      0,
      0,
      D3D11_INPUT_PER_VERTEX_DATA,
      0
    };
  }

  const auto* bytecode = shader->GetNativeShaderData().bytecode;

  device->CreateInputLayout(
    inputElements,
    1,
    bytecode->GetBufferPointer(),
    bytecode->GetBufferSize(),
    &m_InputLayout
  );

  LOG_INFO("D3D11 Input Layout created.");
}

D3D11BufferLayout::~D3D11BufferLayout()
{
  LOG_INFO("D3D11 Input Layout destroyed.");
}
