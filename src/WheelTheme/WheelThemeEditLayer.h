#pragma once

#include <Geode/Geode.hpp>

#include "WheelThemeColorPicker.h"
#include "WheelTheme.h"

using namespace geode::prelude;

class WheelThemeEditLayer : public Popup
{
public:
    static WheelThemeEditLayer* create(WheelTheme* wheelTheme);

    bool init() override;

private:
    // Same size as `MoreSearchLayer` (the advanced options menu on the search screen)
    static constexpr float m_menuWidth = 440.f;
    static constexpr float m_menuHeight = 290.f;

    WheelTheme* m_wheelTheme;

    CCMenu* m_sliceColorPickersMenu = nullptr;
    WheelThemeColorPicker* m_sliceColor1Picker = nullptr;
    WheelThemeColorPicker* m_sliceColor2Picker = nullptr;
    WheelThemeColorPicker* m_sliceColor3Picker = nullptr;
    WheelThemeColorPicker* m_sliceColor4Picker = nullptr;

    explicit WheelThemeEditLayer(WheelTheme* wheelTheme);

    void onColorCountChanged();

    static void onThemeChanged();
};
