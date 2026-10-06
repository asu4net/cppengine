#pragma once

class D3D11BufferLayout final : public BufferLayout
{
  public:
    D3D11BufferLayout(const BufferLayoutParams& params);
    ~D3D11BufferLayout();

    D3D11BufferLayout(const D3D11BufferLayout&) = delete;
    D3D11BufferLayout& operator=(const D3D11BufferLayout&) noexcept = delete;
    D3D11BufferLayout(D3D11BufferLayout&&) = delete;
    D3D11BufferLayout& operator=(D3D11BufferLayout&&) noexcept = delete;

    std::uint32_t GetStride() const { m_Elements.GetStride() };

  private:
    ID3D11InputLayout* m_InputLayout = nullptr;
    BufferElementArray m_Elements;
};
