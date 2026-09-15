#pragma once

struct GenerationalArrayHandle
{
  std::uint32_t idx = 0;
  std::uint32_t gen = 0;
};

class GenerationalArrayBase
{
public:
  virtual ~GenerationalArrayBase() = default;
  virtual const GenerationalArrayItem& GetRaw(GenerationalArrayHandle handle) = 0;
  virtual void Remove(GenerationalArrayHandle handle) = 0;
  virtual bool Has(GenerationalArrayHandle handle) = 0;
  virtual std::size_t Size() const = 0;
}

template<typename TObject>
class GenerationalArrayTyped
{
public:
  virtual ~GenerationalArrayTyped() = default;
  virtual const TObject& Get(GenerationalArrayHandle handle) = 0;
};

template<typename TObject, typename TParams>
class GenerationalArray : public GenerationalArrayTyped<TObject>
{
public:
  virtual void Add(const TParams& params) = 0;
};
