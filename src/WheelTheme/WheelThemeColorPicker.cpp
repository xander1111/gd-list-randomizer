#include "WheelThemeColorPicker.h"

WheelThemeColorPicker* WheelThemeColorPicker::create(const std::string& labelRow1, const std::string& labelRow2,
    ccColor4F* m_colorToEdit, Function<void(const ccColor4B&)> callback)
{
    auto ret = new WheelThemeColorPicker(std::move(callback), m_colorToEdit);
    if (ret && ret->init(labelRow1, labelRow2)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelThemeColorPicker::init(const std::string& labelRow1, const std::string& labelRow2)
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
    m_colorChannelSprite->setColor(to3B(ccc4BFromccc4F(*m_colorToEdit)));

    m_colorPickerButton = CCMenuItemExt::createSpriteExtra(m_colorChannelSprite, [this](CCMenuItemSpriteExtra*)
    {
        ColorPickPopup* picker = ColorPickPopup::create(ccc4BFromccc4F(*m_colorToEdit));
        picker->setColorTarget(m_colorChannelSprite);
        picker->setCallback([this](const ccColor4B& color)
        {
            *m_colorToEdit = to4F(color);
            m_callback(color);
        });

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

WheelThemeColorPicker::WheelThemeColorPicker(Function<void(const ccColor4B&)> callback, ccColor4F* m_colorToEdit)
    : m_colorToEdit(m_colorToEdit), m_callback(std::move(callback)) {}
