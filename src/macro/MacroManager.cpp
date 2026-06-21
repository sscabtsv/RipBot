#pragma once

class MacroManager {
private:
    bool m_isRecording = false;
    bool m_isPlaying = false;

    MacroManager() = default;

public:
    static MacroManager* get();

    bool isRecording() const;
    void setRecording(bool state);

    bool isPlaying() const;
    void setPlaying(bool state);
};
