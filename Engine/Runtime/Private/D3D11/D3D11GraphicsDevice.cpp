#include "D3D11GraphicsDevice.h"

D3D11GraphicsDevice::D3D11GraphicsDevice(const GraphicsDeviceParams& params)
{
  ASSERT(params.windowHandle != nullptr);
  if (params.windowHandle == nullptr)
  {
    LOG_ERR("D3D11 Error: Invalid device handle provided to SwapChain.");
    std::exit(EXIT_FAILURE);
  }

  // Trust the windowHandle is an HWND.
  HWND window = reinterpret_cast<HWND>(params.windowHandle);

  // Create the d3d11 device, device context and swap chain.
  IDXGISwapChain* nativeSwapChain = nullptr;

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

  std::uint32_t flags = 0;
#ifdef CONFIG_DEBUG
  flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  // Create the device, the device context, and the swap chain.
  HRESULT result = D3D11CreateDeviceAndSwapChain(
    nullptr,                   // pAdapter: (nullptr -> choose the default adapter)
    D3D_DRIVER_TYPE_HARDWARE,  // DriverType
    nullptr,                   // Software: This gives a handle to load a sofware driver.
    flags,                     // Flags: For now we just use them for enabling the debug layer.
    nullptr,                   // pFeatureLevels
    0,                         // FeatureLevels
    D3D11_SDK_VERSION,         // With this macro it uses this system SDK version.
    &desc,                     // Descriptor for the swap chain.
    &nativeSwapChain,          // Handle to the swap chain.
    &m_Device,                 // Handle to the device.
    nullptr,                   // pFeatureLevel
    &m_Context                 // Handle to the device context.
  );

  LOG_INFO( "D3D11_CREATE_DEVICE_DEBUG: {}", (flags & D3D11_CREATE_DEVICE_DEBUG) != 0);

  if (FAILED(result))
  {
    LOG_ERR("D3D11 Error creating the device and the swap chain: 0x{:08X}.", static_cast<unsigned>(result));
    std::exit(EXIT_FAILURE);
  }

  // Alloc the Swap Chain.
  SwapChainParams swapChainParams;
  swapChainParams.nativeSwapChain = nativeSwapChain;
  swapChainParams.nativeDevice = m_Device;
  m_SwapChainHandle = Application::GetInstance().GetGraphicsStorage().Emplace<SwapChain>(swapChainParams);
  m_SwapChain = static_cast<D3D11SwapChain*>(Application::GetInstance().GetGraphicsStorage().Get<SwapChain>(m_SwapChainHandle));

  // Config the debug layer.
#ifdef CONFIG_DEBUG

  ID3D11Debug* debug = nullptr;

  HRESULT hr = m_Device->QueryInterface(
    __uuidof(ID3D11Debug),
    reinterpret_cast<void**>(&debug)
  );

  LOG_INFO("ID3D11 Debug layer available: {}", SUCCEEDED(hr));

  if (SUCCEEDED(hr))
  {
    debug->Release();
  }

  ID3D11InfoQueue* infoQueue = nullptr;

  result = m_Device->QueryInterface(
    __uuidof(ID3D11InfoQueue),
    reinterpret_cast<void**>(&infoQueue)
  );

  if (SUCCEEDED(result))
  {
    LOG_INFO("D3D11 Enabled info queue.");
    infoQueue->SetBreakOnSeverity(
      D3D11_MESSAGE_SEVERITY_CORRUPTION,
      FALSE
    );

    infoQueue->SetBreakOnSeverity(
      D3D11_MESSAGE_SEVERITY_ERROR,
      FALSE
    );

    infoQueue->Release();
  }
#endif

  LOG_INFO("D3D11 Device created!");
}

D3D11GraphicsDevice::~D3D11GraphicsDevice()
{
  ASSERT(m_SwapChain != nullptr);
  if (m_SwapChain != nullptr)
  {
    auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
    graphicsStorage.Remove<SwapChain>(m_SwapChainHandle);
    m_SwapChainHandle = {};
    m_SwapChain = nullptr;
  }

  ASSERT(m_Context != nullptr);
  if (m_Context != nullptr)
  {
    m_Context->Release();
    m_Context = nullptr;
  }


  ASSERT(m_Device != nullptr);
  if (m_Device != nullptr)
  {
    m_Device->Release();
    m_Device = nullptr;
  }

  LOG_INFO("D3D11 Device destroyed!");
}

SwapChain& D3D11GraphicsDevice::GetSwapChain()
{
  ASSERT(m_SwapChain != nullptr);
  return *m_SwapChain;
}

void D3D11GraphicsDevice::DumpDebugMessages()
{
#ifdef CONFIG_DEBUG
  if (m_Device == nullptr)
  {
    //LOG_ERR("D3D11 Invalid device when calling DumpDebugMessages()!");
    return;
  }

  ID3D11InfoQueue* infoQueue = nullptr;
  HRESULT result = m_Device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));

  if (FAILED(result))
  {
    //LOG_INFO("D3D11 Couldn't get info queue.");
    return;
  }

  std::uint32_t messageCount = infoQueue->GetNumStoredMessages();
  //LOG_INFO( "D3D11 Stored messages: {}", infoQueue->GetNumStoredMessages());
  //LOG_INFO( "D3D11 Allowed messages: {}", infoQueue->GetNumStoredMessagesAllowedByRetrievalFilter());

  for (std::uint64_t i = 0; i < messageCount; ++i)
  {
    std::size_t messageLength = 0;

    infoQueue->GetMessage(i, nullptr, &messageLength);

    std::vector<uint8_t> buffer(messageLength);
    auto* message = reinterpret_cast<D3D11_MESSAGE*>(buffer.data());

    infoQueue->GetMessage(i, message, &messageLength);

    // Conectar con tu sistema de logging.
    LOG_INFO("[D3D11] {}", message->pDescription);
  }

  infoQueue->ClearStoredMessages();
  infoQueue->Release();
#endif
}

void D3D11GraphicsDevice::ClearBackBuffer(float r, float g, float b)
{
  auto* nativeRenderTargetView = m_SwapChain->GetNativeRenderTargetView();
  ASSERT(nativeRenderTargetView != nullptr); 
  if (nativeRenderTargetView == nullptr)
  {
    LOG_ERR("D3D11 error: Most likely the render target view didn't get properly created.");
    return;
  }

  ASSERT(m_Context != nullptr);
  if (!m_Context)
  {
    LOG_ERR("D3D11 error: Context didn't get properly created.");
    return;
  }

  float clearColor[] = { r, g, b, 1.0f };
  m_Context->ClearRenderTargetView(nativeRenderTargetView, clearColor);
}
