#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DimmingButton : public CCMenu
{
public:
    CCMenuItemSprite* m_button = nullptr;

    static DimmingButton* create(CCNode* sprite, Function<void (DimmingButton*)> callback, bool on = false);

    bool init(CCNode* sprite, bool on);

    [[nodiscard]] bool isToggled() const { return m_toggled; }

    void toggle(bool on);

    void toggleWithCallback(bool on);

private:
    bool m_toggled = false;
    Function<void(DimmingButton*)> m_callback;

    explicit DimmingButton(Function<void(DimmingButton*)> callback);
};
