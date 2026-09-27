#pragma once

#include "Runtime/GraphicsDevice.h"

class SwapChain;
class D3D11SwapChain;

class D3D11GraphicsDevice : public GraphicsDevice
{
  public:
    D3D11GraphicsDevice(const GraphicsDeviceParams& params);
    ~D3D11GraphicsDevice();

    D3D11GraphicsDevice(const D3D11GraphicsDevice&) = delete;
    D3D11GraphicsDevice& operator=(const D3D11GraphicsDevice&) noexcept = delete;
    D3D11GraphicsDevice(D3D11GraphicsDevice&&) = delete;
    D3D11GraphicsDevice& operator=(D3D11GraphicsDevice&&) noexcept = delete;

    SwapChain& GetSwapChain() override;

    ID3D11Device** GetPointer() { return &m_Device; }

    // @Note: Intended to be called from the graphics instance.
    void SetSwapChain(GraphicsHandle swapChainHandle, D3D11SwapChain* swapChain) 
    { 
      m_SwapChainHandle = swapChainHandle;
      m_SwapChain = swapChain;
    };

  private:
    ID3D11Device* m_Device = nullptr;
    GraphicsHandle m_SwapChainHandle;
    D3D11SwapChain* m_SwapChain = nullptr;
};


