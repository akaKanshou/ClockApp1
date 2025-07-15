
add_library(TimerAppLib STATIC "TimerAppLib.cpp" "TimerAppLib.h" "Widgets.h" "Widgets.cpp")

target_include_directories(TimerAppLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TimerAppLib PUBLIC ${PROJECT_SOURCE_DIR}/deps1)

target_link_libraries(TimerAppLib PUBLIC Settings)