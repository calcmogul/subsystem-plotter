cmake_minimum_required(VERSION 3.28)

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

file(GLOB_RECURSE cpp_src src/*.cpp)
add_executable(subsystem-plotter ${cpp_src})

if(APPLE)
    set_target_properties(
        subsystem-plotter
        PROPERTIES LINK_FLAGS "-framework Metal -framework QuartzCore"
    )
endif()

target_compile_features(subsystem-plotter PUBLIC cxx_std_23)
if(NOT MSVC)
    target_compile_options(
        subsystem-plotter
        PRIVATE -Wall -pedantic -Wextra -Werror
    )
else()
    # Suppress the following warnings:
    #   * C4244: lossy conversion
    target_compile_options(subsystem-plotter PRIVATE /wd4244 /WX)
endif()
target_include_directories(subsystem-plotter PUBLIC include)
target_link_libraries(subsystem-plotter PUBLIC wpigui imgui)
