#include "WheelEditMenu.h"

#include "WheelEditEntry.h"
#include "../Utils.h"
#include "alphalaneous.alphas-ui-pack/include/nodes/scroll/Scroll.hpp"

WheelEditMenu* WheelEditMenu::create(std::vector<PickerWheel::Slice>* slices, const float width, const float height)
{
    auto ret = new WheelEditMenu(slices, width, height);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelEditMenu::init()
{
    if (!CCMenu::init())
        return false;

    setContentSize({m_width, m_height});
    setLayout(AnchorLayout::create());

    // Background menu
    NineSlice* bg = NineSlice::create("GJ_square01.png");
    bg->setContentSize({m_width, m_height});
    bg->setZOrder(-1);

    addChildAtPosition(bg, Anchor::Center);


    // Menu content
    CCMenu* content = CCMenu::create();
    content->setID("edit-menu-content"_spr);
    content->setLayout(
        ColumnLayout::create()
        ->setAutoScale(false)
        ->setGap(0.f)
        ->setAxisReverse(true)
    );
    content->setContentSize({m_width, m_height});

    static const float padding = m_height / 16.0f;

    // Search box
    TextInput* searchBox = TextInput::create(m_width - padding, "Search by name, creator, id");
    searchBox->setID("search-box"_spr);
    searchBox->setCommonFilter(CommonFilter::Name);
    searchBox->setMaxCharCount(20);

    searchBox->setCallback(std::bind_front(&WheelEditMenu::updateSearch, this));

    content->addChild(searchBox);

    // Level list
    float levelListHeight = (m_height - padding) * 5.f / 6.f;
    m_levelListLayer = alpha::ui::AdvancedScrollLayer::create({m_width - padding, levelListHeight});
    m_levelListLayer->setID("level-list"_spr);

    for (int i = 0; i < m_slices->size() ; i++) {
        auto slice = m_slices->at(i);

        WheelEditEntry* entry = WheelEditEntry::create(&slice, i % 2 == 0 ? Utils::DefaultListColorA : Utils::DefaultListColorB, m_width - padding);
        entry->setPositionY(static_cast<float>(m_slices->size() - 1 - i) * WheelEditEntry::Height);

        m_levelListLayer->addChild(entry);
        m_entries.emplace_back(entry);
    }

    if (!m_entries.empty())
        setListLayerContentSize();

    content->addChild(m_levelListLayer);

    content->updateLayout();

    addChildAtPosition(content, Anchor::Center);

    updateLayout();

    return true;
}

void WheelEditMenu::onWheelSpin() const
{
    for (const auto entry : m_entries)
        entry->setEnabled(false);
}

void WheelEditMenu::onWheelSpinEnd() const
{
    for (const auto entry : m_entries)
        entry->setEnabled(true);
}

WheelEditMenu::WheelEditMenu(std::vector<PickerWheel::Slice>* slices, const float width, const float height) : m_slices(slices), m_width(width), m_height(height) {}

void WheelEditMenu::updateSearch(std::string const& input) const
{
    if (input.empty()) {
        for (const auto entry : m_entries)
            entry->setVisible(true);

        m_levelListLayer->updateLayout();
        setListLayerContentSize();
        return;
    }

    const std::string inputLower = string::toLower(input);

    for (const auto entry : m_entries) {
        GJGameLevel* level = entry->getSlice()->level;
        const std::string levelNameLower = string::toLower(level->m_levelName);
        const std::string creatorNameLower = string::toLower(level->m_creatorName);
        int levelId;

        try {
            levelId = stoi(input);
        } catch (std::invalid_argument&) {
            levelId = 0;
        } catch (std::out_of_range&) {
            levelId = 0;
        }

        if (levelNameLower.contains(inputLower)
            || creatorNameLower.contains(inputLower)
            || level->m_levelID == levelId) {
            entry->setVisible(true);
        } else {
            entry->setVisible(false);
        }
    }

    setListLayerContentSize();
}

void WheelEditMenu::setListLayerContentSize() const
{
    if (!m_entries.empty())
        m_levelListLayer->setInnerContentSize({
            m_entries[0]->getContentWidth(),
            m_entries[0]->getContentHeight() * static_cast<float>(m_entries.size())
        });
    else
        m_levelListLayer->setInnerContentSize({0.f, 0.f});
}
