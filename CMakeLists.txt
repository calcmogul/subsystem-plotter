cmake_minimum_required(VERSION 3.21)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

project(subsystem-plotter)

# Generate compile_commands.json by default
if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS "YES" CACHE STRING "" FORCE)
endif()

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(WITH_CSCORE OFF CACHE INTERNAL "With CSCore")
set(WITH_GUI ON CACHE INTERNAL "With GUI")
set(WITH_JAVA OFF CACHE INTERNAL "With Java")
set(WITH_NTCORE ON CACHE INTERNAL "With NTCore")
set(WITH_PROTOBUF OFF CACHE BOOL "" FORCE)
set(WITH_SIMULATION_MODULES OFF CACHE INTERNAL "With Simulation Modules")
set(WITH_TESTS OFF CACHE INTERNAL "With Tests")
set(WITH_WPILIB OFF CACHE INTERNAL "With WPILib")
set(WITH_WPIMATH ON CACHE INTERNAL "With WPIMath")
fetchcontent_declare(
    wpilib
    GIT_REPOSITORY https://github.com/wpilibsuite/allwpilib.git
    GIT_TAG v2025.3.2
    EXCLUDE_FROM_ALL
    SYSTEM
)
fetchcontent_makeavailable(wpilib)

# Place libraries in lib and binaries in bin
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Create our main executable
file(GLOB_RECURSE subsystem-plotter-sources src/main/native/cpp/*.cpp)
file(GLOB_RECURSE subsystem-plotter-headers src/main/native/include/*.h)

add_executable(
    subsystem-plotter
    ${subsystem-plotter-sources}
    ${subsystem-plotter-headers}
)
if(APPLE)
    set_target_properties(
        subsystem-plotter
        PROPERTIES LINK_FLAGS "-framework Metal -framework QuartzCore"
    )
else()
    target_link_libraries(subsystem-plotter PUBLIC stdc++fs)
endif()

target_compile_options(
    subsystem-plotter
    PRIVATE
        -Wall
        -pedantic
        -Wextra
        -Werror
        -Wno-unused-parameter
        -Wno-error=deprecated-declarations
)
target_include_directories(subsystem-plotter PUBLIC src/main/native/include)

# Link to imgui and WPILib
target_link_libraries(subsystem-plotter PUBLIC wpigui imgui)
