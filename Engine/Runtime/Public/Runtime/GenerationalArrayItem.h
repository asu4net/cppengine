#pragma once

// @Pending:
// - Make TParams an optional parameter for the
//   generational array item.

// @Warning:
// This is the base class from the GenerationalArrayItem
// with most of the related features. In order to make your
// type usable by the any generational array you MUST inherit
// from GenerationalArrayItem, not this base type.
class GenerationalArrayItemBase
{
public:
  virtual ~GenerationalArrayItemBase() = default;

  virtual bool OnAdded(const TParams& descriptor) 
  {
    return true;
  };

  virtual void OnRemoved() 
  {
  };

  virtual bool IsValid()
  {
    return m_idx > 0 && m_gen > 0;
  }

  bool Setup(std::uint32_t idx, std::uint32_t gen)
  {
    if (IsValid())
    {
      LOG_ERR("Calling Setup but idx/gen still valid.");
      return false;
    }
    m_idx = idx;
    m_gen = gen;
    return true;
  }

  void Reset()
  {
    ASSERT(IsValid());
    m_idx = 0;
    m_gen = 0;
  }

  std::uint32_t GetIndex() const { return m_idx; }
  std::uint32_t GetGeneration() const { return m_gen; }

private:
  std::uint32_t m_idx = 0;
  std::uint32_t m_gen = 0;
};

// @Note:
// We'll keep all the features but the Add callback 
// in the base class, in order to have an interface
// with the class that do not requires the TParams
// template parameter (type erasure).

template<typename TParams>
class GenerationalArrayItem : GenerationalArrayItemBase
{
public:
  virtual ~GenerationalArrayItem() = default;

  virtual void OnAdded(const TParams& descriptor) 
  {
  };
};
