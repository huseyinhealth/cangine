# Sandbox executable
add_executable(sandbox
    src/sandbox.cpp
)

target_link_libraries(sandbox PRIVATE cangine)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../../cangine/shaders"
        "$<TARGET_FILE_DIR:sandbox>/shaders"
    COMMENT "Copying shaders..."
)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/cangine.png"
        "$<TARGET_FILE_DIR:sandbox>/cangine.png"
    COMMENT "Copying cangine.png..."
)

