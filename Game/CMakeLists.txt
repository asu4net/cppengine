add_executable(Launcher 
  Private/main.cpp
)

set_target_properties(Launcher 
  PROPERTIES 
    OUTPUT_NAME "game"
)

target_link_libraries(Launcher
  PRIVATE 
    Runtime
)
