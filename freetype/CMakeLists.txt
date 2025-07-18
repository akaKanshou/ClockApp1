
add_library(ftype2 INTERFACE)

if (WIN32) 
	target_link_libraries(ftype2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}freetype${CMAKE_STATIC_LIBRARY_SUFFIX})
elseif (UNIX) 
	find_package(Freetype REQUIRED)
	target_link_libraries(ftype2 INTERFACE freetype)
endif()

target_include_directories(ftype2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})