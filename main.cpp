#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "macro/MacroManager.hpp"

using namespace geode::prelude;

class $modify(RipBotPlayLayer, PlayLayer) {

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects))
            return false;

        auto m = MacroManager::get();
        m->currentFrame = 0;
        m->playbackIndex = 0;

        return true;
    }

    // Fixes a desync bug: without this, currentFrame/playbackIndex only reset on level *init*, so any death/respawn during recording on playback would drift out of sync with the rest of the run.
    void resetLevel() {
        PlayLayer::resetLevel();

        auto m = MacroManager::get();
        m->currentFrame = 0;
        m->playbackIndex = 0;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        auto m = MacroManager::get();
        m->currentFrame++;

        if (!m->isPlaying()) return;

        while (m->playbackIndex < m->events.size()) {
            auto& e = m->events[m->playbackIndex];

            if (e.frame > m->currentFrame)
                break;

            if (e.frame == m->currentFrame) {
                // Base-qualified call bypasses our own queueButton hook below, so played-back inputs don't get re-recorded.
                PlayLayer::queueButton(e.button, e.push, e.player2, e.timestamp);
            }

            m->playbackIndex++;
        }
    }

    // GD 2.208 added Click Between Frames, which is why queueButton now carries a sub-frame `timestamp`. Hooking this one function (instead of separate pushButton/releaseButton) captures both press/release and CBF timing in a single, version-correct place.
    void queueButton(int button, bool push, bool isPlayer2, double timestamp) {
        auto m = MacroManager::get();

        if (m->isRecording()) {
            m->addEvent(button, isPlayer2, push, timestamp);
        }

        PlayLayer::queueButton(button, push, isPlayer2, timestamp);
    }
};

$on_mod(Loaded) {
    log::info("RipBot Loaded!");
}
