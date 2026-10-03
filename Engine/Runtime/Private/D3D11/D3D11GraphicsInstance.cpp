#include "D3D11GraphicsInstance.h"

D3D11GraphicsInstance::D3D11GraphicsInstance(const GraphicsInstanceParams& params)
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();

  // Alloc the device.
  GraphicsDeviceParams graphicsDeviceParams{ params.windowHandle };
  m_DeviceHandle = graphicsStorage.Emplace<GraphicsDevice>(graphicsDeviceParams);
  m_Device = static_cast<D3D11GraphicsDevice*>(graphicsStorage.Get<GraphicsDevice>(m_DeviceHandle));
  // @Pending: Check if creation went good.
  LOG_INFO("D3D11 Graphics instance created!");
}

D3D11GraphicsInstance::~D3D11GraphicsInstance()
{
  auto& graphicsStorage = Application::GetInstance().GetGraphicsStorage();
  graphicsStorage.Remove<GraphicsDevice>(m_DeviceHandle);
  LOG_INFO("D3D11 Graphics instance destroyed!");
}

GraphicsDevice& D3D11GraphicsInstance::GetDevice()
{
  ASSERT(m_Device != nullptr);
  return *m_Device;
}

void D3D11GraphicsInstance::DrawTestTriangle()
{
  auto* context = m_Device->GetNativeContext();
  auto* device  = m_Device->GetNativeDevice();

  // Create and compile the shaders.

  // -----------------------------------------------
  // VERTEX SHADER
  // -----------------------------------------------
  static ID3D11InputLayout* inputLayout = nullptr;
  static ID3D11VertexShader* vertexShader = nullptr;
  static bool vertexShaderCreated = false;

  if (!vertexShaderCreated)
  {
    vertexShaderCreated = true;
    ID3DBlob* bytecode = nullptr;
    ID3DBlob* errors = nullptr;

    HRESULT result = D3DCompile(
      Shaders::TestTriangle,           // SrcData.
      strlen(Shaders::TestTriangle),   // SrcDataSize.
      "TestTriangle.hlsl",             // SourceName.
      nullptr,                         // Defines.
      nullptr,                         // Include.
      "VSMain",                        // EntryPoint.
      "vs_5_0",                        // Target.
      D3DCOMPILE_ENABLE_STRICTNESS,    // Flags1.
      0,                               // Flags2.
      &bytecode,                       // Bytecode.
      &errors                          // Errors.
    );

    if (FAILED(result))
    {
      if (errors != nullptr)
      {
        const char* message = static_cast<const char*>(errors->GetBufferPointer());
        LOG_ERR("D3D11 Vertex Shader compilation failed: {}", message);
        return;
      }
    }

    device->CreateVertexShader(
      bytecode->GetBufferPointer(),
      bytecode->GetBufferSize(),
      nullptr,
      &vertexShader
    );

    // Create the input layout.
    D3D11_INPUT_ELEMENT_DESC inputElement =
    {
      "POSITION",
      0,
      DXGI_FORMAT_R32G32_FLOAT,
      0,
      0,
      D3D11_INPUT_PER_VERTEX_DATA,
      0
    };

    device->CreateInputLayout(
      &inputElement,
      1,
      bytecode->GetBufferPointer(),
      bytecode->GetBufferSize(),
      &inputLayout
    );
  }

  // -----------------------------------------------
  // PIXEL SHADER
  // -----------------------------------------------

  static ID3D11PixelShader* pixelShader = nullptr;
  static bool pixelShaderCreated = false;

  if (!pixelShaderCreated)
  {
    pixelShaderCreated = true;

    ID3DBlob* bytecode = nullptr;
    ID3DBlob* errors = nullptr;

    HRESULT result = D3DCompile(
      Shaders::TestTriangle,           // SrcData.
      strlen(Shaders::TestTriangle),   // SrcDataSize.
      "TestTriangle.hlsl",             // SourceName.
      nullptr,                         // Defines.
      nullptr,                         // Include.
      "PSMain",                        // EntryPoint.
      "ps_5_0",                        // Target.
      D3DCOMPILE_ENABLE_STRICTNESS,    // Flags1.
      0,                               // Flags2.
      &bytecode,                       // Bytecode.
      &errors                          // Errors.
    );

    if (FAILED(result))
    {
      if (errors != nullptr)
      {
        const char* message = static_cast<const char*>(errors->GetBufferPointer());
        LOG_ERR("D3D11 Pixel Shader compilation failed: {}", message);
        return;
      }
    }

    device->CreatePixelShader(
      bytecode->GetBufferPointer(),
      bytecode->GetBufferSize(),
      nullptr,
      &pixelShader
    );
  }

  // -----------------------------------------------
  // VERTEX BUFFER
  // -----------------------------------------------

  static ID3D11Buffer* vertexBuffer = nullptr;
  static bool vertexBufferCreated = false;

  // Triangle vertex position data type.
  struct Vertex
  {
    float x;
    float y;
  };

  // Simple triangle vertex positions.
  const Vertex vertices[] =
  {
    { +0.0f, +0.5f },
    { +0.5f, -0.5f },
    { -0.5f, -0.5f },
  };

  if (!vertexBufferCreated)
  {
    vertexBufferCreated = true;
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    // Size of every vertex.
    bufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = vertices;
    // These two are for textures.
    data.SysMemPitch = 0u;
    data.SysMemSlicePitch = 0u;

    HRESULT result = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
    if FAILED(result)
    {
      m_Device->DumpDebugMessages();
    }

    // This should be done just when create/resize the swap chain.
    D3D11_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width    = static_cast<float>(1270);
    viewport.Height   = static_cast<float>(720);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewport);
  }

  // -----------------------------------------------
  // DRAW THE TRIANGLE
  // -----------------------------------------------
  if (vertexShaderCreated && pixelShaderCreated && vertexBufferCreated)
  {
    std::uint32_t stride = sizeof(Vertex);
    std::uint32_t offset = 0u;
    context->IASetVertexBuffers(/* start slot */ 0u, /* num of buffers */ 1u, &vertexBuffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(inputLayout);
    context->VSSetShader(vertexShader, nullptr, 0u);
    context->PSSetShader(pixelShader, nullptr, 0u);
    auto* renderTargetView = static_cast<D3D11SwapChain&>(m_Device->GetSwapChain()).GetNativeRenderTargetView();
    context->OMSetRenderTargets(1u, &renderTargetView, nullptr);
    context->Draw(std::size(vertices), 0u);
    m_Device->DumpDebugMessages();
  }
}
