#pragma once
#include "vector2.hpp"

namespace cangine {

/**
 * @brief Three-component floating-point vector.
 *
 * Provides component-wise arithmetic against both other vectors and scalars,
 * plus implicit construction from a @ref Vector2. All operations are
 * @c constexpr and @c noexcept.
 */
struct Vector3 {
    float x{}, y{}, z{}; ///< Vector components.

    // Constructors
    /// @brief Constructs a zero vector (x = y = z = 0).
    constexpr Vector3() noexcept = default;
    /**
     * @brief Constructs a vector from explicit components.
     * @param _x Value for the x component.
     * @param _y Value for the y component.
     * @param _z Value for the z component.
     */
    constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
    /**
     * @brief Constructs a vector from a 2D vector and an explicit z.
     * @param xy Source for the x and y components.
     * @param _z Value for the z component.
     */
    constexpr Vector3(const Vector2& xy, float _z) noexcept : x(xy.x), y(xy.y), z(_z) {}
    /**
     * @brief Constructs a vector from a 2D vector with z defaulting to 0.
     * @param xy Source for the x and y components.
     */
    constexpr Vector3(const Vector2& xy) noexcept : x(xy.x), y(xy.y), z(0) {}

    // Compound assignment operators (Vector3 op Vector3)
    /**
     * @brief Adds another vector component-wise.
     * @param rhs Vector to add.
     * @return Reference to this vector after addition.
     */
    constexpr Vector3& operator+=(const Vector3& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    /**
     * @brief Subtracts another vector component-wise.
     * @param rhs Vector to subtract.
     * @return Reference to this vector after subtraction.
     */
    constexpr Vector3& operator-=(const Vector3& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    /**
     * @brief Multiplies by another vector component-wise.
     * @param rhs Vector to multiply by.
     * @return Reference to this vector after multiplication.
     */
    constexpr Vector3& operator*=(const Vector3& rhs) noexcept { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
    /**
     * @brief Divides by another vector component-wise.
     * @param rhs Vector to divide by.
     * @return Reference to this vector after division.
     */
    constexpr Vector3& operator/=(const Vector3& rhs) noexcept { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

    // Compound assignment operators (Vector3 op scalar)
    /**
     * @brief Adds a scalar to all components.
     * @param rhs Scalar to add.
     * @return Reference to this vector after addition.
     */
    constexpr Vector3& operator+=(float rhs) noexcept { x += rhs; y += rhs; z += rhs; return *this; }
    /**
     * @brief Subtracts a scalar from all components.
     * @param rhs Scalar to subtract.
     * @return Reference to this vector after subtraction.
     */
    constexpr Vector3& operator-=(float rhs) noexcept { x -= rhs; y -= rhs; z -= rhs; return *this; }
    /**
     * @brief Multiplies all components by a scalar.
     * @param rhs Scalar to multiply by.
     * @return Reference to this vector after multiplication.
     */
    constexpr Vector3& operator*=(float rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }
    /**
     * @brief Divides all components by a scalar.
     * @param rhs Scalar to divide by.
     * @return Reference to this vector after division.
     */
    constexpr Vector3& operator/=(float rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }
};

/**
 * @brief Defines the free binary operator @p OP for Vector3.
 *
 * Expands to three overloads: vector-vector, vector-scalar and scalar-vector,
 * each producing a new component-wise result.
 * @param OP The arithmetic operator token (+, -, * or /).
 */
// Free operators for Vector3
#define CANGINE_VECTOR3_OP(OP) \
constexpr Vector3 operator OP(const Vector3& lhs, const Vector3& rhs) noexcept { return Vector3(lhs.x OP rhs.x, lhs.y OP rhs.y, lhs.z OP rhs.z); } \
constexpr Vector3 operator OP(const Vector3& lhs, float rhs) noexcept { return Vector3(lhs.x OP rhs, lhs.y OP rhs, lhs.z OP rhs); } \
constexpr Vector3 operator OP(float lhs, const Vector3& rhs) noexcept { return Vector3(lhs OP rhs.x, lhs OP rhs.y, lhs OP rhs.z); }

CANGINE_VECTOR3_OP(+)
CANGINE_VECTOR3_OP(-)
CANGINE_VECTOR3_OP(*)
CANGINE_VECTOR3_OP(/)

#undef CANGINE_VECTOR3_OP

} // namespace cangine
