#include "WheelThemeEditLayer.h"

#include <iomanip>
#include <sstream>

#include "../UICommon/TogglerWithLabel.h"
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
    CCMenuItemSpriteExtra* resetThemeButton = CCMenuItemExt::createSpriteExtra(
        ButtonSprite::create("Reset Theme", 0.5f),
        std::bind_front(&WheelThemeEditLayer::onResetThemeButton, this)
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
        ->setPadding({0.f, 30.f, 0.f, 40.f})
    );
    m_colorsPage->setContentSize({m_menuWidth, m_menuHeight});
    m_colorsPage->setPosition({0.f, 0.f});
    m_colorsPage->setAnchorPoint({0.f, 0.f});


    // Button color selector
    CCMenu* buttonColorMenu = CCMenu::create();
    buttonColorMenu->setID("button-color-menu"_spr);
    buttonColorMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAutoGrowAxis(0.f)
    );

    CCLabelBMFont* buttonColorMenuLabel = CCLabelBMFont::create(
        "Button Color",
        "bigFont.fnt"
    );
    buttonColorMenuLabel->setID("button-color-menu-label"_spr);
    buttonColorMenuLabel->setScale(0.5f);
    buttonColorMenu->addChild(buttonColorMenuLabel);

    CCMenu* buttonColorButtonMenu = CCMenu::create();
    buttonColorButtonMenu->setID("button-color-button-menu"_spr);
    buttonColorButtonMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
    );
    buttonColorButtonMenu->setContentWidth(m_menuWidth);

    ButtonSprite* greenButtonSprite = ButtonSprite::create(
        "Spin",
        "bigFont.fnt",
        "GJ_button_01.png"
    );
    greenButtonSprite->setScale(0.5f);
    greenButtonSprite->setColor({.r = 125, .g = 125, .b = 125});
    m_greenButton = CCMenuItemExt::createSpriteExtra(
        greenButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            highlightButton(Utils::Green);
            m_wheelTheme->buttonColor = Utils::Green;
            onThemeChanged();
        }
    );
    m_greenButton->setID("green-button"_spr);

    ButtonSprite* cyanButtonSprite = ButtonSprite::create(
        "Spin",
        "bigFont.fnt",
        "GJ_button_02.png"
    );
    cyanButtonSprite->setScale(0.5f);
    cyanButtonSprite->setColor({.r = 125, .g = 125, .b = 125});
    m_cyanButton = CCMenuItemExt::createSpriteExtra(
        cyanButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            highlightButton(Utils::Cyan);
            m_wheelTheme->buttonColor = Utils::Cyan;
            onThemeChanged();
        }
    );
    m_cyanButton->setID("cyan-button"_spr);

    ButtonSprite* pinkButtonSprite = ButtonSprite::create(
        "Spin",
        "bigFont.fnt",
        "GJ_button_03.png"
    );
    pinkButtonSprite->setScale(0.5f);
    pinkButtonSprite->setColor({.r = 125, .g = 125, .b = 125});
    m_pinkButton = CCMenuItemExt::createSpriteExtra(
        pinkButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            highlightButton(Utils::Pink);
            m_wheelTheme->buttonColor = Utils::Pink;
            onThemeChanged();
        }
    );
    m_pinkButton->setID("pink-button"_spr);

    ButtonSprite* grayButtonSprite = ButtonSprite::create(
        "Spin",
        "bigFont.fnt",
        "GJ_button_04.png"
    );
    grayButtonSprite->setScale(0.5f);
    grayButtonSprite->setColor({.r = 125, .g = 125, .b = 125});
    m_grayButton = CCMenuItemExt::createSpriteExtra(
        grayButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            highlightButton(Utils::Gray);
            m_wheelTheme->buttonColor = Utils::Gray;
            onThemeChanged();
        }
    );
    m_grayButton->setID("gray-button"_spr);

    ButtonSprite* blueButtonSprite = ButtonSprite::create(
        "Spin",
        "bigFont.fnt",
        "GJ_button_05.png"
    );
    blueButtonSprite->setScale(0.5f);
    blueButtonSprite->setColor({.r = 125, .g = 125, .b = 125});
    m_blueButton = CCMenuItemExt::createSpriteExtra(
        blueButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            highlightButton(Utils::Blue);
            m_wheelTheme->buttonColor = Utils::Blue;
            onThemeChanged();
        }
    );
    m_blueButton->setID("blue-button"_spr);

    ButtonSprite* redButtonSprite = ButtonSprite::create(
        "Spin",
        "bigFont.fnt",
        "GJ_button_06.png"
    );
    redButtonSprite->setScale(0.5f);
    redButtonSprite->setColor({.r = 125, .g = 125, .b = 125});
    m_redButton = CCMenuItemExt::createSpriteExtra(
        redButtonSprite,
        [this](CCMenuItemSpriteExtra*)
        {
            highlightButton(Utils::Red);
            m_wheelTheme->buttonColor = Utils::Red;
            onThemeChanged();
        }
    );
    m_redButton->setID("red-button"_spr);

    buttonColorButtonMenu->addChild(m_greenButton);
    buttonColorButtonMenu->addChild(m_cyanButton);
    buttonColorButtonMenu->addChild(m_pinkButton);
    buttonColorButtonMenu->addChild(m_grayButton);
    buttonColorButtonMenu->addChild(m_blueButton);
    buttonColorButtonMenu->addChild(m_redButton);

    highlightButton(m_wheelTheme->buttonColor);

    buttonColorButtonMenu->updateLayout();
    buttonColorMenu->addChild(buttonColorButtonMenu);

    buttonColorMenu->updateLayout();
    m_colorsPage->addChild(buttonColorMenu);


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
        ->setPadding({0.f, 30.f, 0.f, 40.f})
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

    // Would like to use `std::format` but MacOS doesn't seem to support it for some reason
    std::ostringstream spinDurationText;
    spinDurationText << std::fixed << std::setprecision(2) << m_wheelTheme->spinDuration;
    spinDurationField->setString(spinDurationText.str());

    spinDurationField->setFilter("1234567890.");
    spinDurationField->setMaxCharCount(8);
    spinDurationField->setScale(0.75f);

    spinDurationField->setCallback([this](std::string const& text)
    {
        if (text.empty())
            return;

        GEODE_UNWRAP_INTO_OR_ELSE(m_wheelTheme->spinDuration, err, numFromString<float>(text))
        {
            log::debug("[WheelThemeEditLayer::spinDurationField callback]: Exception occurred when updating duration: {}", err);
        }
    });
    spinSettingsMenu->addChild(spinDurationField);

    CCLabelBMFont* spinDurationLabel = CCLabelBMFont::create("Spin Duration", "bigFont.fnt");
    spinDurationLabel->setID("spin-duration-label"_spr);
    spinDurationLabel->setScale(0.6f);

    spinSettingsMenu->addChild(spinDurationLabel);

    spinSettingsMenu->updateLayout();
    m_settingsPage->addChild(spinSettingsMenu);


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

