add_executable(helloworld
                src/helloworld.cpp)

target_link_libraries(helloworld PRIVATE cangine)

add_custom_command(TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../../cangine/shaders"
        "$<TARGET_FILE_DIR:helloworld>/shaders"
    COMMENT "Copying shaders..."
)
