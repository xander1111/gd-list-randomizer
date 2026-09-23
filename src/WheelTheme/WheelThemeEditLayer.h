#pragma once

#include <Geode/Geode.hpp>

#include "ColorPickerWithLabel.h"
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

    ColorPickerWithLabel* m_sliceColor1Picker = nullptr;
    ColorPickerWithLabel* m_sliceColor2Picker = nullptr;
    ColorPickerWithLabel* m_sliceColor3Picker = nullptr;
    ColorPickerWithLabel* m_sliceColor4Picker = nullptr;

    explicit WheelThemeEditLayer(WheelTheme* wheelTheme);

    void onSliceColorChanged(ColorPickerWithLabel*);

    void onThemeChanged();
};
