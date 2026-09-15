#pragma once

// @Note: This file is included in the PCH header of the engine Runtime.
// 
// @Description: Handle-Based Static Array. It provides most of the benefits of using an
// array, an allocator and smart pointers without dealing with the complexity of those three
// concepts combined. It is my humble attempt to bring the Data Oriented mindset and the group
// element thinking to the Object Oriented world.
//
// - Contiguous storage: Iterating through this array is guaranteed to be fast.
//   Very convenient for hot paths in your code!
// - Safe access: Keeping a handle instead of an index or a pointer prevents you for
//   both accessing a removed object or a dangling pointer!
// - Almost zero-cost access: The equivalent of accessing two array elements by index.
// - Zero dynamic allocations: It holds a memory block on the stack, does not grow and reuses empty slots.
// - Stable pointers: If you have full ownership of the element (does not get removed from other
//   places) you can keep a pointer to it. Even if new elements get appended/removed!
// - RAII: Append/Remove operations call the constructor/destructor of the Object by using
//   placement new. So you can keep using those if that's your thing.
//
// Downsides:
// - It can't grow.
// - It consumes more memory since needs to store 8 bytes extra per element.
// - It is not nice to debug, since the data lives in a byte array.
//   (You'll need to cast it to the proper type in the watch window or create a natvis).

#include <cstdint>
#include <cstddef>
#include <new>
#include <utility>

// Identifies an element inside a handle array.
struct ArrayHandle
{
  std::uint32_t Index = 0u;
  std::uint32_t Generation = 0u;

  bool operator==(const ArrayHandle& other)
  {
    return Index == other.Index && Generation == other.Generation;
  }

  bool operator!=(const ArrayHandle& other)
  {
    return !(*this == other);
  }
};

// The handle Array.
template<typename T, std::uint32_t Cap>
class StaticHandleArray
{
public:
  // @Note: It is Cap + 1 because we follow ZII (Zero Is Initialization),
  // so the first element will act as a dummy object.
  static constexpr std::uint32_t Capacity = Cap + 1u;

  struct ElemInfo
  {
    std::uint32_t Generation = 0u;
    bool Occupied = false;
  };

  // @Note: We just iterate the occupied slots of the array.
  class Iterator
  {
  public:
    Iterator(StaticHandleArray* array, std::uint32_t index)
      : m_Array(array)
      , m_Index(index)
    {
      AdvanceToNextValid();
    }

    T& operator*()
    {
      return m_Array->GetUnchecked(m_Index);
    }

    T* operator->()
    {
      return &m_Array->GetUnchecked(m_Index);
    }

    Iterator& operator++()
    {
      ++m_Index;
      AdvanceToNextValid();
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const Iterator& other) const
    {
      return m_Index == other.m_Index;
    }

    bool operator!=(const Iterator& other) const
    {
      return m_Index != other.m_Index;
    }

    ArrayHandle Handle() const
    {
      return { m_Index, m_Array->m_Info[m_Index].Generation };
    }

  private:
    void AdvanceToNextValid()
    {
      while (m_Index < Capacity && !m_Array->m_Info[m_Index].Occupied)
      {
        LOG_INFO("{}", m_Index);
        ++m_Index;
      }
    }

    StaticHandleArray* m_Array;
    std::uint32_t m_Index;
  };

  class ConstIterator
  {
  public:
    ConstIterator(const StaticHandleArray* array, std::uint32_t index)
      : m_Array(array)
      , m_Index(index)
    {
      AdvanceToNextValid();
    }

    const T& operator*() const
    {
      return m_Array->GetUnchecked(m_Index);
    }

    const T* operator->() const
    {
      return &m_Array->GetUnchecked(m_Index);
    }

    ConstIterator& operator++()
    {
      ++m_Index;
      AdvanceToNextValid();
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const ConstIterator& other) const
    {
      return m_Index == other.m_Index;
    }

    bool operator!=(const ConstIterator& other) const
    {
      return m_Index != other.m_Index;
    }

    ArrayHandle Handle() const
    {
      return { m_Index, m_Array->m_Info[m_Index].Generation };
    }

  private:
    void AdvanceToNextValid()
    {
      while (m_Index < Capacity && !m_Array->m_Info[m_Index].Occupied)
      {
        ++m_Index;
      }
    }

    const StaticHandleArray* m_Array;
    std::uint32_t m_Index;
  };

  StaticHandleArray()
  {
    for (std::uint32_t i = 1u; i < Capacity; ++i)
    {
      m_FreeList[i - 1u] = i;
    }
    m_FreeListSize = Cap;
  }

  ~StaticHandleArray()
  {
    Clear();
  }

