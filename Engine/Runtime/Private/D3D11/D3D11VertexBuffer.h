#pragma once

#include "Runtime/VertexBuffer.h"

class D3D11VertexBuffer : public VertexBuffer
{
  public: 
    D3D11VertexBuffer(const VertexBufferParams& params);
    ~D3D11VertexBuffer();

    void Use() override;
    void SetData(const void* data, std::uint32_t size) override;
  private:
};
