#include "D3D11GraphicsInstance.h"

D3D11GraphicsInstance::D3D11GraphicsInstance(const GraphicsInstanceParams& params)
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();

  // Alloc the device.
  GraphicsDeviceParams graphicsDeviceParams{ params.windowHandle };
  m_DeviceHandle = graphicsStorage.Emplace<GraphicsDevice>(graphicsDeviceParams);
  auto* deviceInterface = graphicsStorage.Get<GraphicsDevice>(m_DeviceHandle);
  m_Device = static_cast<D3D11GraphicsDevice*>(deviceInterface);

  // Alloc the context.
  GraphicsContextParams graphicsContextParams{ params.windowHandle };
  m_ContextHandle = graphicsStorage.Emplace<GraphicsContext>(graphicsContextParams);
  auto* contextInterface = graphicsStorage.Get<GraphicsContext>(m_ContextHandle);
  m_Context = static_cast<D3D11GraphicsContext*>(contextInterface);

  // Create the d3d11 device, device context and swap chain.
  ASSERT(params.windowHandle != nullptr);
  HWND window = reinterpret_cast<HWND>(params.windowHandle);

  // Alloc the Swap Chain.
  SwapChainParams swapChainParams;
  GraphicsHandle swapChainHandle = graphicsStorage.Emplace<SwapChain>(swapChainParams);
  auto* swapChainInterface = graphicsStorage.Get<SwapChain>(swapChainHandle);

  // Give the Device ownership over the swap chain.
  auto* d3d11SwapChain = static_cast<D3D11SwapChain*>(swapChainInterface);
  m_Device->SetSwapChain(swapChainHandle, d3d11SwapChain);

  // Swap chain settings.
  DXGI_SWAP_CHAIN_DESC desc = {};

  // Will use the window size.
  desc.BufferDesc.Width = 0;                           
  desc.BufferDesc.Height = 0;                          

  // Color buffer format (8 bits RGBA).
  desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  // Will use the monitor refresh rate.
  desc.BufferDesc.RefreshRate.Numerator = 0;
  desc.BufferDesc.RefreshRate.Denominator = 0;

  // Will use the window size.
  desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  // Will use the preferred for the connected display.
  desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

  // Antialiasing. For now no-antialasing.
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;

  // Use use the buffer as the render target.
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

  // One front buffer, one back buffer. (Double Buffering).
  desc.BufferCount = 1; // @Note: Kinda confusing.

  // Window stuff here.
  desc.OutputWindow = window;
  desc.Windowed = TRUE;

  // Throw away the back buffer contents after the presentation. Maximizes performance.
  desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 

  // For now we don't specify any flag here.
  desc.Flags = 0;

  // Create the device, the device context, and the swap chain.
  HRESULT result = D3D11CreateDeviceAndSwapChain(
    nullptr,                                // pAdapter: (nullptr -> choose the default adapter)
    D3D_DRIVER_TYPE_HARDWARE,               // DriverType
    nullptr,                                // Software: This gives a handle to load a sofware driver.
    0,                                      // Flags: @Pending
    nullptr,                                // pFeatureLevels
    0,                                      // FeatureLevels
    D3D11_SDK_VERSION,                      // With this macro it uses this system SDK version.
    &desc,                                  // Descriptor for the swap chain.
    d3d11SwapChain->GetPointer(),           // Handle to the swap chain.
    m_Device->GetPointer(),                 // Handle to the device.
    nullptr,                                // pFeatureLevel
    m_Context->GetPointer()                 // Handle to the device context.
  );

  if (FAILED(result))
  {
    LOG_ERR("D3D11 Error creating the device and the swap chain: 0x{:08X}.", static_cast<unsigned>(result));
    std::exit(EXIT_FAILURE);
  }

  // Gain access to the render target, and give the full ownership of it
  // to the swap chain.
  
  // Get the swap chain
  auto** d3d11SwapChainObjectHandle = d3d11SwapChain->GetPointer();
  auto* d3d11SwapChainObject = d3d11SwapChainObjectHandle != nullptr ? *d3d11SwapChainObjectHandle : nullptr;
  ASSERT(d3d11SwapChainObject != nullptr);
  if (d3d11SwapChainObject == nullptr)
  {
    LOG_ERR("D3D11 unhandled error retrieving the d3d11 swap chain.");
    std::exit(EXIT_FAILURE);
  }

  // Get the back buffer
  ID3D11Resource* backBuffer = nullptr;
  d3d11SwapChainObject->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&backBuffer));
  ASSERT(backBuffer != nullptr);
  if (backBuffer == nullptr)
  {
    LOG_ERR("D3D11 Error retrieving the back buffer.");
    std::exit(EXIT_FAILURE);
  }

  // Create the render target view, filling the pointer in the swap chain.
  auto* d3d11Device = *m_Device->GetPointer();
  d3d11Device->CreateRenderTargetView(backBuffer, nullptr, d3d11SwapChain->GetRenderTargetViewPointer());
  
  // We don't need the back buffer anymore.
  backBuffer->Release();
  backBuffer = nullptr;

  LOG_INFO("D3D11 Graphics instance created!");
}

D3D11GraphicsInstance::~D3D11GraphicsInstance()
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
  graphicsStorage.Remove<GraphicsContext>(m_ContextHandle);
  graphicsStorage.Remove<GraphicsDevice>(m_DeviceHandle);
  LOG_INFO("D3D11 Graphics instance destroyed!");
}

GraphicsDevice& D3D11GraphicsInstance::GetDevice()
{
  ASSERT(m_Device != nullptr);
  return *m_Device;
}

GraphicsContext& D3D11GraphicsInstance::GetContext()
{
  ASSERT(m_Context != nullptr);
  return *m_Context;
}

