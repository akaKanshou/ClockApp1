
add_executable(TimerApp "TimerApp.cpp" "glad.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TimerApp PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(TimerApp PUBLIC TimerAppLib)
target_link_libraries(TimerApp PUBLIC glfw)
target_link_libraries(TimerApp PUBLIC OpenGL::GL)