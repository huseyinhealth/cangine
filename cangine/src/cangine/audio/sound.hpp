#pragma once

#include <string>

namespace cangine {
    /**
     * @brief Describes an audio source: its file path, name and volume.
     *
     * Used by @ref AudioManager to load and reference playable sounds.
     */
    struct Sound {
        std::string name;    ///< Identifier used to reference the sound.
        std::string path;    ///< Path to the audio file.
        float volume = 1.0f; ///< Playback volume (0.0 = silent, 1.0 = full).

        /**
         * @brief Constructs a sound with an explicit path and name.
         * @param path Path to the audio file.
         * @param name Identifier for the sound.
         */
        Sound(std::string path, std::string name) : path(path), name(name) {}
        /**
         * @brief Constructs a sound from a path, deriving the name from it.
         * @param path Path to the audio file.
         */
        Sound(std::string path);
    };
}
