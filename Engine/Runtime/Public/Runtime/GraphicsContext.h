#pragma once

struct GraphicsContextParams
{
  void* windowHandle = nullptr;
};

class SwapChain;

class GraphicsContext
{
  public:
    virtual ~GraphicsContext() = default;

    // @Review: I guess this would be a command in the future.
    virtual void ClearBackBuffer(SwapChain& swapChain, float r = 0, float g = 0, float b = 0) = 0;
};

