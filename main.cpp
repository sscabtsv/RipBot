#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "ui/MainUI.hpp"
#include "macro/MacroManager.hpp"

using namespace geode::prelude;

class $modify(RipBotPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }
        
        auto manager = MacroManager::get();
        manager->currentFrame = 0;
        manager->timeAccumulator = 0.0;
        manager->playbackIndex = 0;
        
        return true;
    }

    void update(float dt) {
        auto manager = MacroManager::get();
        const double fixedDelta = 1.0 / 240.0; // Fixed 240fps deterministic step
        
        manager->timeAccumulator += dt;

        while (manager->timeAccumulator >= fixedDelta) {
            manager->currentFrame++;
            
            if (manager->isPlaying()) {
                while (manager->playbackIndex < manager->events.size()) {
                    const auto& event = manager->events[manager->playbackIndex];
                    
                    if (event.frame == manager->currentFrame) {
                        if (event.push) {
                            this->PlayLayer::pushButton(event.button, event.player2);
                        } else {
                            this->PlayLayer::releaseButton(event.button, event.player2);
                        }
                        manager->playbackIndex++;
                    } else if (event.frame > manager->currentFrame) {
                        break; // Future event, wait for next frames
                    } else {
                        manager->playbackIndex++; // Skip missed/past events to catch up
                    }
                }
            }
            manager->timeAccumulator -= fixedDelta;
        }

        PlayLayer::update(dt);
    }

    void pushButton(int button, bool player2) {
        this->PlayLayer::pushButton(button, player2);
        
        auto manager = MacroManager::get();
        if (manager->isRecording()) {
            manager->addEvent(button, player2, true);
        }
    }

    void releaseButton(int button, bool player2) {
        this->PlayLayer::releaseButton(button, player2);
        
        auto manager = MacroManager::get();
        if (manager->isRecording()) {
            manager->addEvent(button, player2, false);
        }
    }
};

$on_mod(Loaded) {
    log::info("RipBot Loaded!");
}
