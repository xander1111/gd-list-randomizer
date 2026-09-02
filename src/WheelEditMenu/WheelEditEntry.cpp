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
    CCMenu* optionsMenu = CCMenu::create();
    optionsMenu->setID("options-menu"_spr);
    optionsMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding(Padding::horizontal(5.f))
    );
    optionsMenu->setContentSize({m_width, Height});
    optionsMenu->setPosition({0.f, 0.f});

    // Level name
    CCLabelBMFont* levelNameLabel = CCLabelBMFont::create(m_slice->level->m_levelName.c_str(), "bigFont.fnt");
    levelNameLabel->setID("level-name-label"_spr);
    levelNameLabel->setScale(std::min(0.5f, 0.75f * m_width / levelNameLabel->getContentWidth()));

    optionsMenu->addChild(levelNameLabel);

    m_weightField = TextInput::create(45.f, "Weight");
    m_weightField->setID("weight-field"_spr);
    m_weightField->setString(std::to_string(m_slice->settings.weight));
    m_weightField->setCommonFilter(CommonFilter::Uint);
    m_weightField->setMaxCharCount(6);  // int limits are higher, but the text starts to get hard to see
    m_weightField->setScale(0.75f * Height / m_weightField->getContentHeight());

    m_weightField->setCallback(std::bind_front(&WheelEditEntry::updateWeight, this));

    optionsMenu->addChild(m_weightField);


    optionsMenu->updateLayout();

    addChildAtPosition(optionsMenu, Anchor::Center);

    return true;
}

void WheelEditEntry::setEnabled(const bool enabled) const
{
    m_weightField->setEnabled(enabled);
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
        pickerWheel->redrawSlices();
    } catch (const std::exception& e) {
        log::debug("[WheelEditEntry::updateWeight]: Exception occurred when updating weight: {}", e.what());
    }
}
