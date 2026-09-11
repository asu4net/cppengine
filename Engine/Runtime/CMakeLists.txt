add_library(Runtime SHARED
  Core/Private/Application.cpp
)

target_include_directories(Runtime 
  PRIVATE
    Base/Private
    Core/Private
  PUBLIC
    Base/Public
    Core/Public
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
