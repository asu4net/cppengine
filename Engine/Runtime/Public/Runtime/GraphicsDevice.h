#pragma once

struct GraphicsDeviceParams
{
  void* windowHandle = nullptr;
};

class SwapChain;

class GraphicsDevice
{
  public:
    virtual ~GraphicsDevice() = default;
    virtual SwapChain& GetSwapChain() = 0;
    virtual void DumpDebugMessages() = 0;
    virtual void ClearBackBuffer(float r = 0, float g = 0, float b = 0) = 0;
};
