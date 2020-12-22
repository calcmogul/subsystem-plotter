cmake_minimum_required(VERSION 3.18)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(subsystem-plotter)

add_subdirectory(allwpilib)

# Place libraries in lib and binaries in bin.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set C++ standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Create our main executable.
file(GLOB_RECURSE subsystem-plotter-sources src/main/native/cpp/*.cpp)
file(GLOB_RECURSE subsystem-plotter-headers src/main/native/include/*.h)

add_executable(subsystem-plotter ${subsystem-plotter-sources} ${subsystem-plotter-headers})
if (APPLE)
  set_target_properties(subsystem-plotter PROPERTIES LINK_FLAGS "-framework Metal -framework QuartzCore")
else()
  target_link_libraries(subsystem-plotter PUBLIC stdc++fs)
endif()

target_compile_options(subsystem-plotter PRIVATE -Wall -pedantic -Wextra -Werror -Wno-unused-parameter -Wno-error=deprecated-declarations)
target_include_directories(subsystem-plotter PUBLIC src/main/native/include)

# Link to imgui and WPILib.
target_link_libraries(subsystem-plotter PUBLIC wpigui imgui wpimath ntcore wpiutil)
