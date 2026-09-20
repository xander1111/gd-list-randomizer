#include "WheelEditEntry.h"

#include "../Utils.h"
#include "../WheelLayer.h"

WheelEditEntry* WheelEditEntry::create(PickerWheel::Slice* slice, ccColor4F* color, const float width)
{
    auto ret = new WheelEditEntry(slice, color, width);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelEditEntry::init()
{
    if (!CCNode::init())
        return false;

    setContentSize({m_width, Height});
    setLayout(AnchorLayout::create());


    // Background
    m_background = CCLayerColor::create(ccc4BFromccc4F(*m_color), m_width, Height);
    m_background->setID("background"_spr);
    m_background->setZOrder(-1);

    addChildAtPosition(m_background, Anchor::BottomLeft);


    // Level options menu
    m_optionsMenu = CCMenu::create();
    m_optionsMenu->setID("options-menu"_spr);
    m_optionsMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding(Padding::horizontal(5.f))
    );
    m_optionsMenu->setContentSize({m_width, Height});
    m_optionsMenu->setPosition({0.f, 0.f});


    // Left align items
    CCMenu* leftMenu = CCMenu::create();
    leftMenu->setID("left"_spr);
    leftMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setGap(5.f)
        ->setAutoGrowAxis(0.f)
    );


    // Visibility toggle
    m_toggleButton = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(WheelEditEntry::onToggle),
        1.f
    );
    m_toggleButton->setScale(0.75f * Height / m_toggleButton->getContentHeight());

    m_toggled = m_slice->settings.enabled;
    m_toggleButton->toggle(m_slice->settings.enabled);

    leftMenu->addChild(m_toggleButton);


    // Level name
    CCLabelBMFont* levelNameLabel = CCLabelBMFont::create(m_slice->level->m_levelName.c_str(), "bigFont.fnt");
    levelNameLabel->setID("level-name-label"_spr);
    levelNameLabel->setScale(std::min(0.5f, 0.75f * m_width / levelNameLabel->getContentWidth()));

    leftMenu->addChild(levelNameLabel);


    // Right align items
    CCMenu* rightMenu = CCMenu::create();
    rightMenu->setID("right"_spr);
    rightMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setGap(5.f)
        ->setAutoGrowAxis(0.f)
    );

    m_weightField = TextInput::create(45.f, "Weight");
    m_weightField->setID("weight-field"_spr);
    m_weightField->setString(std::to_string(m_slice->settings.weight));
    m_weightField->setCommonFilter(CommonFilter::Uint);
    m_weightField->setMaxCharCount(6);  // int limits are higher, but the text starts to get hard to see
    m_weightField->setScale(0.75f * Height / m_weightField->getContentHeight());

    m_weightField->setCallback(std::bind_front(&WheelEditEntry::updateWeight, this));

    rightMenu->addChild(m_weightField);


    leftMenu->updateLayout();
    rightMenu->updateLayout();

    m_optionsMenu->addChild(leftMenu);
    m_optionsMenu->addChild(rightMenu);

    m_optionsMenu->updateLayout();

    addChildAtPosition(m_optionsMenu, Anchor::Center);

    return true;
}

void WheelEditEntry::setEnabled(const bool enabled) const
{
    m_weightField->setEnabled(enabled);
    m_toggleButton->setEnabled(enabled);
}

void WheelEditEntry::setSearchVisible(const bool visible)
{
    m_searchVisible = visible;

    // The entry's own visibility is managed by AdvancedScrollLayer's culling, so to make this not visible we can just
    // move it on or off the side of the screen
    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (!visible)
        setPositionX(winSize.width * 2.f);
    else
        setPositionX(0.f);
}

void WheelEditEntry::toggle(const bool enabled, const bool redrawWheel)
{
    m_toggleButton->toggle(enabled);

    m_toggled = enabled;
    m_slice->settings.enabled = m_toggled;

    if (redrawWheel) {
        PickerWheel* pickerWheel = CCScene::get()->getChildByType<WheelLayer>()->m_pickerWheel;
        pickerWheel->redrawWheel();
    }
}

WheelEditEntry::WheelEditEntry(PickerWheel::Slice* slice, ccColor4F* color, const float width)
    : m_slice(slice),
    m_width(width),
    m_color(color) {}

void WheelEditEntry::updateWeight(std::string const& text) const
{
    if (text.empty())
        return;

    try {
        m_slice->settings.weight = stoi(text);
        PickerWheel* pickerWheel = CCScene::get()->getChildByType<WheelLayer>()->m_pickerWheel;
        pickerWheel->redrawWheel();
    } catch (const std::exception& e) {
        log::debug("[WheelEditEntry::updateWeight]: Exception occurred when updating weight: {}", e.what());
    }
}

void WheelEditEntry::onToggle(CCObject*)
{
    m_toggled = !m_toggled;
    m_slice->settings.enabled = m_toggled;

    PickerWheel* pickerWheel = CCScene::get()->getChildByType<WheelLayer>()->m_pickerWheel;
    pickerWheel->redrawWheel();
}
