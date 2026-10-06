#pragma once

#include "Runtime/ShaderDataType.h"

struct VertexBufferParams
{
  const void* vertices = nullptr;
  std::size_t vertSize = 0;
  std::size_t vertCount = 0;
  GraphicsHandle deviceHandle;
};

class VertexBuffer
{
  public:
    virtual ~VertexBuffer() = default;
};
