#pragma once

#include <array>
#include "../util/color.hpp"
#include "../types/vector3.hpp"
#include "vertex.hpp"
#include "rectangle.hpp"

namespace cangine {

/**
 * @brief A triangle defined by three vertices.
 */
struct Triangle {
    std::array<Vertex, 3> vertices; ///< The triangle's three corner vertices.

    /**
     * @brief Constructs a triangle from three vertices.
     * @param v1 First vertex.
     * @param v2 Second vertex.
     * @param v3 Third vertex.
     */
    constexpr Triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) noexcept
        : vertices({v1, v2, v3}) {}
};

/**
 * @brief Builds a triangle from three positions sharing a single color.
 * @param a First corner position.
 * @param b Second corner position.
 * @param c Third corner position.
 * @param color Color applied to all three vertices.
 * @return The constructed triangle.
 */
inline Triangle makeTriangle(const Vector3& a, const Vector3& b, const Vector3& c, const Color& color) noexcept {
    return Triangle(
        Vertex(a, color),
        Vertex(b, color),
        Vertex(c, color)
    );
}

/**
 * @brief Splits a rectangle into the two triangles that cover it.
 * @param r Rectangle to triangulate.
 * @return An array of two triangles forming the rectangle.
 */
inline std::array<Triangle, 2> rectToTriangle(const Rectangle& r) noexcept {
    Vector3 A = r.pos;
    Vector3 B(r.pos.x + r.width, r.pos.y, r.pos.z);
    Vector3 C(B.x, r.pos.y + r.height, r.pos.z);
    Vector3 D(r.pos.x, C.y, r.pos.z);

    return {
        makeTriangle(A, B, C, r.color),
        makeTriangle(A, C, D, r.color)
    };
}

}
