#pragma once
#include <vector>
#include <unordered_map>

struct MacroEvent {
    int frame;
    int button;
    bool player2;
    bool push;
};

class MacroManager {
private:
    bool m_isRecording = false;
    bool m_isPlaying = false;

    MacroManager() = default;

public:
    static MacroManager* get();

    int currentFrame = 0;
    double timeAccumulator = 0.0;
    size_t playbackIndex = 0;
    std::vector<MacroEvent> events;

    std::unordered_map<int, bool> p1State;
    std::unordered_map<int, bool> p2State;

    bool isRecording() const;
    void setRecording(bool state);

    bool isPlaying() const;
    void setPlaying(bool state);

    void addEvent(int button, bool player2, bool push);
};
