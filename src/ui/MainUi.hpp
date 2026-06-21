#pragma once
#include <Geode/Geode.hpp>

class MainUI : public geode::Popup {
protected:
    bool setup() override;

public:
    static MainUI* create();
};
