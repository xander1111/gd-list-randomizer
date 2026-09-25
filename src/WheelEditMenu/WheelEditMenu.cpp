#include "WheelEditMenu.h"

#include "WheelEditEntry.h"
#include "../Utils.h"
#include "../WheelFilterLayer.h"
#include "../WheelTheme/WheelTheme.h"
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
    m_content = CCMenu::create();
    m_content->setID("edit-menu-content"_spr);

    AxisLayout* contentLayout = ColumnLayout::create()
        ->setAutoScale(false)
        ->setGap(3.f)
        ->setAxisReverse(true)
        ->setPadding(Padding::vertical(10.f));

    m_content->setLayout(contentLayout);
    m_content->setContentSize({m_width, m_height});

    m_padding = m_height / 16.0f;

    // WheelEditMenu::onOpenFilterMenu

    // Top row of buttons
    CCMenu* buttonMenu = CCMenu::create();
    buttonMenu->setID("button-menu"_spr);
    buttonMenu->setContentSize({m_width - m_padding, (m_height - m_padding) * 1.f / 6.f});
    buttonMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
    );

    // Filter menu button
    CCMenuItemSpriteExtra* filterMenuButton = CCMenuItemExt::createSpriteExtra(
        CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
        [this](CCMenuItemSpriteExtra*)
        {
            WheelFilterLayer::create(&m_entries)->show();
        }
    );

    buttonMenu->addChild(filterMenuButton);

    buttonMenu->updateLayout();

    m_content->addChild(buttonMenu);


    // Search box
    TextInput* searchBox = TextInput::create(m_width - m_padding, "Search by name, creator, id");
    searchBox->setID("search-box"_spr);
    searchBox->setCommonFilter(CommonFilter::Name);
    searchBox->setMaxCharCount(20);

    searchBox->setCallback(std::bind_front(&WheelEditMenu::updateSearch, this));

    m_content->addChild(searchBox);

    // Level list
    const float levelListHeight = (m_height - m_padding) * 2.f / 3.f;
    m_levelListLayer = alpha::ui::AdvancedScrollLayer::create({m_width - m_padding, levelListHeight});
    m_levelListLayer->setID("level-list"_spr);
    m_levelListLayer->setContentHeight(
        m_height
        - buttonMenu->getContentHeight()
        - searchBox->getContentHeight()
        - contentLayout->getGap() * 2.f
        - contentLayout->getPadding().top
        - contentLayout->getPadding().bottom
    );

    for (int i = 0; i < m_slices->size() ; i++) {
        auto& slice = m_slices->at(i);
        WheelEditEntry* entry = WheelEditEntry::create(&slice, &(i % 2 == 0 ? WheelTheme::getDefaultWheelTheme()->sliceColor1 : WheelTheme::getDefaultWheelTheme()->sliceColor2), m_width - m_padding);
        entry->setPositionY(static_cast<float>(m_slices->size() - 1 - i) * WheelEditEntry::Height);

        m_levelListLayer->addChild(entry);
        m_entries.push_back(entry);
    }

    if (!m_entries.empty())
        m_levelListLayer->setInnerContentSize({
            m_entries[0]->getContentWidth(),
            m_entries[0]->getContentHeight() * static_cast<float>(m_entries.size())
        });
    else
        m_levelListLayer->setInnerContentSize({0.f, 0.f});

    m_content->addChild(m_levelListLayer);

    m_content->updateLayout();

    addChildAtPosition(m_content, Anchor::Center);

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

WheelEditEntry* WheelEditMenu::entryForSlice(PickerWheel::Slice* slice) const
{
    WheelEditEntry* ret = nullptr;
    for (auto entry : m_entries) {
        if (entry->getSlice() == slice) {
            ret = entry;
            break;
        }
    }

    return ret;
}

WheelEditMenu::WheelEditMenu(std::vector<PickerWheel::Slice>* slices, const float width, const float height) : m_slices(slices), m_width(width), m_height(height) {}

void WheelEditMenu::updateSearch(std::string const& input)
{
    const std::string inputLower = string::toLower(input);

    int levelId = 0;
    if (!input.empty()) {
        try {
            levelId = stoi(input);
        } catch (std::invalid_argument&) {
            levelId = 0;
        } catch (std::out_of_range&) {
            levelId = 0;
        }
    }

    int visibleCount = 0;
    for (const auto entry : m_entries) {
        bool matches;

        if (!input.empty()) {
            GJGameLevel* level = entry->getSlice()->level;
            const std::string levelNameLower = string::toLower(level->m_levelName);
            const std::string creatorNameLower = string::toLower(level->m_creatorName);

            matches = levelNameLower.contains(inputLower)
                || creatorNameLower.contains(inputLower)
                || level->m_levelID == levelId;

            entry->setSearchVisible(matches);
        } else {
            matches = true;
            entry->setSearchVisible(true);
        }

        if (matches)
            visibleCount++;
    }

    int done = 0;
    for (const auto entry : m_entries) {
        if (entry->getSearchVisible()) {
            entry->setPositionY(static_cast<float>(visibleCount - 1 - done) * WheelEditEntry::Height);
            done++;
        }
    }

    if (visibleCount > 0)
        m_levelListLayer->setInnerContentSize({
            m_entries[0]->getContentWidth(),
            m_entries[0]->getContentHeight() * static_cast<float>(visibleCount)
        });
    else
        m_levelListLayer->setInnerContentSize({0.f, 0.f});
}
