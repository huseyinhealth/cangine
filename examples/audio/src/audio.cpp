#include "audio.hpp"
#include <cangine/engine/gameobjects/ui/button_object.hpp>
#include <cangine/engine/gameobjects/ui/label_object.hpp>
#include <algorithm>

using namespace cangine;

class AudioExample : public CangineApp {
public:
    AudioExample() : CangineApp(800, 600, "cangine :: Audio Example") {}

    Label* statusLabel  = nullptr;
    Label* posLabel     = nullptr;
    Label* durationLabel = nullptr;
    float volume = 1.0f;

    void onFirstFrame() override {
        audioMan->add("./exhilarate.mp3", "exhilarate");
        audioMan->play("exhilarate", true);

        // labels ////////////////////////////////////////////////////////////

        statusLabel = instantiateObject<Label>("Status: Playing");
        Transform::posFor(statusLabel, Vector3(statusLabel->getSize().x / -2, 150, 80));

        posLabel = instantiateObject<Label>("Position: 0s");
        Transform::posFor(posLabel, Vector3(posLabel->getSize().x / -2, 125, 80));

        durationLabel = instantiateObject<Label>("Duration: " + std::to_string((int)audioMan->getDuration("exhilarate")) + "s");
        Transform::posFor(durationLabel, Vector3(durationLabel->getSize().x / -2, 100, 80));

        // pause / resume ////////////////////////////////////////////////////////////

        Button* btnPause = instantiateObject<Button>("Pause / Resume");
        Transform::posFor(btnPause, Vector3(btnPause->getSize().x / -2, 50, 80));
        btnPause->registerOnClickEvent([&](const MouseEvent* e) {
            if (audioMan->isPlaying("exhilarate")) {
                audioMan->pause("exhilarate");
                statusLabel->setText("Status: Paused");
            } else if (audioMan->isPaused("exhilarate")) {
                audioMan->resume("exhilarate");
                statusLabel->setText("Status: Playing");
            }
            Transform::posFor(statusLabel, Vector3(statusLabel->getSize().x / -2, 150, 80));
        });

        // stop ////////////////////////////////////////////////////////////

        Button* btnStop = instantiateObject<Button>("Stop");
        Transform::posFor(btnStop, Vector3(btnStop->getSize().x / -2, 10, 80));
        btnStop->registerOnClickEvent([&](const MouseEvent* e) {
            audioMan->stop("exhilarate");
            statusLabel->setText("Status: Stopped");
            Transform::posFor(statusLabel, Vector3(statusLabel->getSize().x / -2, 150, 80));
        });

        // volume + ////////////////////////////////////////////////////////////

        Button* btnVolUp = instantiateObject<Button>("Volume +");
        Transform::posFor(btnVolUp, Vector3(btnVolUp->getSize().x / -2 + 120, -40, 80));
        btnVolUp->registerOnClickEvent([&](const MouseEvent* e) {
            volume = std::min(volume + 0.1f, 2.0f);
            audioMan->setVolume("exhilarate", volume);
            statusLabel->setText("Volume: " + std::to_string((int)(volume * 100)) + "%");
            Transform::posFor(statusLabel, Vector3(statusLabel->getSize().x / -2, 150, 80));
        });

        // volume - ////////////////////////////////////////////////////////////

        Button* btnVolDown = instantiateObject<Button>("Volume -");
        Transform::posFor(btnVolDown, Vector3(btnVolDown->getSize().x / -2 - 120, -40, 80));
        btnVolDown->registerOnClickEvent([&](const MouseEvent* e) {
            volume = std::max(volume - 0.1f, 0.0f);
            audioMan->setVolume("exhilarate", volume);
            statusLabel->setText("Volume: " + std::to_string((int)(volume * 100)) + "%");
            Transform::posFor(statusLabel, Vector3(statusLabel->getSize().x / -2, 150, 80));
        });

        // seek -10s ////////////////////////////////////////////////////////////

        Button* btnSeekBack = instantiateObject<Button>("<< -10s");
        Transform::posFor(btnSeekBack, Vector3(btnSeekBack->getSize().x / -2 - 120, -90, 80));
        btnSeekBack->registerOnClickEvent([&](const MouseEvent* e) {
            float pos = std::max(audioMan->getPosition("exhilarate") - 10.0f, 0.0f);
            audioMan->seek("exhilarate", pos);
        });

        // seek +10s ////////////////////////////////////////////////////////////

        Button* btnSeekFwd = instantiateObject<Button>(">> +10s");
        Transform::posFor(btnSeekFwd, Vector3(btnSeekFwd->getSize().x / -2 + 120, -90, 80));
        btnSeekFwd->registerOnClickEvent([&](const MouseEvent* e) {
            float dur = audioMan->getDuration("exhilarate");
            float pos = std::min(audioMan->getPosition("exhilarate") + 10.0f, dur);
            audioMan->seek("exhilarate", pos);
        });
    }

    void onFrame(double dt) override {
        float pos = audioMan->getPosition("exhilarate");
        posLabel->setText("Position: " + std::to_string((int)pos) + "s");
        Transform::posFor(posLabel, Vector3(posLabel->getSize().x / -2, 125, 80));
    }
};

int main() {
    AudioExample().run();
    return 0;
}
