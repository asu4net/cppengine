#pragma once

namespace SDL_OpenGL
{
  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h);
  void DeinitWindow();
  void* GetHandle();
}
