// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"

D3D11VertexBuffer::D3D11VertexBuffer(const VertexBufferParams& params)
{
  auto* device = D3D11GraphicsDevice::GetNativeDevice(params.deviceHandle);
  if (device == nullptr)
  {
    return;
  }

  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.ByteWidth = params.vertSize * vertCount;
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bufferDesc.CPUAccessFlags = 0u;
  bufferDesc.MiscFlags = 0u;
  bufferDesc.StructureByteStride = params.VertSize;
  
  D3D11_SUBRESOURCE_DATA data = {};
  data.pSysMem = params.vertices;
  // These two are for textures.
  data.SysMemPitch = 0u;
  data.SysMemSlicePitch = 0u;

  HRESULT result = device->CreateBuffer(&bufferDesc, &data, &m_VertexBuffer);
  if FAILED(result)
  {
    device->DumpDebugMessages();
    return;
  }

  LOG_INFO("D3D11 VertexBuffer created.");
}

D3D11VertexBuffer::~D3D11VertexBuffer()
{
  if (m_VertexBuffer != nullptr)
  {
    m_VertexBuffer->Release();
    m_VertexBuffer = nullptr;
  }
  LOG_INFO("D3D11 VertexBuffer destroyed.");
}
