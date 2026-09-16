#include "Runtime/GraphicsStorage.h"
#include "glad/gl.h" 

// @Pending: Check if OPENGL defined at comptime.
#include "OpenGLVertexBuffer.h"
#include "OpenGLShader.h"

// @Note: In order to keep shorter compilation times
// we'll compile the graphics objects cpp's in this
// translation unit.
#include "OpenGLVertexBuffer.cpp"
#include "OpenGLShader.cpp"

struct StorageArrays
{
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED) \
  StaticHandleArray<_DERIVED, 50> m_##_DERIVED##Array;
#include "Runtime/GraphicsStorageList.h"
#undef GRAPHICS_STORAGE_REGISTER
};
