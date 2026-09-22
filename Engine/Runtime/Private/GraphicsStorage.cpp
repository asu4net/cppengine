#include "Runtime/GraphicsStorage.h"
#include "Runtime/Application.h"

#ifdef ENGINE_OPENGL
#include "glad/glad.h" 
#elif  ENGINE_D3D11
#else
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#error "Missing Graphics API implementation."
#endif

#ifdef ENGINE_OPENGL
#include "OpenGL/OpenGLGraphicsInstance.h"
#include "OpenGL/OpenGLGraphicsDevice.h"
#include "OpenGL/OpenGLGraphicsContext.h"
#include "OpenGL/OpenGLVertexBuffer.h"
#include "OpenGL/OpenGLShader.h"
#elif  ENGINE_D3D11
#include "D3D11/D3D11GraphicsInstance.h"
#include "D3D11/D3D11GraphicsDevice.h"
#include "D3D11/D3D11GraphicsContext.h"
#include "D3D11/D3D11VertexBuffer.h"
#include "D3D11/D3D11Shader.h"
#else
#error "Missing Graphics API implementation."
#endif

// @Note: This class will use some X macro magic in order
// have the right types (depending on the API we are using)
// for the function declarations and definitions. I know it is
// possible to achieve something similar to whis with templates
// but I just find this waaaaaay simpler. Simpler, but not pretty, 
// I'm sorry :(

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

// @Note: In order to keep shorter compilation times
// we'll compile the graphics objects cpp's in this
// translation unit.
#ifdef ENGINE_OPENGL
#include "OpenGL/OpenGLGraphicsInstance.cpp"
#include "OpenGL/OpenGLGraphicsDevice.cpp"
#include "OpenGL/OpenGLGraphicsContext.cpp"
#include "OpenGL/OpenGLVertexBuffer.cpp"
#include "OpenGL/OpenGLShader.cpp"
#elif  ENGINE_D3D11
#include "D3D11/D3D11GraphicsInstance.cpp"
#include "D3D11/D3D11GraphicsDevice.cpp"
#include "D3D11/D3D11GraphicsContext.cpp"
#include "D3D11/D3D11VertexBuffer.cpp"
#include "D3D11/D3D11Shader.cpp"
#else
#error "Missing Graphics API implementation."
#endif
