#pragma once

namespace cangine {
    /**
     * @brief Holds an integer width/height pair.
     *
     * Used to describe sizes in pixels such as window or texture dimensions.
     */
    struct Dimensions {
        int width;  ///< Width in pixels.
        int height; ///< Height in pixels.
    };
}
