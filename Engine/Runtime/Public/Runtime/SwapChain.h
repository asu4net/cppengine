#pragma once

#pragma once

struct SwapChainParams
{
  void* nativeDevice = nullptr;
  void* nativeSwapChain = nullptr;
};

class SwapChain
{
  public:
    virtual ~SwapChain() = default;

    virtual void Present(bool vsync = false) = 0;
};
