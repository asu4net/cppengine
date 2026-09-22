#pragma once

class D3D11GraphicsInstance
{
  public:
    D3D11GraphicsInstance(HWND windowHandle);
    ~D3D11GraphicsInstance();

    D3D11GraphicsInstance(const D3D11GraphicsInstance&);
    D3D11GraphicsInstance& operator(const D3D11GraphicsInstance&) noexcept;
    D3D11GraphicsInstance(D3D11GraphicsInstance&&);
    D3D11GraphicsInstance& operator(D3D11GraphicsInstance&&) noexcept;

    GraphicsDevice& GetDevice() const override;
    GraphicsContext& GetContext() const override;
}
