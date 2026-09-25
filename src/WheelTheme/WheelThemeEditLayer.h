#pragma once

#include <Geode/Geode.hpp>

#include "../Utils.h"
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

    CCMenuItemSpriteExtra* m_nextButton = nullptr;
    CCMenuItemSpriteExtra* m_prevButton = nullptr;

    CCMenu* m_colorsPage = nullptr;
    CCMenu* m_sliceColorPickersMenu = nullptr;
    WheelThemeColorPicker* m_sliceColor1Picker = nullptr;
    WheelThemeColorPicker* m_sliceColor2Picker = nullptr;
    WheelThemeColorPicker* m_sliceColor3Picker = nullptr;
    WheelThemeColorPicker* m_sliceColor4Picker = nullptr;

    CCMenu* m_settingsPage = nullptr;

    CCMenuItemSpriteExtra* m_greenButton = nullptr;
    CCMenuItemSpriteExtra* m_cyanButton = nullptr;
    CCMenuItemSpriteExtra* m_pinkButton = nullptr;
    CCMenuItemSpriteExtra* m_grayButton = nullptr;
    CCMenuItemSpriteExtra* m_blueButton = nullptr;
    CCMenuItemSpriteExtra* m_redButton = nullptr;

    explicit WheelThemeEditLayer(WheelTheme* wheelTheme);

    void updateVisibleColorPickers();

    static void onThemeChanged();

    void onResetThemeButton(CCObject*);

    void highlightButton(unsigned int color) const;

protected:
    void onClose(CCObject*) override;
};
