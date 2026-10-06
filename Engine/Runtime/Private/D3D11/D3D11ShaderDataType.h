#pragma once

#include "Runtime/ShaderDataType.h"

inline DXGI_FORMAT ShaderDataTypeToD3D11(ShaderDataType type)
{
  switch (type)
  {
    case ShaderDataType::Float:  return DXGI_FORMAT_R32_FLOAT;
    case ShaderDataType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
    case ShaderDataType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
    case ShaderDataType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;

    case ShaderDataType::Int:  return DXGI_FORMAT_R32_SINT;
    case ShaderDataType::Int2: return DXGI_FORMAT_R32G32_SINT;
    case ShaderDataType::Int3: return DXGI_FORMAT_R32G32B32_SINT;
    case ShaderDataType::Int4: return DXGI_FORMAT_R32G32B32A32_SINT;

    case ShaderDataType::Bool:
        return DXGI_FORMAT_R32_UINT;

    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        // Una matrix occupies multiple input elems in d3d11.
        return DXGI_FORMAT_UNKNOWN;

    case ShaderDataType::None:
    default:
        return DXGI_FORMAT_UNKNOWN;
  }
}
