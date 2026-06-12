#pragma once
#include <miniaudio.h>

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "sound.hpp"

namespace cangine {

    /**
     * @brief Singleton class that manages audio playback.
     *
     * Provides loading, playing, stopping, pausing and
     * volume control of audio sources via the miniaudio engine.
     */
    class AudioManager {
    private:
        std::vector<Sound> sounds;                                    ///< Loaded sound objects
        std::vector<std::pair<ma_sound*, std::string>> activeSounds;  ///< Currently active sounds (handle, name)
        ma_engine engine{};                                           ///< miniaudio engine instance
        bool initialized = false;                                     ///< Whether the engine has been initialized
        static AudioManager* instance;                                ///< Singleton instance

        /// @brief Removes sounds that have finished playing from memory
        void cleanupFinishedSounds();

    public:
        AudioManager();
        ~AudioManager();

        /**
         * @brief Returns the singleton instance of AudioManager.
         * @return Pointer to the AudioManager instance
         */
        static AudioManager* getInstance();

        /**
         * @brief Adds a Sound object to the manager.
         * @param sound Sound object to add
         */
        void add(const Sound& sound);

        /**
         * @brief Adds a sound from a file path, using the path as its name.
         * @param path Path to the audio file
         */
        void add(const std::string& path);

        /**
         * @brief Adds a sound from a file path and assigns a name to it.
         * @param path Path to the audio file
         * @param name Name to assign to the sound
         */
        void add(const std::string& path, const std::string& name);

        /**
         * @brief Plays a sound by name.
         * @param name Name of the sound to play
         * @param loop Whether to loop the sound
         */
        void play(const std::string& name, bool loop = false);

        /**
         * @brief Plays a Sound object directly.
         * @param sound Sound object to play
         * @param loop Whether to loop the sound
         */
        void play(const Sound& sound, bool loop = false);

        /**
         * @brief Stops a sound by name.
         * @param name Name of the sound to stop
         */
        void stop(const std::string& name);

        /// @brief Stops all currently playing sounds
        void stopAll();

        /**
         * @brief Pauses a sound by name.
         * @param name Name of the sound to pause
         */
        void pause(const std::string& name);

        /// @brief Pauses all currently playing sounds
        void pauseAll();

        /**
         * @brief Resumes a paused sound by name.
         * @param name Name of the sound to resume
         */
        void resume(const std::string& name);

        /// @brief Resumes all paused sounds
        void resumeAll();

        /**
         * @brief Sets the volume of a sound by name.
         * @param name Name of the sound
         * @param volume Volume level (0.0 = silent, 1.0 = full)
         */
        void setVolume(const std::string& name, float volume);

        /**
         * @brief Sets the volume of all sounds.
         * @param volume Volume level (0.0 = silent, 1.0 = full)
         */
        void setVolumeAll(float volume);

        /**
         * @brief Checks whether a sound is currently playing.
         * @param name Name of the sound
         * @return true if playing, false otherwise
         */
        bool isPlaying(const std::string& name);

        /**
         * @brief Checks whether a sound is currently paused.
         * @param name Name of the sound
         * @return true if paused, false otherwise
         */
        bool isPaused(const std::string& name);

        /**
         * @brief Returns the total duration of a sound.
         * @param name Name of the sound
         * @return Duration in seconds
         */
        float getDuration(const std::string& name);

        /**
         * @brief Returns the current playback position of a sound.
         * @param name Name of the sound
         * @return Position in seconds
         */
        float getPosition(const std::string& name);

        /**
         * @brief Seeks to a specific position in a sound.
         * @param name Name of the sound
         * @param seconds Target position in seconds
         */
        void seek(const std::string& name, float seconds);
    };

}