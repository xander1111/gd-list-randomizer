#include "WheelThemeEditLayer.h"
#include "../WheelLayer.h"

WheelThemeEditLayer* WheelThemeEditLayer::create(WheelTheme* wheelTheme)
{
    auto ret = new WheelThemeEditLayer(wheelTheme);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelThemeEditLayer::init()
{
    if (!Popup::init({m_menuWidth, m_menuHeight}))
        return false;

    setTitle("Customize Theme");

    // Color customization page
    CCMenu* colorsPage = CCMenu::create();
    colorsPage->setID("colors-menu"_spr);
    colorsPage->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding({0.f, 40.f, 0.f, 60.f})
    );
    colorsPage->setContentSize({m_menuWidth, m_menuHeight});
    colorsPage->setPosition({0.f, 0.f});
    colorsPage->setAnchorPoint({0.f, 0.f});

    CCMenu* sliceColorsMenu = CCMenu::create();
    sliceColorsMenu->setID("slice-colors-menu"_spr);
    sliceColorsMenu->setLayout(
        RowLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding(Padding::horizontal(20.f))
    );
    sliceColorsMenu->setContentWidth(m_menuWidth);

    // Add and remove colors button
    CCMenu* addRemoveColorsMenu = CCMenu::create();
    addRemoveColorsMenu->setID("add-remove-colors-menu"_spr);
    addRemoveColorsMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setGap(5.f)
        ->setAutoGrowAxis(0.f)
    );

    CCSprite* plusButtonSprite = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    limitNodeWidth(plusButtonSprite, 20.f, 1.f, 0.1f);
    CCMenuItemSpriteExtra* addButton = CCMenuItemExt::createSpriteExtra(
        plusButtonSprite,
        [this] (CCMenuItemSpriteExtra*)
        {
            if (m_wheelTheme->sliceColorCount < 4)
                m_wheelTheme->sliceColorCount++;

            onColorCountChanged();
        }
    );
    addButton->setID("add-color-button"_spr);
    addRemoveColorsMenu->addChild(addButton);

    CCSprite* minusButtonSprite = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
    limitNodeWidth(minusButtonSprite, 20.f, 1.f, 0.1f);
    CCMenuItemSpriteExtra* removeButton = CCMenuItemExt::createSpriteExtra(
        minusButtonSprite,
        [this] (CCMenuItemSpriteExtra*)
        {
            if (m_wheelTheme->sliceColorCount > 2)
                m_wheelTheme->sliceColorCount--;

            onColorCountChanged();
        }
    );
    removeButton->setID("remove-color-button"_spr);
    addRemoveColorsMenu->addChild(removeButton);

    addRemoveColorsMenu->updateLayout();
    sliceColorsMenu->addChild(addRemoveColorsMenu);


    m_sliceColorPickersMenu = CCMenu::create();
    m_sliceColorPickersMenu->setID("slice-color-pickers-menu"_spr);
    m_sliceColorPickersMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
    );
    m_sliceColorPickersMenu->setContentWidth(m_menuWidth - 40.f - 30.f);


    m_sliceColor1Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 1",
        &m_wheelTheme->sliceColor1,
        [](const ccColor4B&)
        {
            onThemeChanged();
        }
    );
    m_sliceColor1Picker->setID("slice-color1-picker"_spr);

    m_sliceColor2Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 2",
        &m_wheelTheme->sliceColor2,
        [](const ccColor4B&)
        {
            onThemeChanged();
        }
    );
    m_sliceColor2Picker->setID("slice-color2-picker"_spr);

    m_sliceColor3Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 3",
        &m_wheelTheme->sliceColor3,
        [](const ccColor4B&)
        {
            onThemeChanged();
        }
    );
    m_sliceColor3Picker->setID("slice-color3-picker"_spr);
    m_sliceColor3Picker->setVisible(m_wheelTheme->sliceColorCount >= 3);

    m_sliceColor4Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 4",
        &m_wheelTheme->sliceColor4,
        [](const ccColor4B&)
        {
            onThemeChanged();
        }
    );
    m_sliceColor4Picker->setID("slice-color4-picker"_spr);
    m_sliceColor4Picker->setVisible(m_wheelTheme->sliceColorCount >= 4);

    m_sliceColorPickersMenu->addChild(m_sliceColor1Picker);
    m_sliceColorPickersMenu->addChild(m_sliceColor2Picker);
    m_sliceColorPickersMenu->addChild(m_sliceColor3Picker);
    m_sliceColorPickersMenu->addChild(m_sliceColor4Picker);

    m_sliceColorPickersMenu->updateLayout();
    sliceColorsMenu->addChild(m_sliceColorPickersMenu);

    // TODO level name text color
    // TODO bg color
    // TODO button color

    sliceColorsMenu->updateLayout();
    colorsPage->addChild(sliceColorsMenu);

    colorsPage->updateLayout();
    m_mainLayer->addChild(colorsPage);


    // TODO tick sound
    // TODO select sound

    // TODO spin speed
    // TODO spin duration

    // TODO show/hide corner decos
    // TODO show/hide level names

    return true;
}

WheelThemeEditLayer::WheelThemeEditLayer(WheelTheme* wheelTheme)
    : m_wheelTheme(wheelTheme) {}

void WheelThemeEditLayer::onColorCountChanged()
{
    m_sliceColor3Picker->setVisible(m_wheelTheme->sliceColorCount >= 3);
    m_sliceColor4Picker->setVisible(m_wheelTheme->sliceColorCount >= 4);

    m_sliceColorPickersMenu->updateLayout();

    onThemeChanged();
}

void WheelThemeEditLayer::onThemeChanged()
{
    CCScene::get()->getChildByType<WheelLayer>()->updateTheme();
}

// TODO - When the theme menu is closed, save the theme. Probably just save it under like "<List ID>-theme"

