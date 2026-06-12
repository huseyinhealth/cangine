#pragma once

#include <string>
#include <exception>
/// @brief Exception types thrown by the engine.
namespace cangine::Exception {
    /**
     * @brief Base class for all engine exceptions.
     *
     * Prefixes the supplied message with @c "cangine err: ".
     */
    class CangineException : public std::exception {
        public:
        std::string message; ///< Full formatted error message.
        /**
         * @brief Constructs the exception with a message.
         * @param msg Human-readable description of the error.
         */
        inline CangineException(std::string msg) : message("cangine err: " + msg) {};
        /**
         * @brief Returns the formatted error message.
         * @return Null-terminated message string.
         */
        inline virtual const char* what() const noexcept override {
            return message.c_str();
        }
    };


    /**
     * @brief Exception thrown for shader compilation/linking failures.
     *
     * Adds a @c "shader err: " prefix on top of the base message.
     */
    class ShaderException : public CangineException {
        public:
        /**
         * @brief Constructs the exception with a message.
         * @param msg Human-readable description of the shader error.
         */
        inline ShaderException(std::string msg) : CangineException("shader err: " + msg) {};
    };
}
