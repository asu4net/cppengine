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

  void Init();
  void Exit();

protected:
  virtual void OnInit();
  virtual void OnExit();

private:
  GraphicsStorage m_GraphicsStorage;
};
