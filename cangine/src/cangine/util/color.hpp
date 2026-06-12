#pragma once

#include <cassert>

namespace cangine {
	/**
	 * @brief RGBA color stored as normalized floating-point components.
	 *
	 * RGB channels are stored in the range [0, 1] (constructors accept 0-255
	 * values and divide by 255), while alpha is stored directly in [0, 1].
	 */
	struct Color {
		float r = 0; ///< Red channel, normalized to [0, 1].
		float g = 0; ///< Green channel, normalized to [0, 1].
		float b = 0; ///< Blue channel, normalized to [0, 1].
		float a = 0; ///< Alpha channel in [0, 1].

		/// @brief Constructs an opaque white color.
		inline Color() : Color(255, 255, 255, 1) {};

		/**
		 * @brief Constructs a color from 0-255 RGB values and an alpha.
		 * @param r Red channel in the range [0, 255].
		 * @param g Green channel in the range [0, 255].
		 * @param b Blue channel in the range [0, 255].
		 * @param a Alpha in the range [0, 1] (asserted).
		 */
		inline Color(float r, float g, float b, float a) {
			assert(a >= 0.0f && a <= 1.0f);

			this->r = r / 255.0f;
			this->g = g / 255.0f;
			this->b = b / 255.0f;
			this->a = a;
		};
		/**
		 * @brief Constructs a fully opaque color from 0-255 RGB values.
		 * @param r Red channel in the range [0, 255].
		 * @param g Green channel in the range [0, 255].
		 * @param b Blue channel in the range [0, 255].
		 */
		inline Color(float r, float g, float b) : Color(r, g, b, 1.0f) {};

	};

	/// @brief Predefined commonly used opaque colors.
	namespace Colors {
		inline const Color BLACK(0, 0, 0);     ///< Black.
		inline const Color WHITE(255, 255, 255); ///< White.
		inline const Color GRAY(128, 128, 128);  ///< Medium gray.
		inline const Color DARK_GRAY(99, 99, 99); ///< Dark gray.
		inline const Color RED(255, 0, 0);     ///< Pure red.
		inline const Color GREEN(0, 255, 0);   ///< Pure green.
		inline const Color BLUE(0, 0, 255);    ///< Pure blue.
	}
}
