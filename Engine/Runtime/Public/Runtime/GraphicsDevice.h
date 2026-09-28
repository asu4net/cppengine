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
};
