#pragma once

#include "Runtime/SwapChain.h"

class D3D11SwapChain : public SwapChain
{
  public:
    D3D11SwapChain(const SwapChainParams& params);
    ~D3D11SwapChain();

    D3D11SwapChain(const D3D11SwapChain&) = delete;
    D3D11SwapChain& operator=(const D3D11SwapChain&) noexcept = delete;
    D3D11SwapChain(D3D11SwapChain&&) = delete;
    D3D11SwapChain& operator=(D3D11SwapChain&&) noexcept = delete;

    void Present(bool vsync = false) override;

    IDXGISwapChain** GetPointer() { return &m_SwapChain; }

  private:
    IDXGISwapChain* m_SwapChain = nullptr;
};

