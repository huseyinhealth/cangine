add_executable(audio
    src/audio.cpp
)

target_link_libraries(audio PRIVATE cangine)


add_custom_command(TARGET audio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/exhilarate.mp3"
        "$<TARGET_FILE_DIR:audio>"
)

