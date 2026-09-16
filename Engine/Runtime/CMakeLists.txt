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
