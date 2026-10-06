#pragma once

#include "Runtime/VertexBuffer.h"

class D3D11VertexBuffer : public VertexBuffer
{
  public: 
    D3D11VertexBuffer(const VertexBufferParams& params);
    ~D3D11VertexBuffer();

    D3D11VertexBuffer(const D3D11VertexBuffer&) = delete;
    D3D11VertexBuffer& operator=(const D3D11VertexBuffer&) noexcept = delete;
    D3D11VertexBuffer(D3D11VertexBuffer&&) = delete;
    D3D11VertexBuffer& operator=(D3D11VertexBuffer&&) noexcept = delete;

    ID3D11Buffer* GetNativeVertexBuffer() const { return m_VertexBuffer; }

  private:
    ID3D11Buffer* m_VertexBuffer = nullptr;
};
