
add_executable(TimerApp "TimerApp.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TimerApp PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(TimerApp PUBLIC TimerAppLib)
target_include_directories(TimerApp PRIVATE ${PROJECT_SOURCE_DIR})