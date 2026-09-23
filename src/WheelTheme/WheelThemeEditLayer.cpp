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
    CCMenu* colorsMenu = CCMenu::create();
    colorsMenu->setID("colors-menu"_spr);
    colorsMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding({0.f, 40.f, 0.f, 60.f})
    );
    colorsMenu->setContentSize({m_menuWidth, m_menuHeight});
    colorsMenu->setPosition({0.f, 0.f});
    colorsMenu->setAnchorPoint({0.f, 0.f});


    // Slice colors update menu
    CCMenu* sliceColorsMenu = CCMenu::create();
    sliceColorsMenu->setID("slice-colors-menu"_spr);
    sliceColorsMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(20.f))
    );
    sliceColorsMenu->setContentWidth(m_menuWidth);


    m_sliceColor1Picker = ColorPickerWithLabel::create(
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

    m_sliceColor2Picker = ColorPickerWithLabel::create(
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

    m_sliceColor3Picker = ColorPickerWithLabel::create(
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

    m_sliceColor4Picker = ColorPickerWithLabel::create(
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

    sliceColorsMenu->addChild(m_sliceColor1Picker);
    sliceColorsMenu->addChild(m_sliceColor2Picker);
    sliceColorsMenu->addChild(m_sliceColor3Picker);
    sliceColorsMenu->addChild(m_sliceColor4Picker);

    sliceColorsMenu->updateLayout();
    colorsMenu->addChild(sliceColorsMenu);

    // TODO level name text color
    // TODO bg color
    // TODO button color

    colorsMenu->updateLayout();
    m_mainLayer->addChild(colorsMenu);


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

void WheelThemeEditLayer::onSliceColorChanged(ColorPickerWithLabel*)
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

