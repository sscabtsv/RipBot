#pragma once
#include <vector>
#include <filesystem>

struct MacroEvent {
    int frame = 0;
    double timestamp = 0.0;
    int button = 0;
    bool player2 = false;
    bool push = false;
};

class MacroManager {
private:
    bool m_isRecording = false;
    bool m_isPlaying = false;

    MacroManager() = default;

public:
    static MacroManager* get();

    int currentFrame = 0;
    size_t playbackIndex = 0;

    std::vector<MacroEvent> events;

    bool isRecording() const;
    void setRecording(bool state);

    bool isPlaying() const;
    void setPlaying(bool state);

    void addEvent(int button, bool player2, bool push, double timestamp);

    // Clears events and resets playback/record cursors. Called internally by setRecording(true), but exposed so PlayLayer can also call it on resetLevel() to fix the death/respawn desync bug.
    void reset();

    bool saveToFile(std::filesystem::path const& path) const;
    bool loadFromFile(std::filesystem::path const& path);
};
