#include "OpenGLGraphicsInstance.h"

OpenGLGraphicsInstance::OpenGLGraphicsInstance(const GraphicsInstanceParams& params)
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();

  // Create the device.
  GraphicsDeviceParams graphicsDeviceParams{ params.windowHandle };
  m_DeviceHandle = graphicsStorage.Emplace<GraphicsDevice>(graphicsDeviceParams);
  m_Device = graphicsStorage.Get<GraphicsDevice>(m_DeviceHandle);

  // Create the instance.
  GraphicsContextParams graphicsContextParams{ params.windowHandle };
  m_ContextHandle = graphicsStorage.Emplace<GraphicsContext>(graphicsContextParams);
  m_Context = graphicsStorage.Get<GraphicsContext>(m_ContextHandle);
}

OpenGLGraphicsInstance::~OpenGLGraphicsInstance()
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
  graphicsStorage.Remove<GraphicsContext>(m_ContextHandle);
  graphicsStorage.Remove<GraphicsDevice>(m_DeviceHandle);
}

GraphicsDevice& OpenGLGraphicsInstance::GetDevice() const
{
  ASSERT(m_Device != nullptr);
  return *m_Device;
}

GraphicsContext& OpenGLGraphicsInstance::GetContext() const
{
  ASSERT(m_Context != nullptr);
  return *m_Context;
}
