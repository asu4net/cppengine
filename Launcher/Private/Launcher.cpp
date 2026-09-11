#include "Runtime/CoreMinimal.h"

int main(int argc, char** argv)
{
  Application* app = new Application();
  app->Init();
  app->Exit();
  // @Note: We don't need to call delete on app
  // the OS takes resposability of freeing all
  // the memory for us.
}
