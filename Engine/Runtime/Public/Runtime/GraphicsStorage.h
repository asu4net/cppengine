#pragma once

#include "Runtime/ShaderDataType.h"
#include "Runtime/VertexBuffer.h"
#include "Runtime/Shader.h"

struct StorageArrays;

typedef ArrayHandle GraphicsHandle;

class GraphicsStorage
{
public:
  GraphicsStorage();
  ~GraphicsStorage();

  // @Note: There is no need to move around an object of this class.
  // The rule of six, maybe nowadays is the rule of eight, just god
  // knows.
  GraphicsStorage(const GraphicsStorage&) = delete;
  GraphicsStorage& operator=(const GraphicsStorage&) noexcept = delete;
  GraphicsStorage(GraphicsStorage&&) = delete;
  GraphicsStorage& operator=(GraphicsStorage&&) noexcept = delete;

  template<typename T, typename TParams>
  GraphicsHandle Emplace(const TParams& params);

private:
  StorageArrays* m_Arrays = nullptr;
};
