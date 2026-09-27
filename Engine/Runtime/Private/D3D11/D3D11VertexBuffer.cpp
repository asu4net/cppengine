// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"

D3D11VertexBuffer::D3D11VertexBuffer(const VertexBufferParams& params)
{
  LOG_INFO("D3D11 VertexBuffer created.");
}

D3D11VertexBuffer::~D3D11VertexBuffer()
{
  LOG_INFO("D3D11 VertexBuffer destroyed.");
}

void D3D11VertexBuffer::Use()
{
  LOG_INFO("D3D11 VertexBuffer used.");
}

void D3D11VertexBuffer::SetData(const void* data, std::uint32_t size)
{
  LOG_INFO("D3D11 VertexBuffer updated data.");
}
