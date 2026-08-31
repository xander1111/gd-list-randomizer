#include "WheelEditMenu.h"

#include "WheelEditEntry.h"
#include "../Utils.h"
#include "alphalaneous.alphas-ui-pack/include/nodes/scroll/Scroll.hpp"

WheelEditMenu* WheelEditMenu::create(std::vector<PickerWheel::PickerWheelSlice>* slices, const float width, const float height)
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
    content->setLayout(ColumnLayout::create()->setAutoScale(false)->setGap(0.f));
    content->setContentSize({m_width, m_height});

    static const float padding = m_height / 16.0f;

    // TODO Search box

    // Level list
    float levelListHeight = (m_height - padding) * 5.f / 6.f;
    alpha::ui::AdvancedScrollLayer* levelListLayer = alpha::ui::AdvancedScrollLayer::create({m_width - padding, levelListHeight});
    levelListLayer->setID("level-list"_spr);

    CCMenu* levelListContent = CCMenu::create();
    levelListContent->setID("level-list-content"_spr);
    levelListContent->setLayout(
        ColumnLayout::create()
        ->setAutoScale(false)
        ->setGap(0.f)
        ->setAutoGrowAxis(0.f)
        ->setAxisReverse(true)
    );
    levelListContent->setPosition({0.f, 0.f});
    levelListContent->setAnchorPoint({0.f, 0.f});

    int i = 0;
    for (auto & slice : *m_slices)
        levelListContent->addChild(WheelEditEntry::create(&slice, i++ % 2 == 0 ? Utils::DefaultListColorA : Utils::DefaultListColorB, m_width - padding));

    levelListContent->updateLayout();

    levelListLayer->addChild(levelListContent);
    levelListLayer->setInnerContentSize(levelListContent->getContentSize());

    content->addChild(levelListLayer);

    content->updateLayout();

    addChildAtPosition(content, Anchor::Center);

    updateLayout();

    return true;
}

WheelEditMenu::WheelEditMenu(std::vector<PickerWheel::PickerWheelSlice>* slices, const float width, const float height) : m_slices(slices), m_width(width), m_height(height) {}
