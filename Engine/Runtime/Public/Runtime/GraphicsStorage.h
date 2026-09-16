#pragma once

struct StorageArrays;

// @Review: We could achieve something similar to the 
// concept of distinct types, that already exists in 
// some modern C-like languages, using struct wrappers. 
// Leaving it in this way, we take the risk that the caller uses
// a Shader GraphicsHandle, access a VertexBuffer (for example).
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

  // @Review: We technically wouldn't need to Specify T here.
  // but for more explicitness I'm going to leave it for now.
  template<typename T, typename TParams>
  GraphicsHandle Emplace(const TParams& params);

  template<typename T>
  T* Get(GraphicsHandle handle);

  template<typename T>
  void Remove(GraphicsHandle handle);

  template<typename T>
  bool IsValid(GraphicsHandle handle);

private:
  StorageArrays* m_Arrays = nullptr;
};
