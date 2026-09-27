#include "D3D11SwapChain.h"

D3D11SwapChain::D3D11SwapChain(const SwapChainParams& params)
{
  // @Note: The graphics instance is responsible to create
  // this and give a value to the d3d11 pointer.
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
