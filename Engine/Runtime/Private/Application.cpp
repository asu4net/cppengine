#include "Runtime/Application.h"

// Test code for graphis storage.
#include "Runtime/VertexBuffer.h"
#include "Runtime/Shader.h"

// @Note: For know we keep here both files
// since we are not going to expose the 
// window creation API to the user.
// This may change in the future.
#include "WindowCreation.h"
#include "WindowCreation.cpp"
#include "Input.h"
#include "Input.cpp"

bool Application::Run(std::string_view name, std::uint32_t w, std::uint32_t h)
{
  WindowCreation::InitWindow(name, w, h);
  m_IsRunning = true;

  while (m_IsRunning)
  {
    m_IsRunning = !Input::ShouldCloseWindow();

    // @Review: This will go somewhere else once we do the D3D11 implementation.
#ifdef ENGINE_OPENGL
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
#endif
    WindowCreation::Present();
  }
  
  WindowCreation::DeinitWindow();
  return true;
};
