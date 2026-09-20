#pragma once

#include "Runtime/GraphicsStorage.h"

class ENGINE_API Application
{
public:
  Application() = default;
  virtual ~Application() = default;

  Application(const Application&) = delete;
  Application& operator=(const Application&) noexcept = delete;
  Application(Application&&) = delete;
  Application& operator=(Application&&) noexcept = delete;

  bool Run(std::string_view name = "Application", std::uint32_t w = 1270, std::uint32_t h = 720);

private:
  bool m_IsRunning = false;
  std::uint32_t m_Width = 0;
  std::uint32_t m_Height = 0;
  GraphicsStorage m_GraphicsStorage;
};
