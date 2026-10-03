add_library(Runtime SHARED
  Private/Application.cpp
  Private/GraphicsStorage.cpp
)

target_include_directories(Runtime 
  PRIVATE
    Private
  PUBLIC
    Public
)

target_precompile_headers(Runtime
  PRIVATE
    RuntimePCH.h
)

if (ENGINE_SDL)
  target_link_libraries(Runtime
    PRIVATE
      SDL3::SDL3
  )
endif()

if(ENGINE_GRAPHICS_API STREQUAL "D3D11")
  target_compile_definitions(Runtime PRIVATE ENGINE_D3D11)
  target_link_libraries(Runtime 
    PRIVATE
      d3d11
      dxgi
      D3DCompiler
  )
elseif(ENGINE_GRAPHICS_API STREQUAL "OPENGL")
  target_compile_definitions(Runtime PRIVATE ENGINE_OPENGL)
  target_link_libraries(Runtime
    PRIVATE
      glad
  )
elseif(ENGINE_GRAPHICS_API STREQUAL "METAL")
  target_compile_definitions(Runtime PRIVATE ENGINE_METAL)
  # @Pending: Link with metal stuff.
endif()

target_link_libraries(Runtime
  PUBLIC
    glm::glm
)

target_compile_definitions(Runtime 
  PRIVATE 
    RUNTIME_BUILD
)

if (ENGINE_SDL)
  target_compile_definitions(Runtime PRIVATE ENGINE_SDL)
endif()
