
add_library(ftype2 INTERFACE)

if (WIN32) 
	target_link_libraries(ftype2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/freetype.lib)
elseif (UNIX) 
	target_link_libraries(ftype2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/libfreetype.a)
endif()

#link_directories(${CMAKE_BINARY_DIR}/freetype)

target_include_directories(ftype2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})