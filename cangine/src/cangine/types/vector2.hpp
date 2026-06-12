#pragma once
//#include <type_traits>

namespace cangine {

/**
 * @brief Two-component floating-point vector.
 *
 * Provides component-wise arithmetic against both other vectors and scalars.
 * All operations are @c constexpr and @c noexcept so the type can be used in
 * compile-time contexts.
 */
struct Vector2 {
    float x{}, y{}; ///< Vector components.

    // Constructors
    /// @brief Constructs a zero vector (x = y = 0).
    constexpr Vector2() noexcept = default;
    /**
     * @brief Constructs a vector from explicit components.
     * @param _x Value for the x component.
     * @param _y Value for the y component.
     */
    constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}

    // Compound assignment operators
    /**
     * @brief Adds another vector component-wise.
     * @param rhs Vector to add.
     * @return Reference to this vector after addition.
     */
    constexpr Vector2& operator+=(const Vector2& rhs) noexcept {
        x += rhs.x; y += rhs.y; return *this;
    }
    /**
     * @brief Subtracts another vector component-wise.
     * @param rhs Vector to subtract.
     * @return Reference to this vector after subtraction.
     */
    constexpr Vector2& operator-=(const Vector2& rhs) noexcept {
        x -= rhs.x; y -= rhs.y; return *this;
    }
    /**
     * @brief Multiplies by another vector component-wise.
     * @param rhs Vector to multiply by.
     * @return Reference to this vector after multiplication.
     */
    constexpr Vector2& operator*=(const Vector2& rhs) noexcept {
        x *= rhs.x; y *= rhs.y; return *this;
    }
    /**
     * @brief Divides by another vector component-wise.
     * @param rhs Vector to divide by.
     * @return Reference to this vector after division.
     */
    constexpr Vector2& operator/=(const Vector2& rhs) noexcept {
        x /= rhs.x; y /= rhs.y; return *this;
    }

    /**
     * @brief Adds a scalar to both components.
     * @param rhs Scalar to add.
     * @return Reference to this vector after addition.
     */
    constexpr Vector2& operator+=(float rhs) noexcept { x += rhs; y += rhs; return *this; }
    /**
     * @brief Subtracts a scalar from both components.
     * @param rhs Scalar to subtract.
     * @return Reference to this vector after subtraction.
     */
    constexpr Vector2& operator-=(float rhs) noexcept { x -= rhs; y -= rhs; return *this; }
    /**
     * @brief Multiplies both components by a scalar.
     * @param rhs Scalar to multiply by.
     * @return Reference to this vector after multiplication.
     */
    constexpr Vector2& operator*=(float rhs) noexcept { x *= rhs; y *= rhs; return *this; }
    /**
     * @brief Divides both components by a scalar.
     * @param rhs Scalar to divide by.
     * @return Reference to this vector after division.
     */
    constexpr Vector2& operator/=(float rhs) noexcept { x /= rhs; y /= rhs; return *this; }
};

/**
 * @brief Defines the free binary operator @p OP for Vector2.
 *
 * Expands to three overloads: vector-vector, vector-scalar and scalar-vector,
 * each producing a new component-wise result.
 * @param OP The arithmetic operator token (+, -, * or /).
 */
// Free operators for Vector2
#define CANGINE_VECTOR2_OP(OP) \
constexpr Vector2 operator OP(const Vector2& lhs, const Vector2& rhs) noexcept { return Vector2(lhs.x OP rhs.x, lhs.y OP rhs.y); } \
constexpr Vector2 operator OP(const Vector2& lhs, float rhs) noexcept { return Vector2(lhs.x OP rhs, lhs.y OP rhs); } \
constexpr Vector2 operator OP(float lhs, const Vector2& rhs) noexcept { return Vector2(lhs OP rhs.x, lhs OP rhs.y); }

CANGINE_VECTOR2_OP(+)
CANGINE_VECTOR2_OP(-)
CANGINE_VECTOR2_OP(*)
CANGINE_VECTOR2_OP(/)

#undef CANGINE_VECTOR2_OP

}
