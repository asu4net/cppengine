#include "Runtime/Application.h"
#include "Runtime/GraphicsInstance.h"
#include "Runtime/GraphicsDevice.h"
#include "Runtime/SwapChain.h"

// @Note: For know we keep here both files
// since we are not going to expose the 
// window creation API to the user.
// This may change in the future.
#include "WindowCreation.h"
#include "WindowCreation.cpp"
#include "Input.h"
#include "Input.cpp"

// @Pending: REMOVE LATER
#if defined(CONFIG_DEBUG) && defined(ENGINE_D3D11)
#include "D3D11/D3D11GraphicsInstance.h"
#endif

static Application* s_CurrentApplication = nullptr;

Application& Application::GetInstance()
{
  ASSERT(s_CurrentApplication != nullptr);
  return *s_CurrentApplication;
}

Application::Application()
{
  if (s_CurrentApplication == nullptr)
  {
    s_CurrentApplication = this;
    return;
  }

  LOG_ERR("Trying to create multiple application instances!");
  std::exit(EXIT_FAILURE);
}

bool Application::Run(std::string_view name, std::uint32_t w, std::uint32_t h)
{
  // Create the window.
  bool windowCreated = WindowCreation::InitWindow(name, w, h);
  if (!windowCreated)
  {
    LOG_ERR("Couldn't create the window!");
    return false;
  }

  // Create the graphics instance and get a pointer.
  GraphicsInstanceParams graphicsInstanceParams{ WindowCreation::GetHandle() };
  m_GraphicsInstanceHandle = m_GraphicsStorage.Emplace<GraphicsInstance>(graphicsInstanceParams);
  m_GraphicsInstance = m_GraphicsStorage.Get<GraphicsInstance>(m_GraphicsInstanceHandle);

  if (m_GraphicsInstance == nullptr)
  {
    LOG_ERR("Couldn't create the graphics instance!");
    return false;
  }

  // Run the main loop.
  m_IsRunning = true;
  while (m_IsRunning)
  {
    m_IsRunning = !Input::ShouldCloseWindow();

    // Clear the back buffer.
    m_GraphicsInstance->GetDevice().ClearBackBuffer(0.1f, 0.2f, 0.3f);

    // @Pending: Draw frame.
#if defined(CONFIG_DEBUG) && defined(ENGINE_D3D11)
    auto* instance = static_cast<D3D11GraphicsInstance*>(m_GraphicsInstance);
    instance->DrawTestTriangle();
#endif
    
    // @Pending: Specify vsync on/off.
    m_GraphicsInstance->GetDevice().GetSwapChain().Present(/*vsync*/ true);

#ifdef CONFIG_DEBUG
    m_GraphicsInstance->GetDevice().DumpDebugMessages();
#endif
  }
  
// @Note: Deinitialization should be done just for debugging purposes.
// The OS should be in charge to free all the memory, not us.
#ifdef CONFIG_DEBUG
  // Destroy window and graphics.
  m_GraphicsInstance = nullptr;
  m_GraphicsStorage.Remove<GraphicsInstance>(m_GraphicsInstanceHandle);
  WindowCreation::DeinitWindow();
#endif
  return true;
};

GraphicsInstance& Application::GetGraphicsInstance() const
{
  ASSERT(m_GraphicsInstance != nullptr);
  return *m_GraphicsInstance;
}

GraphicsStorage& Application::GetGraphicsStorage()
{
  return m_GraphicsStorage;
}
