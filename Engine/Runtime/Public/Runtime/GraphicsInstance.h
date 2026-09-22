#pragma once

struct GraphicsInstanceParams
{
  void* windowHandle = nullptr;
};

class GraphicsDevice;
class GraphicsContext;

// @Note: When created will create a device and a device context. And will setup
// any needed stuff to get the graphics up and running. 
// @Pending: For the OpenGL implementation we should move here the OpenGL 
// context creation.
class GraphicsInstance
{
  public:
    virtual ~GraphicsInstance() = default;

    virtual GraphicsDevice& GetDevice() const = 0;
    virtual GraphicsContext& GetContext() const = 0;
};
