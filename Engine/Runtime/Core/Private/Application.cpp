#include "Runtime/Application.h"

void Application::Init()
{
  OnInit();
};

void Application::Exit()
{
  OnExit();
};

void Application::OnInit()
{
  std::print("Application init! \n");
}

void Application::OnExit()
{
  std::print("Application exit! \n");
}
