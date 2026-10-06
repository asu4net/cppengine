#pragma once

// Helper Classes ----------------------------------------------------------------------
struct BufferElementView
{
  const char* Name; // @Pending: Change this to a std::string_view.
  ShaderDataType Type;
};

struct BufferElement
{
  std::string Name;
  ShaderDataType Type;
  std::uint32_t Size;
  std::size_t Offset;

  BufferElement() = default;

  BufferElement(ShaderDataType type, const char* name, bool normalized = false)
    : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0)
  {
  }
};

class BufferElementArray
{
public:
  BufferElementArray() {}

  BufferElementArray(std::span<const BufferElementView> elementsView)
    : m_Elements(elementsView.size())
  {
    for (std::size_t i = 0; i < elementsView.size(); ++i)
    {
      const auto& elemView = elementsView[i];
      auto& elem = m_Elements[i];
      elem = BufferElement(elemView.Type, elemView.Name);
    }
    CalculateOffsetsAndStride();
  }

  std::uint32_t GetStride() const { return m_Stride; }
  const std::vector<BufferElement>& GetElements() const { return m_Elements; }

  std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
  std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
  std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
  std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
  void CalculateOffsetsAndStride()
  {
    std::size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements)
    {
      element.Offset = offset;
      offset += element.Size;
      m_Stride += element.Size;
    }
  }
private:
  // @Pending: Unnecessary allocation here. Change to std::array with a max size.
  std::vector<BufferElement> m_Elements;
  std::uint32_t m_Stride = 0;
};
// -------------------------------------------------------------------------------------

struct BufferLayoutParams
{
  std::span<const BufferElementView> BufferElementsView;
  GraphicsHandle ShaderHandle;
  GraphicsHandle DeviceHandle;
};

class BufferLayout
{
  public:
    virtual ~BufferLayout() = default;
    virtual std::uint32_t GetStride() const = 0;
};
