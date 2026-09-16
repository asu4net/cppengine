// @Note: This translation unit gets compiled inside "Runtime/GraphicsStorage.cpp"

OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexBufferParams& params)
{
  LOG_INFO("OpenGL VertexBuffer created.");
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
  LOG_INFO("OpenGL VertexBuffer destroyed.");
}

void OpenGLVertexBuffer::Use()
{
  LOG_INFO("OpenGL VertexBuffer used.");
}

void OpenGLVertexBuffer::SetData(const void* data, std::uint32_t size)
{
  LOG_INFO("OpenGL VertexBuffer updated data.");
}
