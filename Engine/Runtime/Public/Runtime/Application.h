#pragma once

#include "Runtime/Base.h"

class ENGINE_API Application
{
  public:
    Application() = default;
    virtual ~Application() {}

    void Init();
    void Exit();

  protected:
    virtual void OnInit();
    virtual void OnExit();

  private:
    void InitGraphics();
};
