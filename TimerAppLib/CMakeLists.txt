
add_library(TimerAppLib STATIC "TimerAppLib.cpp" "TimerAppLib.h" "Widgets.h" "Widgets.cpp" "../deps1/Shaderh.h")

target_include_directories(TimerAppLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TimerAppLib PRIVATE ${PROJECT_SOURCE_DIR}/deps1)