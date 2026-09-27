#include "D3D11GraphicsDevice.h"

D3D11GraphicsDevice::D3D11GraphicsDevice(const GraphicsDeviceParams& params)
{
  // @Note: The graphics instance is responsible to create
  // this and give a value for the d3d11 pointers.
  LOG_INFO("D3D11 Device created!");
}

D3D11GraphicsDevice::~D3D11GraphicsDevice()
{
  ASSERT(m_SwapChain != nullptr);
  if (m_SwapChain != nullptr)
  {
    auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
    graphicsStorage.Remove<SwapChain>(m_SwapChainHandle);
    m_SwapChainHandle = {};
    m_SwapChain = nullptr;
  }

  ASSERT(m_Device != nullptr);
  if (m_Device != nullptr)
  {
    m_Device->Release();
    m_Device = nullptr;
  }

  LOG_INFO("D3D11 Device destroyed!");
}


SwapChain& D3D11GraphicsDevice::GetSwapChain()
{
  ASSERT(m_SwapChain != nullptr);
  return *m_SwapChain;
}
