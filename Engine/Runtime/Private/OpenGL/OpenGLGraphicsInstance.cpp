#include "OpenGLGraphicsInstance.h"

OpenGLGraphicsInstance::OpenGLGraphicsInstance(const GraphicsInstanceParams& params)
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();

  // Alloc the device.
  GraphicsDeviceParams graphicsDeviceParams{ params.windowHandle };
  m_DeviceHandle = graphicsStorage.Emplace<GraphicsDevice>(graphicsDeviceParams);
  auto* deviceInterface = graphicsStorage.Get<GraphicsDevice>(m_DeviceHandle);
  m_Device = static_cast<OpenGLGraphicsDevice*>(deviceInterface);

  // Alloc the context.
  GraphicsContextParams graphicsContextParams{ params.windowHandle };
  m_ContextHandle = graphicsStorage.Emplace<GraphicsContext>(graphicsContextParams);
  auto* contextInterface = graphicsStorage.Get<GraphicsContext>(m_ContextHandle);
  m_Context = static_cast<OpenGLGraphicsContext*>(contextInterface);

  // Alloc the Swap Chain.
  // @Note: Obvioulsy OpenGL does not have an exposed swap chain. This is
  // just an interface to call swap buffers on the GL state machine and 
  // for keeping the graphics abstraction layer coherent.
  SwapChainParams swapChainParams;
  GraphicsHandle swapChainHandle = graphicsStorage.Emplace<SwapChain>(swapChainParams);
  auto* swapChainInterface = graphicsStorage.Get<SwapChain>(swapChainHandle);

  auto* openglSwapChain = static_cast<OpenGLSwapChain*>(swapChainInterface);

  // @Hack: Provide a window handle to the swap chain.
  // Since opengl depends on OS impl to do the present, the
  // swap chain needs a handle to the OS window.
  openglSwapChain->SetWindowHandle(params.windowHandle);

  // Give the Device ownership over the swap chain.
  m_Device->SetSwapChain(swapChainHandle, openglSwapChain);
}

OpenGLGraphicsInstance::~OpenGLGraphicsInstance()
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
  graphicsStorage.Remove<GraphicsContext>(m_ContextHandle);
  graphicsStorage.Remove<GraphicsDevice>(m_DeviceHandle);
}

GraphicsDevice& OpenGLGraphicsInstance::GetDevice()
{
  ASSERT(m_Device != nullptr);
  return *m_Device;
}

GraphicsContext& OpenGLGraphicsInstance::GetContext()
{
  ASSERT(m_Context != nullptr);
  return *m_Context;
}
