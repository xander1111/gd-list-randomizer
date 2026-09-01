#include "SliceSelectedPopup.h"

SliceSelectedPopup* SliceSelectedPopup::create(PickerWheel::Slice* slice)
{
    auto ret = new SliceSelectedPopup(slice);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool SliceSelectedPopup::init()
{
    if (!Popup::init(300.f, 100.f))
        return false;

    this->setTitle(m_slice->level->m_levelName);

    CCLabelBMFont* creatorLabel = CCLabelBMFont::create(("By " + m_slice->level->m_creatorName).c_str(), "bigFont.fnt");
    creatorLabel->setScale(0.4f);
    creatorLabel->setID("creator-label"_spr);
    m_mainLayer->addChildAtPosition(creatorLabel, Anchor::Top, {0.f, -40.f});

    // View button
    ButtonSprite* viewLevelButtonSprite = ButtonSprite::create("View", 56, false, "bigFont.fnt", "GJ_button_01-uhd.png", 30.f, 0.5f);
    viewLevelButtonSprite->setContentSize({56, 30});


    CCMenuItemSpriteExtra* viewLevelButton = CCMenuItemSpriteExtra::create(
        viewLevelButtonSprite,
        this,
        menu_selector(SliceSelectedPopup::onViewLevel)
    );

    CCMenu* buttonMenu = CCMenu::create();
    buttonMenu->setLayout(RowLayout::create());
    buttonMenu->setAnchorPoint({0.5f, 0.f});

    buttonMenu->addChild(viewLevelButton);

    buttonMenu->updateLayout();
    buttonMenu->setID("level-buttons-menu"_spr);
    buttonMenu->setZOrder(1);

    m_mainLayer->addChildAtPosition(buttonMenu, Anchor::Bottom, {0, 8});

    return true;
}

SliceSelectedPopup::SliceSelectedPopup(PickerWheel::Slice* slice) : m_slice(slice) {}

void SliceSelectedPopup::onViewLevel(CCObject*)
{
    // Load level page
    CCScene* levelScene = LevelInfoLayer::scene(m_slice->level, false);
    CCTransitionFade* transitionFade = CCTransitionFade::create(0.5, levelScene);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}
