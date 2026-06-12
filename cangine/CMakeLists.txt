cmake_minimum_required(VERSION 3.15)
project(cangine)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(EPOXY REQUIRED IMPORTED_TARGET epoxy)
pkg_check_modules(FONTCONFIG REQUIRED IMPORTED_TARGET fontconfig)

# engine
add_library(cangine SHARED
# audio
    src/cangine/audio/audiomanager.cpp
    src/cangine/audio/sound.cpp
# engine
    src/cangine/engine/components/ui/button_component.cpp
    src/cangine/engine/components/ui/inputfield_component.cpp
    src/cangine/engine/components/ui/panel_component.cpp
    src/cangine/engine/components/camera_component.cpp
    src/cangine/engine/components/component.cpp
    src/cangine/engine/components/quadrenderer_component.cpp
    src/cangine/engine/components/textrenderer_component.cpp
    src/cangine/engine/components/transform_component.cpp
    src/cangine/engine/gameobjects/ui/button_object.cpp
    src/cangine/engine/gameobjects/ui/inputfield_object.cpp
    src/cangine/engine/gameobjects/ui/label_object.cpp
    src/cangine/engine/gameobjects/ui/panel_object.cpp
    src/cangine/engine/gameobjects/camera_object.cpp
    src/cangine/engine/gameobjects/game_object.cpp
    src/cangine/engine/object_manager.cpp
# platform
    src/cangine/platform/input.cpp
    src/cangine/platform/key.cpp
    src/cangine/platform/mouse.cpp
    src/cangine/platform/window.cpp
# render
    src/cangine/render/renderer/primitive_renderer.cpp
    src/cangine/render/renderer/quad_renderer.cpp
    src/cangine/render/renderer/render_system.cpp
    src/cangine/render/renderer/text_renderer.cpp
    src/cangine/render/ebo.cpp
    src/cangine/render/shader.cpp
    src/cangine/render/texture.cpp
    src/cangine/render/vao.cpp
    src/cangine/render/vbo.cpp
# util
    src/cangine/util/position.cpp
# root
    src/cangine/cangine.cpp
)

# Include paths
target_include_directories(cangine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    external/stb_image
    external/miniaudio
)

target_link_libraries(cangine PUBLIC
    glfw
    PkgConfig::EPOXY
    Freetype::Freetype
    PkgConfig::FONTCONFIG
)

set_target_properties(cangine PROPERTIES
    VERSION 0.1.0
    SOVERSION 1
)

install(
    TARGETS cangine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/cangine"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/cangine
)
