#pragma once

namespace WindowCreation
{
  bool InitWindow(std::string_view name, std::uint32_t w, std::uint32_t h);
  void DeinitWindow();
  void* GetHandle();
  // @Review: This should be somewhere else, maybe.
  void Present(bool vsync = false);
}
