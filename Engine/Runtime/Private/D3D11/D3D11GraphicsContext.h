#pragma once

#include "Runtime/GraphicsContext.h"

class D3D11GraphicsContext : public GraphicsContext
{
  public:
    D3D11GraphicsContext(const GraphicsContextParams& params);
    ~D3D11GraphicsContext();

    D3D11GraphicsContext(const D3D11GraphicsContext&) = delete;
    D3D11GraphicsContext& operator=(const D3D11GraphicsContext&) noexcept = delete;
    D3D11GraphicsContext(D3D11GraphicsContext&&) = delete;
    D3D11GraphicsContext& operator=(D3D11GraphicsContext&&) noexcept = delete;

    ID3D11DeviceContext** GetPointer() { return &m_Context; }

  private:
    ID3D11DeviceContext* m_Context;
};


