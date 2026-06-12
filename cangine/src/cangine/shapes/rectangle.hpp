#pragma once

#include <cmath>
#include <array>
#include <cfloat>

#include "../types/vector2.hpp"
#include "../types/vector3.hpp"
#include "../util/color.hpp"

#include "../engine/components/transform_component.hpp"

namespace cangine {
    /**
     * @brief An axis-aligned rectangle with a position, size and color.
     *
     * The geometry is axis-aligned in its own space, but the static helpers
     * accept a @ref Transform so they can perform rotated/scaled hit-tests and
     * collision checks via the Separating Axis Theorem.
     */
    struct Rectangle {
        Vector3 pos;  ///< Center position (z is the layer).
        float width;  ///< Width before scaling.
        float height; ///< Height before scaling.
        Color color;  ///< Fill color.

        /**
         * @brief Tests whether a point lies inside a (possibly transformed) rectangle.
         * @param rect Rectangle whose width/height define the bounds.
         * @param transform Transform supplying position, rotation and scale.
         * @param point Point to test, in world/screen space.
         * @return @c true if the point is inside the rectangle, @c false otherwise.
         */
        inline static bool containsPoint(const Rectangle& rect, const Transform& transform, const Vector2& point) {
            Vector3 pos = transform.getPosition();
            Vector3 rot = transform.getRotation();
            Vector3 scl = transform.getScale();
            
            // transform point into rect's local space
            float dx = point.x - pos.x;
            float dy = point.y - pos.y;
            
            float radians = -rot.z * 3.14159265f / 180.0f;  // inverse rotation
            float c = std::cos(radians);
            float s = std::sin(radians);
            
            float localX = dx * c - dy * s;
            float localY = dx * s + dy * c;
            
            float hw = (rect.width  * scl.x) / 2.0f;
            float hh = (rect.height * scl.y) / 2.0f;
            
            return (localX >= -hw && localX <= hw) &&
                (localY >= -hh && localY <= hh);
        }

        /**
         * @brief Tests whether two (possibly transformed) rectangles overlap.
         *
         * Uses the Separating Axis Theorem over the four edge normals of the
         * two rectangles, so rotation and scale are respected.
         * @param r1 First rectangle (provides width/height).
         * @param t1 Transform for the first rectangle.
         * @param r2 Second rectangle (provides width/height).
         * @param t2 Transform for the second rectangle.
         * @return @c true if the rectangles intersect, @c false otherwise.
         */
        inline static bool overlaps(const Rectangle& r1, const Transform& t1,
                                    const Rectangle& r2, const Transform& t2) {

            // lambda that returns the 4 corners of a rect
            auto getCorners = [](const Rectangle& r, const Transform& t) {
                Vector3 pos = t.getPosition();
                Vector3 rot = t.getRotation();
                Vector3 scl = t.getScale();

                float hw = (r.width  * scl.x) / 2.0f;
                float hh = (r.height * scl.y) / 2.0f;
                float rad = rot.z * 3.14159265f / 180.0f;
                float c = std::cos(rad), s = std::sin(rad);

                auto rotate = [&](float x, float y) -> Vector2 {
                    return { pos.x + x*c - y*s, pos.y + x*s + y*c };
                };

                return std::array<Vector2, 4>{
                    rotate(-hw, -hh), rotate( hw, -hh),
                    rotate( hw,  hh), rotate(-hw,  hh)
                };
            };

            auto corners1 = getCorners(r1, t1);
            auto corners2 = getCorners(r2, t2);

            // SAT: project all corners onto the given axis and check for overlap
            auto testAxis = [](const Vector2& axis,
                            const std::array<Vector2, 4>& c1,
                            const std::array<Vector2, 4>& c2) -> bool {
                float min1 = FLT_MAX, max1 = -FLT_MAX;
                float min2 = FLT_MAX, max2 = -FLT_MAX;

                for (auto& v : c1) {
                    float p = v.x * axis.x + v.y * axis.y;
                    min1 = std::min(min1, p); max1 = std::max(max1, p);
                }
                for (auto& v : c2) {
                    float p = v.x * axis.x + v.y * axis.y;
                    min2 = std::min(min2, p); max2 = std::max(max2, p);
                }

                return max1 >= min2 && max2 >= min1;  // true if there is overlap
            };

            // 2 axes from r1 + 2 axes from r2 = 4 axes
            Vector3 rot1 = t1.getRotation(), rot2 = t2.getRotation();
            float rad1 = rot1.z * 3.14159265f / 180.0f;
            float rad2 = rot2.z * 3.14159265f / 180.0f;

            std::array<Vector2, 4> axes = {{
                { std::cos(rad1), std::sin(rad1) },   // r1 x axis
                {-std::sin(rad1), std::cos(rad1) },   // r1 y axis
                { std::cos(rad2), std::sin(rad2) },   // r2 x axis
                {-std::sin(rad2), std::cos(rad2) }    // r2 y axis
            }};

            for (auto& axis : axes) {
                if (!testAxis(axis, corners1, corners2))
                    return false;  // separating axis found, no collision
            }

            return true;  // all axes overlap, collision confirmed
        }
    };

    /**
     * @brief Point-in-rectangle test (axis-aligned, untransformed).
     * @param vec Point to test.
     * @param rect Axis-aligned rectangle centered on @c rect.pos.
     * @return @c true if the point is within the rectangle bounds.
     */
    constexpr inline bool operator>>(const Vector2& vec, const Rectangle& rect) noexcept {
        float halfW = rect.width  / 2.0f;
        float halfH = rect.height / 2.0f;
        return (vec.x >= rect.pos.x - halfW && vec.x <= rect.pos.x + halfW) &&
            (vec.y >= rect.pos.y - halfH && vec.y <= rect.pos.y + halfH);
    }

    /**
     * @brief Point-in-rectangle test with reversed operand order.
     * @param rect Axis-aligned rectangle.
     * @param vec Point to test.
     * @return @c true if the point is within the rectangle bounds.
     */
    constexpr inline bool operator>>(const Rectangle& rect, const Vector2& vec) noexcept { return vec >> rect; };

    /**
     * @brief Axis-aligned overlap test between two rectangles.
     * @param r1 First rectangle.
     * @param r2 Second rectangle.
     * @return @c true if the rectangles overlap (ignoring rotation/scale).
     */
    constexpr inline bool operator>>(const Rectangle& r1, const Rectangle& r2) noexcept {
        return (std::abs(r1.pos.x - r2.pos.x) < (r1.width  + r2.width)  / 2.0f) &&
            (std::abs(r1.pos.y - r2.pos.y) < (r1.height + r2.height) / 2.0f);
    }

}
