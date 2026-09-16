#pragma once

#include "Runtime/ShaderDataType.h"

struct VertexBufferParams
{
};

class VertexBuffer
{
  public:
    virtual ~VertexBuffer() = default;

    virtual void Use() = 0;
    virtual void SetData(const void* data, std::uint32_t size) = 0;
};
