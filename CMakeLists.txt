cmake_minimum_required(VERSION 3.20)

# Common attributes for all the subprojects.
project(cppengine VERSION 1.0 LANGUAGES CXX C)
# - VERSION   Version of the project.
# - LANGUAGES If not set, determined by the file extension.

# Try to use the specified c++ version.
set(CMAKE_CXX_STANDARD 23)

# Enforce the specified c++ version.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The output executable will go to the Binaries directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)

# The output libraries will go to the Binaries directory.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)

# The intermediate files will go to Binaries-Intermediate directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries-Intermediate)

add_compile_definitions(
  $<$<CONFIG:Debug>:CONFIG_DEBUG>
  $<$<CONFIG:Release>:CONFIG_RELEASE>
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

add_subdirectory(Engine/Runtime)
add_subdirectory(Engine/ThirdParty/SDL)
add_subdirectory(Engine/ThirdParty/glad)
add_subdirectory(Launcher)

# Build manual:

# [command] cmake -S . -B Build
# -S: Where is the root CMakeLists.txt
# -B: Where should cmake generate the build folder.

# [command] cmake --build Build
# This uses the files generated in the build folder to
# compile the projects.

# Regular workflow on Windows: cls && cmake --build Build && call "Binaries/Debug/game.exe"