void WheelThemeEditLayer::highlightButton(const unsigned int color) const
{
    m_wheelTheme->buttonColor = color;

    if (m_greenButton != nullptr && m_greenButton->getChildByType<ButtonSprite>() != nullptr)
        m_greenButton->getChildByType<ButtonSprite>()->setColor(
            color == Utils::Green ? ccColor3B { .r = 255, .g = 255, .b = 255 }
            : ccColor3B { .r = 125, .g = 125, .b = 125 }
        );

    if (m_cyanButton != nullptr && m_cyanButton->getChildByType<ButtonSprite>() != nullptr)
        m_cyanButton->getChildByType<ButtonSprite>()->setColor(
            color == Utils::Cyan ? ccColor3B { .r = 255, .g = 255, .b = 255 }
            : ccColor3B { .r = 125, .g = 125, .b = 125 }
        );

    if (m_pinkButton != nullptr && m_pinkButton->getChildByType<ButtonSprite>() != nullptr)
        m_pinkButton->getChildByType<ButtonSprite>()->setColor(
            color == Utils::Pink ? ccColor3B { .r = 255, .g = 255, .b = 255 }
            : ccColor3B { .r = 125, .g = 125, .b = 125 }
        );

    if (m_grayButton != nullptr && m_grayButton->getChildByType<ButtonSprite>() != nullptr)
        m_grayButton->getChildByType<ButtonSprite>()->setColor(
            color == Utils::Gray ? ccColor3B { .r = 255, .g = 255, .b = 255 }
            : ccColor3B { .r = 125, .g = 125, .b = 125 }
        );

    if (m_blueButton != nullptr && m_blueButton->getChildByType<ButtonSprite>() != nullptr)
        m_blueButton->getChildByType<ButtonSprite>()->setColor(
            color == Utils::Blue ? ccColor3B { .r = 255, .g = 255, .b = 255 }
            : ccColor3B { .r = 125, .g = 125, .b = 125 }
        );

    if (m_redButton != nullptr && m_redButton->getChildByType<ButtonSprite>() != nullptr)
        m_redButton->getChildByType<ButtonSprite>()->setColor(
            color == Utils::Red ? ccColor3B { .r = 255, .g = 255, .b = 255 }
            : ccColor3B { .r = 125, .g = 125, .b = 125 }
        );
}

void WheelThemeEditLayer::onClose(CCObject* cc_object)
{
    const std::string listId = Utils::getListId(CCScene::get()->getChildByType<WheelLayer>()->m_list);

    Mod::get()->setSavedValue(listId + "-theme", *m_wheelTheme);

    Popup::onClose(cc_object);
}
