#pragma once

#pragma once

struct SwapChainParams
{
};

class SwapChain
{
  public:
    virtual ~SwapChain() = default;

    virtual void Present(bool vsync = false) = 0;
};
