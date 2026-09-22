#pragma once

struct GraphicsDeviceParams
{
  void* windowHandle = nullptr;
};

class GraphicsDevice
{
  public:
    virtual ~GraphicsDevice() = default;
};
