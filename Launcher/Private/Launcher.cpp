#include "Runtime/Application.h"

int main(int argc, char** argv)
{
  Application* app = new Application();
  app->Run();
  // @Note: We don't really need to call delete on app
  // the OS takes resposability of freeing all
  // the memory for us. But for debugging purposes
  // we'll keep it for now.
  delete app;
  app = nullptr;
}
