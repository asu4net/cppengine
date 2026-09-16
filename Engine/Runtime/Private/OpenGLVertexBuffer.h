#pragma once

class OpenGLVertexBuffer : public VertexBuffer
{
  public: 
    OpenGLVertexBuffer(const VertexBufferParams& params);
    ~OpenGLVertexBuffer();

    void Use() override;
    void SetData(const void* data, std::uint32_t size) override;
  private:
};
