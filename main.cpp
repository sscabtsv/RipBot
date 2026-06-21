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

    void update(float dt) {
        PlayLayer::update(dt);

        auto m = MacroManager::get();

        m->currentFrame++;

        if (m->isPlaying()) {
            while (m->playbackIndex < m->events.size()) {

                auto& e = m->events[m->playbackIndex];

                if (e.frame > m->currentFrame)
                    break;

                if (e.frame == m->currentFrame) {
                    if (e.push)
                        PlayLayer::pushButton(e.button, e.player2);
                    else
                        PlayLayer::releaseButton(e.button, e.player2);
                }

                m->playbackIndex++;
            }
        }
    }

    void pushButton(int button, bool player2) {
        PlayLayer::pushButton(button, player2);

        auto m = MacroManager::get();
        if (!m->isRecording()) return;

        m->addEvent(button, player2, true);
    }

    void releaseButton(int button, bool player2) {
        PlayLayer::releaseButton(button, player2);

        auto m = MacroManager::get();
        if (!m->isRecording()) return;

        m->addEvent(button, player2, false);
    }
};

$on_mod(Loaded) {
    log::info("RipBot Loaded!");
}
