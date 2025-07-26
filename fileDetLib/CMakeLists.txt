
add_library(fileDetLib STATIC "fileDet.h" "fileDet.cpp")

target_include_directories(fileDetLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(fileDetLib PRIVATE ${CMAKE_BINARY_DIR})

target_compile_features(fileDetLib PRIVATE cxx_std_20)