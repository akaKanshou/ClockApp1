# CMakeList.txt : CMake project for ClockApp1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (ClockApp1 "ClockApp1.cpp" "ClockApp1.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ClockApp1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
