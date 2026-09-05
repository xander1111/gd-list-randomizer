#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TogglerWithLabel : public CCMenu
{
public:
    static TogglerWithLabel* create(CCObject* target, Function<void(CCMenuItemToggler*)> callback,
        const gd::string& labelText, float togglerScale = 0.8f, float labelScale = 0.35f);

    bool init() override;

    void toggle(bool on) const;

private:
    explicit TogglerWithLabel(CCObject* target, Function<void(CCMenuItemToggler*)> callback, float togglerScale,
        gd::string labelText, float labelScale);

    CCObject* m_target;
    Function<void(CCMenuItemToggler*)> m_callback;
    float m_togglerScale;
    float m_labelScale;
    gd::string m_labelText;

    CCMenuItemToggler* m_toggler = nullptr;
    CCLabelBMFont* m_label = nullptr;
};
