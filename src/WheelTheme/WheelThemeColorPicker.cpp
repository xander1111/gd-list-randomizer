#include "WheelThemeColorPicker.h"

WheelThemeColorPicker* WheelThemeColorPicker::create(const std::string& labelRow1, const std::string& labelRow2,
                                                     const ccColor3B &initialColor, Function<void(WheelThemeColorPicker*)> onClose,
                                                     Function<void(const ccColor4B&)> callback)
{
    auto ret = new WheelThemeColorPicker(std::move(onClose), std::move(callback));
    if (ret && ret->init(labelRow1, labelRow2, initialColor)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelThemeColorPicker::init(const std::string& labelRow1, const std::string& labelRow2, const ccColor3B &initialColor)
{
    if (!CCMenu::init())
        return false;

    setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setGap(2.f)
        ->setAutoGrowAxis(0.f)
    );

    m_colorChannelSprite = ColorChannelSprite::create();
    m_colorChannelSprite->setColor(initialColor);

    m_colorPickerButton = CCMenuItemExt::createSpriteExtra(m_colorChannelSprite, [initialColor, this](CCMenuItemSpriteExtra*)
    {
        ColorPickPopup* picker = ColorPickPopup::create(to4B(initialColor));
        picker->setColorTarget(m_colorChannelSprite);
        picker->setCallback(std::move(m_callback));

        picker->show();
    });
    addChild(m_colorPickerButton);

    m_labelRow1 = CCLabelBMFont::create(labelRow1.c_str(), "bigFont.fnt");
    m_labelRow1->limitLabelWidth(m_colorChannelSprite->getContentWidth(), 0.5f, 0.2f);
    addChild(m_labelRow1);

    m_labelRow2 = CCLabelBMFont::create(labelRow2.c_str(), "bigFont.fnt");
    m_labelRow2->limitLabelWidth(m_colorChannelSprite->getContentWidth(), 0.5f, 0.2f);
    addChild(m_labelRow2);

    updateLayout();

    return true;
}

WheelThemeColorPicker::WheelThemeColorPicker(Function<void(WheelThemeColorPicker*)> onClose,
    Function<void(const ccColor4B&)> callback)
    : m_onClose(std::move(onClose)), m_callback(std::move(callback)) {}
