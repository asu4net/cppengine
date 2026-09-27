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