  // @Note: Storage is now raw std::byte + placement-new, so a naive
  // copy/move of this class would memcpy live objects and double-run
  // their destructors. Deleted on purpose; write an explicit copy/move
  // (looping Append/Emplace per occupied slot) if you ever need one.
  StaticHandleArray(const StaticHandleArray&) = delete;
  StaticHandleArray& operator=(const StaticHandleArray&) = delete;
  StaticHandleArray(StaticHandleArray&&) = delete;
  StaticHandleArray& operator=(StaticHandleArray&&) = delete;

  // @Note: Constructs T in-place inside the slot. This is the primitive
  // both Append overloads are built on; use it directly for move-only
  // or non-default-constructible T (e.g. RAII wrappers over GL/DX11
  // handles that acquire the resource in their constructor).
  template<typename... Args>
  ArrayHandle Emplace(Args&&... args)
  {
    if (m_FreeListSize == 0u)
    {
      return { 0u, 0u }; // Full, return the dummy element handle.
    }

    --m_FreeListSize;
    std::uint32_t index = m_FreeList[m_FreeListSize];

    ::new (static_cast<void*>(&m_Data[index * sizeof(T)])) T(std::forward<Args>(args)...);
    m_Info[index].Occupied = true;
    ++m_Count;

    return { index, m_Info[index].Generation };
  }

  ArrayHandle Append(const T& element)
  {
    return Emplace(element);
  }

  ArrayHandle Append(T&& element)
  {
    return Emplace(std::move(element));
  }

  bool Remove(ArrayHandle handle)
  {
    if (handle.Index == 0u || handle.Index >= Capacity)
    {
      return false;
    }

    ElemInfo& info = m_Info[handle.Index];
    if (!info.Occupied || info.Generation != handle.Generation)
    {
      return false;
    }

    DestroyAt(handle.Index);
    info.Occupied = false;
    ++info.Generation;
    --m_Count;

    m_FreeList[m_FreeListSize] = handle.Index;
    ++m_FreeListSize;

    return true;
  }

  T* Get(ArrayHandle handle)
  {
    if (handle.Index == 0u || handle.Index >= Capacity)
    {
      return nullptr;
    }

    ElemInfo& info = m_Info[handle.Index];
    if (!info.Occupied || info.Generation != handle.Generation)
    {
      return nullptr;
    }

    return &GetUnchecked(handle.Index);
  }

  const T* Get(ArrayHandle handle) const
  {
    if (handle.Index == 0u || handle.Index >= Capacity)
    {
      return nullptr;
    }

    const ElemInfo& info = m_Info[handle.Index];
    if (!info.Occupied || info.Generation != handle.Generation)
    {
      return nullptr;
    }

    return &GetUnchecked(handle.Index);
  }

  bool IsValid(ArrayHandle handle) const
  {
    if (handle.Index == 0u || handle.Index >= Capacity)
    {
      return false;
    }

    const ElemInfo& info = m_Info[handle.Index];
    return info.Occupied && info.Generation == handle.Generation;
  }

  // Vacía el array llamando al destructor de cada elemento ocupado
  // e invalidando sus handles (incrementa la generación).
  void Clear()
  {
    for (std::uint32_t i = 1u; i < Capacity; ++i)
    {
      if (m_Info[i].Occupied)
      {
        DestroyAt(i);
        m_Info[i].Occupied = false;
        ++m_Info[i].Generation;
      }
    }

    for (std::uint32_t i = 1u; i < Capacity; ++i)
    {
      m_FreeList[i - 1u] = i;
    }
    m_FreeListSize = Cap;
    m_Count = 0u;
  }

  std::uint32_t Count() const 
  { 
    return m_Count; 
  }

  bool IsFull()  const 
  { 
    return m_FreeListSize == 0u; 
  }

  bool IsEmpty() const 
  { 
    return m_Count == 0u; 
  }

  Iterator begin() 
  { 
    return Iterator(this, 1u); 
  }

  Iterator end() 
  { 
    return Iterator(this, Capacity); 
  }

  ConstIterator begin() const 
  { 
    return ConstIterator(this, 1u); 
  }

  ConstIterator end() const 
  { 
    return ConstIterator(this, Capacity); 
  }

private:
  T& GetUnchecked(std::uint32_t index)
  {
    // @Note: std::launder is required because placement-new re-creates
    // an object in storage that the compiler otherwise still considers
    // to hold whatever was there before (or nothing at all).
    return *std::launder(reinterpret_cast<T*>(&m_Data[index * sizeof(T)]));
  }

  const T& GetUnchecked(std::uint32_t index) const
  {
    return *std::launder(reinterpret_cast<const T*>(&m_Data[index * sizeof(T)]));
  }

  void DestroyAt(std::uint32_t index)
  {
    GetUnchecked(index).~T();
  }

  alignas(T) std::byte m_Data[sizeof(T) * Capacity];
  ElemInfo m_Info[Capacity]{};
  std::uint32_t m_FreeList[Cap]{};
  std::uint32_t m_FreeListSize = 0u;
  std::uint32_t m_Count = 0u;
};
