#pragma once

namespace SDL_D3D11
{
  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h);
  void DeinitWindow();
  void* GetHandle();
}
