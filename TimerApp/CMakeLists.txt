
add_executable(TimerApp "TimerApp.cpp" "glad.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TimerApp PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(TimerApp PUBLIC TimerAppLib)

if (WIN32)
    add_custom_command(TARGET TimerApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/freetype/freetype.dll"
            $<TARGET_FILE_DIR:TimerApp>
    )
endif()