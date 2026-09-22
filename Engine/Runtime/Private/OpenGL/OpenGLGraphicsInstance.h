#pragma once

#include "Runtime/GraphicsInstance.h"

class OpenGLGraphicsInstance : public GraphicsInstance
{
  public:
    OpenGLGraphicsInstance(const GraphicsInstanceParams& params);
    ~OpenGLGraphicsInstance();

    OpenGLGraphicsInstance(const OpenGLGraphicsInstance&) = delete;
    OpenGLGraphicsInstance& operator=(const OpenGLGraphicsInstance&) noexcept = delete;
    OpenGLGraphicsInstance(OpenGLGraphicsInstance&&) = delete;
    OpenGLGraphicsInstance& operator=(OpenGLGraphicsInstance&&) noexcept = delete;

    GraphicsDevice& GetDevice() const override;
    GraphicsContext& GetContext() const override;
  private:
    GraphicsHandle m_ContextHandle;
    GraphicsContext* m_Context;

    GraphicsHandle m_DeviceHandle;
    GraphicsDevice* m_Device;
};
