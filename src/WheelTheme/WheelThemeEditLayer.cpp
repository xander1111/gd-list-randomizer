#include "WheelThemeEditLayer.h"

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
        [this] (CCMenuItemSpriteExtra* button)
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
        [this] (CCMenuItemSpriteExtra* button)
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
        to3B(ccc4BFromccc4F(WheelTheme::getDefaultWheelTheme()->sliceColor1)),
        std::bind_front(&WheelThemeEditLayer::onSliceColorChanged, this),
        [this](const ccColor4B& color)
        {
            m_wheelTheme->sliceColor1 = to4F(color);
        }
    );
    m_sliceColor1Picker->setID("slice-color1-picker"_spr);

    m_sliceColor2Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 2",
        to3B(ccc4BFromccc4F(WheelTheme::getDefaultWheelTheme()->sliceColor2)),
        std::bind_front(&WheelThemeEditLayer::onSliceColorChanged, this),
        [this](const ccColor4B& color)
        {
            m_wheelTheme->sliceColor2 = to4F(color);
        }
    );
    m_sliceColor2Picker->setID("slice-color2-picker"_spr);

    m_sliceColor3Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 3",
        to3B(ccc4BFromccc4F(WheelTheme::getDefaultWheelTheme()->sliceColor3)),
        std::bind_front(&WheelThemeEditLayer::onSliceColorChanged, this),
        [this](const ccColor4B& color)
        {
            m_wheelTheme->sliceColor3 = to4F(color);
        }
    );
    m_sliceColor3Picker->setID("slice-color3-picker"_spr);
    m_sliceColor3Picker->setVisible(m_wheelTheme->sliceColorCount >= 3);

    m_sliceColor4Picker = WheelThemeColorPicker::create(
        "Wheel",
        "Color 4",
        to3B(ccc4BFromccc4F(WheelTheme::getDefaultWheelTheme()->sliceColor4)),
        std::bind_front(&WheelThemeEditLayer::onSliceColorChanged, this),
        [this](const ccColor4B& color)
        {
            m_wheelTheme->sliceColor4 = to4F(color);
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

void WheelThemeEditLayer::onColorCountChanged() const
{
    m_sliceColor3Picker->setVisible(m_wheelTheme->sliceColorCount >= 3);
    m_sliceColor4Picker->setVisible(m_wheelTheme->sliceColorCount >= 4);

    m_sliceColorPickersMenu->updateLayout();
}

void WheelThemeEditLayer::onSliceColorChanged(WheelThemeColorPicker*)
{
    onThemeChanged();
}

void WheelThemeEditLayer::onThemeChanged()
{
    // TODO when theme gets updated:
    //   - Redraw entire WheelLayer? At least the background and decorations need to be redrawn
    //   - Redraw entire PickerWheel?
    //     - Unsure if it would be worth it to try to only redraw the necessary parts, like avoiding redrawing slices if
    //       only the outline color changed. In theory, it shouldn't really be an issue since people are probably not
    //       going to be making that many changes in quick succession, but maybe the color picker updates frequently?
}

// TODO - When the theme menu is closed, save the theme. Probably just save it under like "<List ID>-theme"

