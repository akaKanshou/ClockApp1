
add_library(fileDetLib STATIC "fileDet.h" "fileDet.cpp")

target_include_directories(fileDetLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(fileDetLib PRIVATE Settings)