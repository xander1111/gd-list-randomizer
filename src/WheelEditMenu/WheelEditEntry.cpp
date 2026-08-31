#include "WheelEditEntry.h"

#include "../Utils.h"

WheelEditEntry* WheelEditEntry::create(PickerWheel::PickerWheelSlice* slice, ccColor4F* color, const float width, const float height)
{
    auto ret = new WheelEditEntry(slice, color, width, height);
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

    setContentSize({m_width, m_height});
    setLayout(AnchorLayout::create());


    // Background
    m_background = CCLayerColor::create(ccc4BFromccc4F(*m_color), m_width, m_height);
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
    optionsMenu->setContentSize({m_width, m_height});
    optionsMenu->setPosition({0.f, 0.f});

    // Level name
    CCLabelBMFont* levelNameLabel = CCLabelBMFont::create(m_slice->level->m_levelName.c_str(), "bigFont.fnt");
    levelNameLabel->setID("level-name-label"_spr);
    levelNameLabel->setScale(std::min(0.5f, 0.75f * m_width / levelNameLabel->getContentWidth()));

    optionsMenu->addChild(levelNameLabel);

    // Level weight field
    TextInput* weightField = TextInput::create(45.f, "Weight");
    weightField->setID("weight-field"_spr);
    weightField->setString(std::to_string(m_slice->weight));
    weightField->setCommonFilter(CommonFilter::Uint);
    weightField->setMaxCharCount(6);  // int limits are higher, but the text starts to get hard to see
    weightField->setScale(0.75f * m_height / weightField->getContentHeight());

    weightField->setCallback(std::bind_front(&WheelEditEntry::updateWeight, this));

    optionsMenu->addChild(weightField);


    optionsMenu->updateLayout();

    addChildAtPosition(optionsMenu, Anchor::Center);

    return true;
}

WheelEditEntry::WheelEditEntry(PickerWheel::PickerWheelSlice* slice, ccColor4F* color, const float width, const float height)
    : m_slice(slice),
    m_width(width),
    m_height(height),
    m_color(color) {}

void WheelEditEntry::updateWeight(std::string const& text) const
{
    if (text.empty())
        return;

    try {
        m_slice->weight = stoi(text);
        const auto pickerWheel = typeinfo_cast<PickerWheel*>(CCScene::get()->getChildByIDRecursive("picker-wheel"_spr));
        pickerWheel->redrawSlices();
    } catch (const std::exception& e) {
        log::debug("[WheelEditEntry::updateWeight]: Exception occurred when updating weight: {}", e.what());
    }
}
