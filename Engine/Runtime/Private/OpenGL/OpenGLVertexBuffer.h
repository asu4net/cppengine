#pragma once

#include "Runtime/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer
{
  public: 
    OpenGLVertexBuffer(const VertexBufferParams& params);
    ~OpenGLVertexBuffer();
};
