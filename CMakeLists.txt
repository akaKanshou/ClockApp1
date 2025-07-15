# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (ClockApp1)

find_package(OpenGL REQUIRED)
# Include sub-projects.

add_subdirectory ("TimerAppLib")
add_subdirectory ("TimerApp")
add_subdirectory("glfw-3.4")

add_library(Settings INTERFACE)

configure_file(filePaths.h.in filePaths.h)
target_include_directories(Settings INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(Settings INTERFACE cxx_std_20)
