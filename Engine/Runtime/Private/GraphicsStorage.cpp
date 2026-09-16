#include "Runtime/GraphicsStorage.h"
#include "glad/gl.h" 

// @Note: This class will use some X macro magic in order
// have the right types (depending on the API we are using)
// for the function declarations and definitions. This is not
// going to be pretty. :(

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
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS) \
  StaticHandleArray<_DERIVED, 50> m_##_DERIVED##Array; // @Pending: _SIZE to the X macro.
#include "Runtime/GraphicsStorageList.h"
#undef GRAPHICS_STORAGE_REGISTER
};

GraphicsStorage::GraphicsStorage()
  : m_Arrays(new StorageArrays()) // @Note: Just one big allocation for the graphic objects.
{
}

GraphicsStorage::~GraphicsStorage()
{
  ASSERT(m_Arrays != nullptr);
  delete m_Arrays;
  m_Arrays = nullptr;
}

#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS)           \
template<>                                                            \
GraphicsHandle GraphicsStorage::Emplace<_BASE>(const _PARAMS& params) \
{                                                                     \
   return m_Arrays->m_##_DERIVED##Array.Emplace(params);              \
}

#include "Runtime/GraphicsStorageList.h"

#undef GRAPHICS_STORAGE_REGISTER
