#pragma once

#include "Runtime/GraphicsDevice.h"

class SwapChain;
class D3D11SwapChain;

class D3D11GraphicsDevice final : public GraphicsDevice
{
  public:
    // @Note: Helper function, meant to be called from graphic
    // resource objects to retrieve the native device pointer
    // without dealing with error checking and other stuff.
    static ID3D11Device* SafeGetNativeDevice(GraphicsHandle deviceHandle);

    D3D11GraphicsDevice(const GraphicsDeviceParams& params);
    ~D3D11GraphicsDevice();

    D3D11GraphicsDevice(const D3D11GraphicsDevice&) = delete;
    D3D11GraphicsDevice& operator=(const D3D11GraphicsDevice&) noexcept = delete;
    D3D11GraphicsDevice(D3D11GraphicsDevice&&) = delete;
    D3D11GraphicsDevice& operator=(D3D11GraphicsDevice&&) noexcept = delete;

    SwapChain& GetSwapChain() override;

    ID3D11DeviceContext* GetNativeContext() { return m_Context; } 
    ID3D11Device* GetNativeDevice() { return m_Device; } 

    void DumpDebugMessages() override;
    void ClearBackBuffer(float r = 0, float g = 0, float b = 0) override;

    void SetGraphicsState(const GraphicsState& state) override;
    void ImmediateDraw() override;

  private:
    ID3D11Device* m_Device = nullptr;
    ID3D11DeviceContext* m_Context = nullptr;
    GraphicsHandle m_SwapChainHandle;
    D3D11SwapChain* m_SwapChain = nullptr;
    GraphicsState m_State;
};


