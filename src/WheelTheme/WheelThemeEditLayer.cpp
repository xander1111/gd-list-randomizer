#include "WheelThemeEditLayer.h"

#include "../TogglerWithLabel.h"
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


    // Next page button
    CCSprite* nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextButtonSprite->setFlipX(true);
    m_nextButton = CCMenuItemExt::createSpriteExtra(
        nextButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            m_colorsPage->setVisible(false);
            m_nextButton->setVisible(false);

            m_settingsPage->setVisible(true);
            m_prevButton->setVisible(true);
        }
    );
    m_nextButton->setID("next-button"_spr);
    m_nextButton->setPosition({m_menuWidth + 30.f, m_menuHeight / 2.f});

    m_buttonMenu->addChild(m_nextButton);

    // Previous page button
    m_prevButton = CCMenuItemExt::createSpriteExtra(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        [this](CCMenuItemSpriteExtra*)
        {
            m_colorsPage->setVisible(true);
            m_nextButton->setVisible(true);

            m_settingsPage->setVisible(false);
            m_prevButton->setVisible(false);
        }
    );
    m_prevButton->setID("prev-button"_spr);
    m_prevButton->setPosition({-30.f, m_menuHeight / 2.f});
    m_prevButton->setVisible(false);

    m_buttonMenu->addChild(m_prevButton);


    // Color customization page
    m_colorsPage = CCMenu::create();
    m_colorsPage->setID("colors-menu"_spr);
    m_colorsPage->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(false)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding({0.f, 40.f, 0.f, 60.f})
    );
    m_colorsPage->setContentSize({m_menuWidth, m_menuHeight});
    m_colorsPage->setPosition({0.f, 0.f});
    m_colorsPage->setAnchorPoint({0.f, 0.f});

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
    m_colorsPage->addChild(sliceColorsMenu);

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

    // Background color
    WheelThemeColorPicker* bgColorPicker = WheelThemeColorPicker::create(
        "Background",
        "Color",
        &m_wheelTheme->backgroundColor,
        [](const ccColor4B&)
        {
            onThemeChanged();
        }
    );
    bgColorPicker->setID("bg-color-picker"_spr);

    otherColorMenu->addChild(bgColorPicker);


    otherColorMenu->updateLayout();
    m_colorsPage->addChild(otherColorMenu);

    m_colorsPage->updateLayout();
    m_mainLayer->addChild(m_colorsPage);


    // Settings page
    m_settingsPage = CCMenu::create();
    m_settingsPage->setID("settings-menu"_spr);
    m_settingsPage->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding({0.f, 40.f, 0.f, 60.f})
    );
    m_settingsPage->setContentSize({m_menuWidth, m_menuHeight});
    m_settingsPage->setPosition({0.f, 0.f});
    m_settingsPage->setAnchorPoint({0.f, 0.f});

    m_settingsPage->setVisible(false);

    // Show/hide corner decos and level names
    CCMenu* togglersMenu = CCMenu::create();
    togglersMenu->setID("togglers-menu"_spr);
    togglersMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
    );
    togglersMenu->setContentWidth(m_menuWidth);

    TogglerWithLabel* cornerDecoToggler = TogglerWithLabel::create(
        [this](const TogglerWithLabel* toggler)
        {
            log::debug(":D - corner deco toggler");
            m_wheelTheme->showCornerDecorations = toggler->m_toggled;
            onThemeChanged();
        },
        "Show corner decorations"
    );
    cornerDecoToggler->setID("corner-deco-toggler"_spr);
    cornerDecoToggler->toggle(m_wheelTheme->showCornerDecorations);
    togglersMenu->addChild(cornerDecoToggler);

    TogglerWithLabel* levelNameToggler = TogglerWithLabel::create(
        [this](const TogglerWithLabel* toggler)
        {
            log::debug(":D - level name toggler");
            m_wheelTheme->showLevelNamesOnWheel = toggler->m_toggled;
            onThemeChanged();
        },
        "Show level names"
    );
    levelNameToggler->setID("level-name-toggler"_spr);
    levelNameToggler->toggle(m_wheelTheme->showLevelNamesOnWheel);
    togglersMenu->addChild(levelNameToggler);

    togglersMenu->updateLayout();
    m_settingsPage->addChild(togglersMenu);

    // Spin settings menu
    CCMenu* spinSettingsMenu = CCMenu::create();
    spinSettingsMenu->setID("spin-settings-menu"_spr);
    spinSettingsMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Center)
    );
    spinSettingsMenu->setContentWidth(m_menuWidth);


    // Spin duration
    TextInput* spinDurationField = TextInput::create(100.f, "Spin Duration");
    spinDurationField->setID("duration-field"_spr);
    spinDurationField->setString(std::format("{:.2f}", m_wheelTheme->spinDuration));
    spinDurationField->setCommonFilter(CommonFilter::Float);
    spinDurationField->setMaxCharCount(8);
    spinDurationField->setScale(0.75f);

    spinDurationField->setCallback([this](std::string const& text)
    {
        if (text.empty())
            return;

        try {
            m_wheelTheme->spinDuration = stof(text);
        } catch (const std::exception& e) {
            log::debug("[WheelThemeEditLayer::spinDurationField callback]: Exception occurred when updating duration: {}", e.what());
        }
    });
    spinSettingsMenu->addChild(spinDurationField);

    CCLabelBMFont* spinDurationLabel = CCLabelBMFont::create("Spin Duration", "bigFont.fnt");
    spinDurationLabel->setID("spin-duration-label"_spr);
    spinDurationLabel->setScale(0.6f);

    spinSettingsMenu->addChild(spinDurationLabel);

    spinSettingsMenu->updateLayout();
    m_settingsPage->addChild(spinSettingsMenu);


    // TODO tick sound
    // TODO select sound


    m_settingsPage->updateLayout();
    m_mainLayer->addChild(m_settingsPage);

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
