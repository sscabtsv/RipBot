#include "MainUI.hpp"

using namespace geode::prelude;

MainUI* MainUI::create() {
    auto ret = new MainUI();
    if (ret && ret->initAnchored(380.f, 280.f)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MainUI::setup() {
    this->setTitle("RipBot");
    return true;
}
