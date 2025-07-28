# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (ClockApp1 VERSION 1.0)

find_package(OpenGL REQUIRED)
# Include sub-projects.

add_subdirectory ("TimerAppLib")
add_subdirectory ("TimerApp")
add_subdirectory("glfw-3.4")
add_subdirectory("freetype")
add_subdirectory("fileDetLib")

add_library(Settings INTERFACE)

configure_file(filePaths.h.in filePaths.h)
target_include_directories(Settings INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(Settings INTERFACE cxx_std_20)

install(TARGETS TimerApp 
  RUNTIME DESTINATION bin)

install(FILES ${CMAKE_SOURCE_DIR}/freetype/freetype.dll 
  DESTINATION bin)

message("--INSTALLED BIN FILES")

install(FILES "${CMAKE_SOURCE_DIR}/deps1/bg3.png" "${CMAKE_SOURCE_DIR}/deps1/button.png"
  DESTINATION share/ClockApp1)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/deps1/Shaders
  DESTINATION share/ClockApp1)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/deps1/Fonts
  DESTINATION share/ClockApp1)

message("--INSTALLED SHARE FILES")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${ClockApp1_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ClockApp1_VERSION_MINOR}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_WIX_VERSION 4)
include(CPack)