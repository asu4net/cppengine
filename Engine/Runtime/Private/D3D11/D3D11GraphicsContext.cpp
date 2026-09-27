#include "D3D11GraphicsContext.h"

D3D11GraphicsContext::D3D11GraphicsContext(const GraphicsContextParams& params)
{
  // @Note: The graphics instance is responsible to create
  // this and give a value to the d3d11 pointer.
  LOG_INFO("D3D11 Device Context created!");
}

D3D11GraphicsContext::~D3D11GraphicsContext()
{
  ASSERT(m_Context != nullptr);
  if (m_Context != nullptr)
  {
    m_Context->Release();
    m_Context = nullptr;
  }
  LOG_INFO("D3D11 Device Context destroyed!");
}

void D3D11GraphicsContext::ClearBackBuffer(SwapChain& swapChain, float r, float g, float b)
{
  auto& d3d11SwapChain = static_cast<D3D11SwapChain&>(swapChain);
  auto** renderTargetViewPointer = d3d11SwapChain.GetRenderTargetViewPointer();
  ASSERT(renderTargetViewPointer != nullptr);
  if (renderTargetViewPointer == nullptr)
  {
    LOG_ERR("D3D11 error: Most likely GetRenderTargetViewPointer is not implemented.");
    return;
  }

  auto* renderTargetView = *renderTargetViewPointer;
  ASSERT(renderTargetView != nullptr); 
  if (renderTargetView == nullptr)
  {
    LOG_ERR("D3D11 error: Most likely the render target view didn't get properly created.");
    return;
  }

  ASSERT(m_Context != nullptr);
  if (!m_Context)
  {
    LOG_ERR("D3D11 error: Context didn't get properly created.");
    return;
  }

  float clearColor[] = { r, g, b, 1.0f };
  m_Context->ClearRenderTargetView(renderTargetView, clearColor);
}
