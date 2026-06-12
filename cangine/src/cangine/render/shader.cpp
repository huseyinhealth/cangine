#include "shader.hpp"

#include <epoxy/gl.h>
#include <GLFW/glfw3.h>

namespace cangine {

    Shader::Shader(std::string name, std::string shaderPath) {
        int success;
        char infoLog[512];

        this->name = name;
        this->shaderPath = shaderPath;
        this->ID = glCreateProgram();

        this->compileShaders();

        if (this->valid) {
            glGetShaderiv(this->vs, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(this->vs, 512, NULL, infoLog);
                std::cout << "VERTEX SHADER ERROR\n" << infoLog << "\n";
                this->valid = false;
            }

            glGetShaderiv(this->fs, GL_COMPILE_STATUS, &success);
            if (!success) {
                this->valid = false;
                glGetShaderInfoLog(this->fs, 512, NULL, infoLog);
                std::cout << "FRAGMENT SHADER ERROR\n" << infoLog << "\n";
            }
        }

        if (!this->valid) {
            glDeleteProgram(this->ID);
            this->ID = 0;
            this->cleanUp();
            glfwTerminate();
            throw Exception::ShaderException("Shader is not a valid (" + this->name + ")");
        }

        this->attach();
        this->cleanUp();

    }

    Shader::~Shader() {
        glDeleteProgram(this->ID);
    }

    void Shader::attach() {
        int success;
        char infolog[512];

        glAttachShader(this->ID, this->vs);
        glAttachShader(this->ID, this->fs);

        glLinkProgram(this->ID);

        glGetProgramiv(this->ID, GL_LINK_STATUS, &success);

        if (!success) {
            this->valid = false;
            glGetProgramInfoLog(this->ID, 512, NULL, infolog);
            std::cout << "SHADER PROGRAM LINK ERROR " << infolog << "\n";
        }
    }

    void Shader::cleanUp() {
        if (this->vs) glDeleteShader(this->vs);
        if (this->fs) glDeleteShader(this->fs);
    }

    void Shader::compileFromSource(const std::string& vCode, const std::string& fCode) {
        const char* vsSrc = vCode.c_str();
        const char* fsSrc = fCode.c_str();

        this->vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(this->vs, 1, &vsSrc, NULL);
        glCompileShader(this->vs);

        this->fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(this->fs, 1, &fsSrc, NULL);
        glCompileShader(this->fs);
    }

    void Shader::compileShaders() {
        std::string
            vsPath = this->shaderPath + "/" + this->name + ".vert",
            fsPath = this->shaderPath + "/" + this->name + ".frag";

        std::ifstream
            vFile(vsPath),
            fFile(fsPath);

        #ifdef CANGINE_DEBUG
            std::cout << vsPath << "\n" << fsPath << "\n";
        #endif

        if (!vFile.is_open() || !fFile.is_open()) {
            std::cout << "Failed to open shader files.\n";
            this->valid = false;
            this->vs = 0;
            this->fs = 0;
            return;
        }

        std::stringstream vStream, fStream;
        vStream << vFile.rdbuf();
        fStream << fFile.rdbuf();

        this->compileFromSource(vStream.str(), fStream.str());
    }

    Shader::Shader(const std::string& vsSrc, const std::string& fsSrc, SourceTag) {
        int success;
        char infoLog[512];

        this->name = "<source>";
        this->shaderPath = "";
        this->ID = glCreateProgram();

        this->compileFromSource(vsSrc, fsSrc);

        if (this->valid) {
            glGetShaderiv(this->vs, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(this->vs, 512, NULL, infoLog);
                std::cout << "VERTEX SHADER ERROR\n" << infoLog << "\n";
                this->valid = false;
            }

            glGetShaderiv(this->fs, GL_COMPILE_STATUS, &success);
            if (!success) {
                this->valid = false;
                glGetShaderInfoLog(this->fs, 512, NULL, infoLog);
                std::cout << "FRAGMENT SHADER ERROR\n" << infoLog << "\n";
            }
        }

        if (!this->valid) {
            glDeleteProgram(this->ID);
            this->ID = 0;
            this->cleanUp();
            glfwTerminate();
            throw Exception::ShaderException("Shader source compilation failed");
        }

        this->attach();
        this->cleanUp();
    }

    Shader* Shader::fromSource(const std::string& vsSrc, const std::string& fsSrc) {
        return new Shader(vsSrc, fsSrc, SourceTag{});
    }

    void Shader::use() {
        if (!this->valid) {
            std::cout << "Attempted to use invalid shader program!\n";
            return;
        }

        glUseProgram(this->ID);
    }

    void Shader::unbind() {
        glUseProgram(0);
    }

    void Shader::setBool(const std::string& name, bool value) {
        if (!valid) return;
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::setInt(const std::string& name, int value) {
        if (!valid) return;
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setFloat(const std::string& name, float value) {
        if (!valid) return;
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::setVec2(const std::string& name, float x, float y) {
        if (!valid) return;
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void Shader::setVec3(const std::string& name, float x, float y, float z) {
        if (!valid) return;
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
        if (!valid) return;
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
}