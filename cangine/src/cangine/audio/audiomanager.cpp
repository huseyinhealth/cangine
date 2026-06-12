#define MINIAUDIO_IMPLEMENTATION
#include "audiomanager.hpp"

namespace cangine {
    AudioManager* AudioManager::instance = nullptr;

    // ── constructor / destructor ──────────────────────────────────────────────

    AudioManager::AudioManager() {
        assert(instance == nullptr);
        ma_result result = ma_engine_init(nullptr, &engine);
        if (result == MA_SUCCESS) {
            this->initialized = true;
            std::cout << "AudioManager: initialized.\n";
        } else {
            std::cout << "AudioManager: failed to initialize audio engine.\n";
        }
        instance = this;
    }

    AudioManager::~AudioManager() {
        stopAll();
        if (initialized)
            ma_engine_uninit(&engine);
        instance = nullptr;
    }

    AudioManager* AudioManager::getInstance() {
        return instance;
    }

    // ── internal ─────────────────────────────────────────────────────────────

    void AudioManager::cleanupFinishedSounds() {
        for (auto it = activeSounds.begin(); it != activeSounds.end();) {
            ma_sound* s = it->first;
            if (!ma_sound_is_playing(s) && ma_sound_at_end(s)) {
                ma_sound_uninit(s);
                delete s;
                it = activeSounds.erase(it);
            } else {
                ++it;
            }
        }
    }

    // ── add ──────────────────────────────────────────────────────────────────

    void AudioManager::add(const Sound& sound) {
        for (auto& s : sounds) {
            if (s.name == sound.name) {
                std::cout << "AudioManager: sound '" << sound.name << "' already exists.\n";
                return;
            }
        }
        sounds.push_back(sound);
    }

    void AudioManager::add(const std::string& path) {
        add(Sound(path));
    }

    void AudioManager::add(const std::string& path, const std::string& name) {
        add(Sound(path, name));
    }

    // ── play ─────────────────────────────────────────────────────────────────

    void AudioManager::play(const std::string& name, bool loop) {
        for (auto& s : sounds) {
            if (s.name == name) {
                play(s, loop);
                return;
            }
        }
        std::cout << "AudioManager: sound not found: " << name << "\n";
    }

    void AudioManager::play(const Sound& sound, bool loop) {
        if (!initialized) return;

        cleanupFinishedSounds();

        ma_sound* s = new ma_sound();
        ma_result result = ma_sound_init_from_file(&engine, sound.path.c_str(), 0, nullptr, nullptr, s);
        if (result != MA_SUCCESS) {
            std::cout << "AudioManager: failed to load: " << sound.path << "\n";
            delete s;
            return;
        }
        ma_sound_set_volume(s, sound.volume);
        ma_sound_set_looping(s, loop ? MA_TRUE : MA_FALSE);
        ma_sound_start(s);
        activeSounds.push_back({ s, sound.name });
    }

    // ── stop ─────────────────────────────────────────────────────────────────

    void AudioManager::stop(const std::string& name) {
        for (auto it = activeSounds.begin(); it != activeSounds.end();) {
            if (it->second == name) {
                ma_sound_stop(it->first);
                ma_sound_uninit(it->first);
                delete it->first;
                it = activeSounds.erase(it);
            } else {
                ++it;
            }
        }
    }

    void AudioManager::stopAll() {
        for (auto& [s, n] : activeSounds) {
            ma_sound_stop(s);
            ma_sound_uninit(s);
            delete s;
        }
        activeSounds.clear();
    }

    // ── pause / resume ────────────────────────────────────────────────────────

    void AudioManager::pause(const std::string& name) {
        for (auto& [s, n] : activeSounds)
            if (n == name) ma_sound_stop(s);
    }

    void AudioManager::pauseAll() {
        for (auto& [s, n] : activeSounds)
            ma_sound_stop(s);
    }

    void AudioManager::resume(const std::string& name) {
        for (auto& [s, n] : activeSounds)
            if (n == name) ma_sound_start(s);
    }

    void AudioManager::resumeAll() {
        for (auto& [s, n] : activeSounds)
            ma_sound_start(s);
    }

    // ── volume ────────────────────────────────────────────────────────────────

    void AudioManager::setVolume(const std::string& name, float volume) {
        for (auto& [s, n] : activeSounds)
            if (n == name) ma_sound_set_volume(s, volume);
    }

    void AudioManager::setVolumeAll(float volume) {
        for (auto& [s, n] : activeSounds)
            ma_sound_set_volume(s, volume);
    }

    // ── query ─────────────────────────────────────────────────────────────────

    bool AudioManager::isPlaying(const std::string& name) {
        for (auto& [s, n] : activeSounds)
            if (n == name) return ma_sound_is_playing(s);
        return false;
    }

    bool AudioManager::isPaused(const std::string& name) {
        for (auto& [s, n] : activeSounds)
            if (n == name) return !ma_sound_is_playing(s) && !ma_sound_at_end(s);
        return false;
    }

    float AudioManager::getDuration(const std::string& name) {
        for (auto& [s, n] : activeSounds) {
            if (n == name) {
                float duration = 0.0f;
                ma_sound_get_length_in_seconds(s, &duration);
                return duration;
            }
        }
        return 0.0f;
    }

    float AudioManager::getPosition(const std::string& name) {
        for (auto& [s, n] : activeSounds) {
            if (n == name) {
                float cursor = 0.0f;
                ma_sound_get_cursor_in_seconds(s, &cursor);
                return cursor;
            }
        }
        return 0.0f;
    }

    void AudioManager::seek(const std::string& name, float seconds) {
        for (auto& [s, n] : activeSounds) {
            if (n == name) {
                ma_uint64 frame = (ma_uint64)(seconds * ma_engine_get_sample_rate(&engine));
                ma_sound_seek_to_pcm_frame(s, frame);
                return;
            }
        }
    }
}
