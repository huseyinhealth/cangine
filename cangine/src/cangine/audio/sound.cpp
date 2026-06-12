#include "sound.hpp"

#include <filesystem>

namespace cangine {
    Sound::Sound(std::string path) {
        std::string name = std::filesystem::path(path).filename().string();
        Sound(path, name);
    }
}
