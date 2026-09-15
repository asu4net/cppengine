#pragma once

#include "GraphicsObject.h"

struct VertexBufferDescriptor
{
};

class IVertexBuffer : public GraphicsObject<VertexBufferDescriptor>
{
  public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void* data, std::uint32_t size) = 0;

    static std::shared_ptr<VertexBuffer> Create(std::uint32_t size);
    static std::shared_ptr<VertexBuffer> Create(float* vertices, std::uint32_t size);
};
