#include "D3D11SwapChain.h"

D3D11SwapChain::D3D11SwapChain(const SwapChainParams& params)
{
  ASSERT(params.nativeSwapChain != nullptr);
  if (params.nativeSwapChain == nullptr)
  {
    LOG_ERR("D3D11 Error: The nativeSwapChain pointer can't be nullptr.");
    std::exit(EXIT_FAILURE);
  }

  ASSERT(params.nativeDevice != nullptr);
  if (params.nativeDevice == nullptr)
  {
    LOG_ERR("D3D11 Error: The nativeDevice pointer can't be nullptr.");
    std::exit(EXIT_FAILURE);
  }

  // @Note: We trust that the native pointers provided are of 
  // the right types.   
  m_SwapChain = reinterpret_cast<IDXGISwapChain*>(params.nativeSwapChain);
  auto* nativeDevice = reinterpret_cast<ID3D11Device*>(params.nativeDevice);

  // Create the render target view.

  // Get the back buffer.
  ID3D11Resource* backBuffer = nullptr;
  m_SwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
  ASSERT(backBuffer != nullptr);

  if (backBuffer == nullptr)
  {
    LOG_ERR("D3D11 Error retrieving the back buffer.");
    std::exit(EXIT_FAILURE);
  }

  // Create the render target view, filling the pointer in the swap chain.
  ID3D11RenderTargetView* nativeRenderTargetView = nullptr;
  nativeDevice->CreateRenderTargetView(backBuffer, nullptr, &nativeRenderTargetView);
  // @Pending check if valid renderTargetView
  m_RenderTargetView = nativeRenderTargetView;
  
  // We don't need the back buffer anymore.
  backBuffer->Release();
  backBuffer = nullptr;

  LOG_INFO("D3D11 Swap Chain created!");
}

D3D11SwapChain::~D3D11SwapChain()
{
  ASSERT(m_RenderTargetView != nullptr);
  if (m_RenderTargetView != nullptr)
  {
    m_RenderTargetView->Release();
    m_RenderTargetView = nullptr;
  }
  ASSERT(m_SwapChain != nullptr);
  if (m_SwapChain != nullptr)
  {
    m_SwapChain->Release();
    m_SwapChain = nullptr;
  }
  LOG_INFO("D3D11 Swap Chain destroyed!");
}

void D3D11SwapChain::Present(bool vsync)
{
  ASSERT(m_SwapChain != nullptr);
  // @Note: Sync Interval
  // 0 -> Present without waiting the vsync (vsync off)
  // 1 -> Waits 1 vsync
  // 2 -> Waits 2 vsyncs, etc.
  // @Note: Second parameter are flags
  m_SwapChain->Present(vsync ? 1 : 0, 0);
}
