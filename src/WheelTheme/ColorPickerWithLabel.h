#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ColorPickerWithLabel : public CCMenu
{
public:
    static ColorPickerWithLabel* create(const std::string& labelRow1, const std::string& labelRow2,
        const ccColor3B &initialColor, Function<void(ColorPickerWithLabel*)> onClose,
        Function<void(const ccColor4B&)> callback);

    bool init(const std::string& labelRow1, const std::string& labelRow2, const ccColor3B &initialColor);

private:
    ColorChannelSprite* m_colorChannelSprite = nullptr;
    CCMenuItemSpriteExtra* m_colorPickerButton = nullptr;
    CCLabelBMFont* m_labelRow1 = nullptr;
    CCLabelBMFont* m_labelRow2 = nullptr;

    Function<void(ColorPickerWithLabel*)> m_onClose;
    Function<void(const ccColor4B&)> m_callback;

    explicit ColorPickerWithLabel(Function<void(ColorPickerWithLabel*)> onClose,
        Function<void(const ccColor4B&)> callback);
};
