#pragma once

#include "Runtime/GraphicsInstance.h"

class D3D11GraphicsContext;
class D3D11GraphicsDevice;

class D3D11GraphicsInstance : public GraphicsInstance
{
  public:
    D3D11GraphicsInstance(const GraphicsInstanceParams& params);
    ~D3D11GraphicsInstance();

    D3D11GraphicsInstance(const D3D11GraphicsInstance&) = delete;
    D3D11GraphicsInstance& operator=(const D3D11GraphicsInstance&) noexcept = delete;
    D3D11GraphicsInstance(D3D11GraphicsInstance&&) = delete;
    D3D11GraphicsInstance& operator=(D3D11GraphicsInstance&&) noexcept = delete;

    GraphicsDevice& GetDevice() override;
    GraphicsContext& GetContext() override;

    void DrawTestTriangle() override;

  private:
    GraphicsHandle m_ContextHandle;
    D3D11GraphicsContext* m_Context = nullptr;

    GraphicsHandle m_DeviceHandle;
    D3D11GraphicsDevice* m_Device = nullptr;
};

