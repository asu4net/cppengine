#pragma once

struct GraphicsContextParams
{
  void* windowHandle = nullptr;
};

class GraphicsContext
{
  public:
    virtual ~GraphicsContext() = default;
};

