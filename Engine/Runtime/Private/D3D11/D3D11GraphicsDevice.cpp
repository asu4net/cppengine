#include "D3D11GraphicsDevice.h"

ID3D11Device* D3D11GraphicsDevice::SafeGetNativeDevice(GraphicsHandle deviceHandle)
{
  bool validDevice = Application::GetInstance().GetGraphicsStorage().IsValid(deviceHandle);
  ASSERT(validDevice);
  if (!validDevice)
  {
      LOG_ERR("D3D11 error! Can't properly construct the resource due to missing device pointer.");
      return nullptr;
  }

  auto* baseDevice = Application::GetInstance().GetGraphicsStorage().Get<GraphicsDevice>(deviceHandle);
  ASSERT(baseDevice != nullptr);
  if (baseDevice == nullptr)
  {
      LOG_ERR("D3D11 error! Unexpected GraphicsStorage error! Can't retrieve data from valid handle!");
      return nullptr;
  }

  auto* device = static_cast<D3D11GraphicsDevice*>(baseDevice);

  auto* nativeDevice = device->GetNativeDevice();

  ASSERT(nativeDevice != nullptr);
  if (nativeDevice == nullptr)
  {
      LOG_ERR("D3D11 error! Unexpected device error! Can't retrieve native device!");
      return nullptr;
  }

  return nativeDevice;
}

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
  ASSERT(m_SwapChain != nullptr);
  if (!m_SwapChain)
  {
    LOG_ERR("D3D11 error: SwapChain didn't get properly created.");
    return;
  }

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

void D3D11GraphicsDevice::SetGraphicsState(const GraphicsState& state) 
{
  // @Review: in the future this will probably be a command.
  m_State = state;
}

void D3D11GraphicsDevice::ImmediateDraw()
{
  if (m_State.sizeOfVertices == 0)
  {
    LOG_ERR("D3D11 error: ImmediateDraw: Need valid vertex size.");
    return;
  }

  ASSERT(m_Context != nullptr);
  if (!m_Context)
  {
    LOG_ERR("D3D11 error: Context didn't get properly created.");
    return;
  }

  ASSERT(m_SwapChain != nullptr);
  if (!m_SwapChain)
  {
    LOG_ERR("D3D11 error: Swap Chain didn't get properly created.");
    return;
  }

  // This should be done just when create/resize the swap chain.
  D3D11_VIEWPORT viewport{};
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;
  viewport.Width    = static_cast<float>(1270);
  viewport.Height   = static_cast<float>(720);
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;
  m_Context->RSSetViewports(1, &viewport);

  // Set the render target.
  m_Context->OMSetRenderTargets(1u, &m_SwapChain->GetNativeRenderTargetView(), nullptr);

  // Bind the Vertex Shader.
  bool validVS = Application::GetInstance().GetGraphicsStorage().IsValid(m_State.vertexShader);
  if (!validVS)
  {
    LOG_ERR("D3D11 error: ImmediateDraw: No VS shader bound to the state.");
    return;
  }

  auto* vsBase = Application::GetInstance().GetGraphicsStorage().Get<Shader>(m_State.vertexShader);
  auto* vsDerived = vsBase ? static_cast<D3D11Shader*>(vsBase) : nullptr;
  if (vsShader != nullptr)
  {
    const D3D11ShaderData& vsDerivedData = vsDerived->GetNativeShaderData();
    if (vsDerivedData.stage == ShaderStage::Vertex)
    {
      m_Context->VSSetShader(vsDerivedData.vs, nullptr);
    }
  }

  // Bind the Pixel Shader.
  bool validPS = Application::GetInstance().GetGraphicsStorage().IsValid(m_State.pixelShader);
  if (!validPS)
  {
    LOG_ERR("D3D11 error: ImmediateDraw: No PS shader bound to the state.");
    return;
  }

  auto* psBase = Application::GetInstance().GetGraphicsStorage().Get<Shader>(m_State.pixelShader);
  auto* psDerived = psBase ? static_cast<D3D11Shader*>(psBase) : nullptr;
  if (psShader != nullptr)
  {
    const D3D11ShaderData& psDerivedData = psDerived->GetNativeShaderData();
    if (psDerivedData.stage == ShaderStage::Pixel)
    {
      m_Context->PSSetShader(psDerivedData.ps, nullptr);
    }
  }

  // Bind the Vertex Buffer.
  bool validVB = Application::GetInstance().GetGraphicsStorage().IsValid(m_State.vertexBuffer);
  if (!validVB)
  {
    LOG_ERR("D3D11 error: ImmediateDraw: No vertex buffer bound to the state.");
    return;
  }

  auto* vbBase = Application::GetInstance().GetGraphicsStorage().Get<VertexBuffer>(m_State.vertexBuffer);
  auto* vbDerived = vbBase ? static_cast<D3D11VertexBuffer*>(vbBase) : nullptr;
  if (vbShader != nullptr)
  {
    ID3D11VertexBuffer* vertexBuffer = psDerived->GetNativeVertexBuffer();
    if (vertexBuffer != nullptr)
    {
      // @Pending: stride
      // @Pending: offset
      context->IASetVertexBuffers(/* start slot */ 0u, /* num of buffers */ 1u, &vertexBuffer, &stride, &offset);
    }
  }

  m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
  m_Context->Draw(m_State.sizeOfVertices, 0u);
}
