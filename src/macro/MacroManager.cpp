#include "MacroManager.hpp"

MacroManager* MacroManager::get() {
    static MacroManager instance;
    return &instance;
}

bool MacroManager::isRecording() const { return m_isRecording; }
void MacroManager::setRecording(bool state) { m_isRecording = state; }

bool MacroManager::isPlaying() const { return m_isPlaying; }
void MacroManager::setPlaying(bool state) { m_isPlaying = state; }
