#include "MacroManager.hpp"

MacroManager* MacroManager::get() {
    static MacroManager instance;
    return &instance;
}

bool MacroManager::isRecording() const { return m_isRecording; }
void MacroManager::setRecording(bool state) { 
    m_isRecording = state; 
    if (state) {
        events.clear();
        currentFrame = 0;
        timeAccumulator = 0.0;
        p1State.clear();
        p2State.clear();
    }
}

bool MacroManager::isPlaying() const { return m_isPlaying; }
void MacroManager::setPlaying(bool state) { 
    m_isPlaying = state; 
    if (state) {
        playbackIndex = 0;
        currentFrame = 0;
        timeAccumulator = 0.0;
        p1State.clear();
        p2State.clear();
    }
}

void MacroManager::addEvent(int button, bool player2, bool push) {
    if (!m_isRecording) return;

    auto& stateMap = player2 ? p2State : p1State;

    // Edge detection: Only store state transitions
    if (stateMap.find(button) != stateMap.end() && stateMap[button] == push) {
        return;
    }
    stateMap[button] = push;

    // Prevent duplicate events on the exact same frame
    if (!events.empty()) {
        const auto& lastEvent = events.back();
        if (lastEvent.frame == currentFrame && lastEvent.button == button && 
            lastEvent.player2 == player2 && lastEvent.push == push) {
            return;
        }
    }

    events.push_back({currentFrame, button, player2, push});
}
