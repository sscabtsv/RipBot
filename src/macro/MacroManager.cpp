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
        playbackIndex = 0;
    }
}

bool MacroManager::isPlaying() const { return m_isPlaying; }

void MacroManager::setPlaying(bool state) {
    m_isPlaying = state;

    if (state) {
        playbackIndex = 0;
        currentFrame = 0;
    }
}

void MacroManager::addEvent(int button, bool player2, bool push) {
    if (!m_isRecording) return;

    events.push_back({
        currentFrame,
        button,
        player2,
        push
    });
