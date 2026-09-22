#pragma once

namespace SDLOpenGL
{
  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h);
  void DeinitWindow();
  void* GetHandle();
  void SwapBuffers(bool vsync = false);
}
