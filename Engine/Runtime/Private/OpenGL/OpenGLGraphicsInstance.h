#pragma once

#include "Runtime/GraphicsInstance.h"

class OpenGLGraphicsContext;
class OpenGLGraphicsDevice;

class OpenGLGraphicsInstance : public GraphicsInstance
{
  public:
    OpenGLGraphicsInstance(const GraphicsInstanceParams& params);
    ~OpenGLGraphicsInstance();

    OpenGLGraphicsInstance(const OpenGLGraphicsInstance&) = delete;
    OpenGLGraphicsInstance& operator=(const OpenGLGraphicsInstance&) noexcept = delete;
    OpenGLGraphicsInstance(OpenGLGraphicsInstance&&) = delete;
    OpenGLGraphicsInstance& operator=(OpenGLGraphicsInstance&&) noexcept = delete;

    GraphicsDevice& GetDevice() override;

  private:
    GraphicsHandle m_DeviceHandle;
    OpenGLGraphicsDevice* m_Device;
};
