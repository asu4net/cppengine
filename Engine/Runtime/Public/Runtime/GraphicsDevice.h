#pragma once

struct GraphicsState
{
  GraphicsHandle vertexShader;
  GraphicsHandle pixelShader;
  GraphicsHandle bufferLayout;
  GraphicsHandle vertexBuffer;
  std::size_t sizeOfVertices = 0;
};

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
    virtual void SetGraphicsState(const GraphicsState& state) = 0;
    virtual void ImmediateDraw() = 0;
};
