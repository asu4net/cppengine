#include "Runtime/GraphicsStorage.h"
#include "glad/gl.h" 

// @Pending: Check if OPENGL defined at comptime.
#include "OpenGLVertexBuffer.h"
#include "OpenGLShader.h"

// @Note: This class will use some X macro magic in order
// have the right types (depending on the API we are using)
// for the function declarations and definitions. I know it is
// possible to achieve something similar to whis with templates
// but I just find this way simpler. Simpler, but not pretty, I'm
// sorry :(

// @Note: In order to keep shorter compilation times
// we'll compile the graphics objects cpp's in this
// translation unit.
#include "OpenGLVertexBuffer.cpp"
#include "OpenGLShader.cpp"

// Comptime Generated Storages.
struct StorageArrays
{
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS) \
  StaticHandleArray<_DERIVED, 50> m_##_DERIVED##Array; // @Pending: _SIZE to the X macro.
#include "GraphicsStorageList.h"
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

// Comptime Generated Emplace
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS)           \
template<>                                                            \
GraphicsHandle GraphicsStorage::Emplace<_BASE>(const _PARAMS& params) \
{                                                                     \
   return m_Arrays->m_##_DERIVED##Array.Emplace(params);              \
}
#include "GraphicsStorageList.h"
#undef GRAPHICS_STORAGE_REGISTER

// Comptime Generated Get
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS)           \
template<>                                                            \
_BASE* GraphicsStorage::Get(GraphicsHandle handle)                    \
{                                                                     \
   return m_Arrays->m_##_DERIVED##Array.Get(handle);                  \
}
#include "GraphicsStorageList.h"
#undef GRAPHICS_STORAGE_REGISTER

// Comptime Generated Remove
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS)           \
template<>                                                            \
void GraphicsStorage::Remove<_BASE>(GraphicsHandle handle)            \
{                                                                     \
   m_Arrays->m_##_DERIVED##Array.Remove(handle);                      \
}
#include "GraphicsStorageList.h"
#undef GRAPHICS_STORAGE_REGISTER

// Comptime Generated IsValid
#define GRAPHICS_STORAGE_REGISTER(_BASE, _DERIVED, _PARAMS)           \
template<>                                                            \
bool GraphicsStorage::IsValid<_BASE>(GraphicsHandle handle)           \
{                                                                     \
   return m_Arrays->m_##_DERIVED##Array.IsValid(handle);              \
}
#include "GraphicsStorageList.h"
#undef GRAPHICS_STORAGE_REGISTER
