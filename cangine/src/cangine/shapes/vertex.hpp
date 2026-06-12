#pragma once

#include "../types/types.hpp"
#include "../util/color.hpp"

namespace cangine {
    /**
     * @brief A single renderable vertex: position, color and texture coordinate.
     *
     * Bundles everything the renderer needs per vertex. Provides constructors
     * that accept either explicit float components or a @ref Vector3 position,
     * with color and UV defaulting to black / (0,0).
     */
    struct Vertex {
        float x{0.0f}, y{0.0f}, z{0.0f}; ///< Position components.
        Color color{Colors::BLACK};      ///< Vertex color.
        Vector2 uv{0.0f, 0.0f};          ///< Texture coordinate.

        /// @brief Constructs a vertex at the origin, black, with zero UV.
        constexpr Vertex() noexcept = default;

        /**
         * @brief Constructs a fully specified vertex.
         * @param _x Position x component.
         * @param _y Position y component.
         * @param _z Position z component.
         * @param c Vertex color.
         * @param texCoord Texture coordinate.
         */
        constexpr Vertex(float _x, float _y, float _z, const Color& c, const Vector2& texCoord) noexcept
            : x(_x), y(_y), z(_z), color(c), uv(texCoord) {}

        /**
         * @brief Constructs a colored vertex with a zero texture coordinate.
         * @param _x Position x component.
         * @param _y Position y component.
         * @param _z Position z component.
         * @param c Vertex color.
         */
        constexpr Vertex(float _x, float _y, float _z, const Color& c) noexcept
            : Vertex(_x, _y, _z, c, Vector2{0.0f, 0.0f}) {}

        /**
         * @brief Constructs a black vertex from explicit components.
         * @param _x Position x component.
         * @param _y Position y component.
         * @param _z Position z component.
         */
        constexpr Vertex(float _x, float _y, float _z) noexcept
            : Vertex(_x, _y, _z, Colors::BLACK) {}

        /**
         * @brief Constructs a fully specified vertex from a position vector.
         * @param v Position vector.
         * @param c Vertex color.
         * @param texCoord Texture coordinate.
         */
        constexpr Vertex(const Vector3& v, const Color& c, const Vector2& texCoord) noexcept
            : Vertex(v.x, v.y, v.z, c, texCoord) {}

        /**
         * @brief Constructs a colored vertex from a position vector.
         * @param v Position vector.
         * @param c Vertex color.
         */
        constexpr Vertex(const Vector3& v, const Color& c) noexcept
            : Vertex(v.x, v.y, v.z, c) {}

        /**
         * @brief Constructs a black vertex from a position vector.
         * @param v Position vector.
         */
        constexpr Vertex(const Vector3& v) noexcept
            : Vertex(v.x, v.y, v.z, Colors::BLACK) {}
    };
}
