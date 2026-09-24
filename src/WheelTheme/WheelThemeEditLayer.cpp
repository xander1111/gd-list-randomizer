#include "WheelThemeEditLayer.h"

#include "../Utils.h"
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

    // Reset to default theme button
    ButtonSprite* resetThemeButtonSprite = ButtonSprite::create("Reset Theme", 0.5f);
    CCMenuItemSpriteExtra* resetThemeButton = CCMenuItemSpriteExtra::create(
        resetThemeButtonSprite,
        this,
        menu_selector(WheelThemeEditLayer::onResetThemeButton)
    );
    resetThemeButton->setID("apply-button"_spr);
    resetThemeButton->setPosition({m_menuWidth / 2.f, 30});

    m_buttonMenu->addChild(resetThemeButton);


    // Color customization page
    CCMenu* colorsPage = CCMenu::create();
    colorsPage->setID("colors-menu"_spr);
    colorsPage->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(false)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
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

            updateVisibleColorPickers();
            onThemeChanged();
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

            updateVisibleColorPickers();
            onThemeChanged();
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

    sliceColorsMenu->updateLayout();
    colorsPage->addChild(sliceColorsMenu);

    // Other colors
    CCMenu* otherColorMenu = CCMenu::create();
    otherColorMenu->setID("other-color-pickers-menu"_spr);
    otherColorMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(20.f))
    );
    otherColorMenu->setContentWidth(m_menuWidth);

    // Level name text color
    WheelThemeColorPicker* levelNameColorPicker = WheelThemeColorPicker::create(
        "Level Name",
        "Color",
        &m_wheelTheme->textColor,
        [](const ccColor4B&)
        {
            onThemeChanged();
        }
    );
    levelNameColorPicker->setID("level-name-color-picker"_spr);

    otherColorMenu->addChild(levelNameColorPicker);

    // TODO button color


    // TODO bg color


    otherColorMenu->updateLayout();
    colorsPage->addChild(otherColorMenu);

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

void WheelThemeEditLayer::updateVisibleColorPickers()
{
    m_sliceColor3Picker->setVisible(m_wheelTheme->sliceColorCount >= 3);
    m_sliceColor4Picker->setVisible(m_wheelTheme->sliceColorCount >= 4);

    m_sliceColorPickersMenu->updateLayout();
}

void WheelThemeEditLayer::onThemeChanged()
{
    CCScene::get()->getChildByType<WheelLayer>()->updateTheme();
}

void WheelThemeEditLayer::onResetThemeButton(CCObject*)
{
    createQuickPopup(
        "Reset to Default Theme",
        "Are you sure you want to reset to the default theme?",
        "No", "Yes",
        [this](auto btn, const bool btn2)
        {
            if (btn2) {
                *m_wheelTheme = *WheelTheme::getDefaultWheelTheme();
                updateVisibleColorPickers();
                onThemeChanged();

                onClose(btn);
            }
        }
    );
}

void WheelThemeEditLayer::onClose(CCObject* cc_object)
{
    const gd::string listId = Utils::getListId(CCScene::get()->getChildByType<WheelLayer>()->m_list);

    Mod::get()->setSavedValue(listId + "-theme", *m_wheelTheme);

    Popup::onClose(cc_object);
}
