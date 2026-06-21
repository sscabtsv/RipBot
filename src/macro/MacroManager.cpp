#include "MacroManager.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/file.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

MacroManager* MacroManager::get() {
    static MacroManager instance;
    return &instance;
}

bool MacroManager::isRecording() const { return m_isRecording; }

void MacroManager::setRecording(bool state) {
    m_isRecording = state;

    if (state) {
        m_isPlaying = false;
        reset();
    }
}

bool MacroManager::isPlaying() const { return m_isPlaying; }

void MacroManager::setPlaying(bool state) {
    m_isPlaying = state;

    if (state) {
        m_isRecording = false;
        currentFrame = 0;
        playbackIndex = 0;
    }
}

void MacroManager::reset() {
    events.clear();
    currentFrame = 0;
    playbackIndex = 0;
}

void MacroManager::addEvent(int button, bool player2, bool push, double timestamp) {
    if (!m_isRecording) return;

    events.push_back(MacroEvent {
        currentFrame,
        timestamp,
        button,
        player2,
        push
    });
}

bool MacroManager::saveToFile(std::filesystem::path const& path) const {
    auto arr = matjson::Value::array();

    for (auto const& e : events) {
        auto obj = matjson::Value::object();
        obj["frame"] = e.frame;
        obj["timestamp"] = e.timestamp;
        obj["button"] = e.button;
        obj["player2"] = e.player2;
        obj["push"] = e.push;
        arr.push(obj);
    }

    auto res = utils::file::writeString(path, arr.dump());
    return res.isOk();
}

bool MacroManager::loadFromFile(std::filesystem::path const& path) {
    auto read = utils::file::readString(path);
    if (read.isErr()) return false;

    auto parsed = matjson::parse(read.unwrap());
    if (!parsed.isOk()) return false;

    auto root = parsed.unwrap();
    if (!root.isArray()) return false;

    events.clear();
    for (auto const& item : root) {
        MacroEvent e;
        e.frame = item["frame"].asInt().unwrapOr(0);
        e.timestamp = item["timestamp"].asDouble().unwrapOr(0.0);
        e.button = item["button"].asInt().unwrapOr(0);
        e.player2 = item["player2"].asBool().unwrapOr(false);
        e.push = item["push"].asBool().unwrapOr(false);
        events.push_back(e);
    }

    currentFrame = 0;
    playbackIndex = 0;

    return true;
}
