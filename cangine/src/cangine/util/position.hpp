#pragma once

#include "../types/types.hpp"
#include "../shapes/shapes.hpp"

/// @brief Maximum z-layer value used when normalizing depth into clip space.
static constexpr float POSITION_MAX_LAYER = 100.0f;

namespace cangine {
	class Window;
	/**
	 * @brief Helper for converting screen-space positions into normalized
	 *        device coordinates.
	 *
	 * Normalization maps pixel coordinates (relative to the active window) into
	 * the [-1, 1] clip-space range expected by the renderer.
	 */
	class Position {
	public:
		static Window* window_instance; ///< Window used as the reference frame for normalization.
		/**
		 * @brief Normalizes a 3D position into clip space.
		 * @param v Position in screen/pixel space (z is the layer).
		 * @return The position in normalized device coordinates.
		 */
		static Vector3 normalize(const Vector3 &v);
		/**
		 * @brief Normalizes a 2D position into clip space.
		 * @param v Position in screen/pixel space.
		 * @return The position in normalized device coordinates.
		 */
		static Vector2 normalize(const Vector2 &v);
		/**
		 * @brief Normalizes a vertex's position into clip space.
		 * @param v Vertex whose position is in screen/pixel space.
		 * @return A copy of the vertex with its position normalized.
		 */
		static Vertex normalize(const Vertex &v);
	};
}
