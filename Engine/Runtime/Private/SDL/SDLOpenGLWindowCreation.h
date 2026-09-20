#pragma once

namespace SDLOpenGL
{
  bool CreateWindowAndContext(std::string_view name, std::uint32_t w, std::uint32_t h);
  void DestroyContextAndWindow();
  void SwapBuffers(bool vsync = false);
}
