#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "../engine/exception.hpp"

#define CANGINE_DEFAULT_VERT_SRC R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 uv;
out vec4 ColorOut;
out vec2 vUv;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ColorOut = aColor;
    vUv = uv;
}
)"

#define CANGINE_DEFAULT_FRAG_SRC R"(#version 330 core
out vec4 FragColor;
in vec4 ColorOut;
in vec2 vUv;
uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform bool uMono;
void main()
{
    if (uUseTexture)
    {
        if (uMono)
        {
            float alpha = texture(uTexture, vUv).r;
            FragColor = vec4(ColorOut.rgb, alpha * ColorOut.a);
        }
        else
        {
            vec4 t = texture(uTexture, vUv) * ColorOut;
            FragColor = t;
        }
    }
    else
    {
        FragColor = ColorOut;
    }
}
)"

// "
// ^ to fix ide coloring issues

namespace cangine {

    /**
     * @brief Wraps a compiled and linked OpenGL shader program.
     *
     * Loads a vertex/fragment shader pair from disk, compiles and links them
     * into a program, and exposes helpers for setting uniform values.
     */
    class Shader {
    private:
        unsigned int vs; ///< Compiled vertex shader handle.
        unsigned int fs; ///< Compiled fragment shader handle.

        struct SourceTag {};

        /// @brief Attaches and links the compiled shader stages into the program.
        void attach();
        /// @brief Deletes the intermediate shader stage objects after linking.
        void cleanUp();
        /// @brief Reads and compiles the vertex and fragment shader sources.
        void compileShaders();
        /// @brief Compiles vertex and fragment shaders directly from source strings.
        void compileFromSource(const std::string& vsSrc, const std::string& fsSrc);

        Shader(const std::string& vsSrc, const std::string& fsSrc, SourceTag);

    public:
        std::string shaderPath; ///< Directory the shader sources are loaded from.

        bool valid = true;   ///< Whether compilation and linking succeeded.
        unsigned int ID;     ///< OpenGL shader program handle.
        std::string name;    ///< Base name of the shader source files.

        /// @brief Activates this shader program for subsequent draw calls.
        void use();
        /// @brief Deactivates the current shader program.
        void unbind();
        /**
         * @brief Sets a boolean uniform.
         * @param name Uniform variable name.
         * @param value Value to assign.
         */
        void setBool(const std::string& name, bool value);
        /**
         * @brief Sets an integer uniform.
         * @param name Uniform variable name.
         * @param value Value to assign.
         */
        void setInt(const std::string& name, int value);
        /**
         * @brief Sets a float uniform.
         * @param name Uniform variable name.
         * @param value Value to assign.
         */
        void setFloat(const std::string& name, float value);
        /**
         * @brief Sets a vec2 uniform.
         * @param name Uniform variable name.
         * @param x First component.
         * @param y Second component.
         */
        void setVec2(const std::string& name, float x, float y);
        /**
         * @brief Sets a vec3 uniform.
         * @param name Uniform variable name.
         * @param x First component.
         * @param y Second component.
         * @param z Third component.
         */
        void setVec3(const std::string& name, float x, float y, float z);
        /**
         * @brief Sets a vec4 uniform.
         * @param name Uniform variable name.
         * @param x First component.
         * @param y Second component.
         * @param z Third component.
         * @param w Fourth component.
         */
        void setVec4(const std::string& name, float x, float y, float z, float w);
        /**
         * @brief Loads, compiles and links a shader program.
         * @param name Base name of the shader source files.
         * @param shaderPath Directory containing the shader sources.
         */
        Shader(std::string name, std::string shaderPath = "./shaders");
        /// @brief Deletes the OpenGL shader program.
        ~Shader();

        /**
         * @brief Creates a shader program compiled from source strings.
         * @param vsSrc Vertex shader GLSL source.
         * @param fsSrc Fragment shader GLSL source.
         */
        static Shader* fromSource(const std::string& vsSrc, const std::string& fsSrc);
    };
}