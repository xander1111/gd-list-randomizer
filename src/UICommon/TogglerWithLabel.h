#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class TogglerWithLabel : public CCMenu
{
public:
    /// We use our own `m_toggled` field to track whether this entry is toggled since toggler callbacks seem to get
    /// called before the toggler's `m_toggled` field gets updated. If we used the toggler's `m_toggled` field in a
    /// callback, it would give us the value that the toggler was previously set to, rather than the value it is being
    /// updated to.
    bool m_toggled = false;

    static TogglerWithLabel* create(Function<void(TogglerWithLabel*)> callback,
        const gd::string& labelText, float togglerScale = 0.8f, float labelScale = 0.35f);

    bool init() override;

    void toggle(bool on);

    void toggleWithCallback(bool on);

private:
    Function<void(TogglerWithLabel*)> m_callback;
    float m_togglerScale;
    float m_labelScale;
    gd::string m_labelText;

    CCMenuItemToggler* m_toggler = nullptr;
    CCLabelBMFont* m_label = nullptr;

    explicit TogglerWithLabel(Function<void(TogglerWithLabel*)> callback, float togglerScale,
        gd::string labelText, float labelScale);
};
