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

target_link_libraries(Runtime
  PRIVATE
    glad
    SDL3::SDL3
)

target_compile_definitions(Runtime 
  PRIVATE 
    RUNTIME_BUILD
)

if (ENGINE_OPENGL)
  target_compile_definitions(Runtime PRIVATE ENGINE_OPENGL)
endif()

if (ENGINE_D3D11)
  target_compile_definitions(Runtime PRIVATE ENGINE_D3D11)
endif()

if (ENGINE_METAL)
  target_compile_definitions(Runtime PRIVATE ENGINE_METAL)
endif()
