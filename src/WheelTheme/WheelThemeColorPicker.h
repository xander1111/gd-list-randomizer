#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WheelThemeColorPicker : public CCMenu
{
public:
    static WheelThemeColorPicker* create(const std::string& labelRow1, const std::string& labelRow2,
        ccColor4F* m_colorToEdit, Function<void(const ccColor4B&)> callback);

    bool init(const std::string& labelRow1, const std::string& labelRow2);

private:
    ColorChannelSprite* m_colorChannelSprite = nullptr;
    CCMenuItemSpriteExtra* m_colorPickerButton = nullptr;
    CCLabelBMFont* m_labelRow1 = nullptr;
    CCLabelBMFont* m_labelRow2 = nullptr;

    ccColor4F* m_colorToEdit;
    Function<void(const ccColor4B&)> m_callback;

    explicit WheelThemeColorPicker(Function<void(const ccColor4B&)> callback, ccColor4F* m_colorToEdit);
};
