#pragma once

struct GraphicsObjectHandle
{
  std::uint32_t idx = 0;
  std::uint32_t gen = 0;
};

template<typename TInterface> 
class IGraphicsObjectStorageBase
{
public:
  virtual ~IGraphicsObjectStorageBase() = default;
  virtual void Remove(GraphicsObjectHandle handle) = 0;
  virtual const TInterface& RequestInterface(GraphicsObjectHandle handle) const = 0;
};

template<typename TInterface, typename TDescriptor>
class IGraphicsObjectStorage : public IGraphicsObjectStorageBase<TInterface>
{
public:
  virtual ~IGraphicsObjectStorage() = default;
  virtual GraphicsObjectHandle Add(const TDescriptor& descriptor) = 0;
};

template<typename TInterface, typename TObject, typename TDescriptor, std::size_t MAX>
class GraphicsObjectStorage : public IGraphicsObjectStorage<TInterface, TDescriptor>
{
public:
  TObject data[MAX];
};
