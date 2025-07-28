
add_library(TimerAppLib STATIC "Widgets.h" "Widgets.cpp")

target_include_directories(TimerAppLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(TimerAppLib PUBLIC ${PROJECT_SOURCE_DIR}/deps1)

target_link_libraries(TimerAppLib PUBLIC OpenGL::GL)
target_link_libraries(TimerAppLib PUBLIC glfw)
target_link_libraries(TimerAppLib PUBLIC Settings)
target_link_libraries(TimerAppLib PUBLIC ftype2)
target_link_libraries(TimerAppLib PUBLIC fileDetLib)