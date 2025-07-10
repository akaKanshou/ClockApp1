
add_library(TimerAppLib STATIC "TimerAppLib.cpp" "TimerAppLib.h")

target_include_directories(TimerAppLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})