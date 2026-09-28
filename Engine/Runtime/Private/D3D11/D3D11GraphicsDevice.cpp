#include "D3D11GraphicsDevice.h"

D3D11GraphicsDevice::D3D11GraphicsDevice(const GraphicsDeviceParams& params)
{
  // @Note: The graphics instance is responsible to create
  // this and give a value for the d3d11 pointers.
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
    LOG_ERR("D3D11 Invalid device when calling DumpDebugMessages()!");
    return;
  }

  ID3D11InfoQueue* infoQueue = nullptr;
  HRESULT result = m_Device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(&infoQueue));

  if (FAILED(result))
  {
    return;
  }

  const UINT64 messageCount = infoQueue->GetNumStoredMessages();

  for (std::uint64_t i = 0; i < messageCount; ++i)
  {
    std::size_t messageLength = 0;

    infoQueue->GetMessage(i, nullptr, &messageLength);

    std::vector<uint8_t> buffer(messageLength);
    auto* message = reinterpret_cast<D3D11_MESSAGE*>(buffer.data());

    infoQueue->GetMessage(i, message, &messageLength);

    // Conectar con tu sistema de logging.
    LOG_ERR("[D3D11] {}", message->pDescription);
  }

  infoQueue->ClearStoredMessages();
  infoQueue->Release();
#endif
}
