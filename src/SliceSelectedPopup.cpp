#include "SliceSelectedPopup.h"

#include "WheelLayer.h"

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

    setTitle(m_slice->level->m_levelName);

    CCLabelBMFont* creatorLabel = CCLabelBMFont::create(("By " + m_slice->level->m_creatorName).c_str(), "bigFont.fnt");
    creatorLabel->setScale(0.4f);
    creatorLabel->setID("creator-label"_spr);
    m_mainLayer->addChildAtPosition(creatorLabel, Anchor::Top, {0.f, -40.f});


    // Button menu
    CCMenu* buttonMenu = CCMenu::create();
    buttonMenu->setLayout(
        RowLayout::create()
        ->setGap(10.f)
        ->setAutoGrowAxis(0.f)
    );
    buttonMenu->setAnchorPoint({0.5f, 0.f});

    // Disable button
    ButtonSprite* disableButtonSprite = ButtonSprite::create("Disable Level", 0, false, "bigFont.fnt", "GJ_button_01.png", 30.f, 0.5f);

    CCMenuItemSpriteExtra* disableLevelButton = CCMenuItemSpriteExtra::create(
        disableButtonSprite,
        this,
        menu_selector(SliceSelectedPopup::onDisableLevel)
    );

    buttonMenu->addChild(disableLevelButton);

    // View button
    ButtonSprite* viewLevelButtonSprite = ButtonSprite::create("View", 0, false, "bigFont.fnt", "GJ_button_01.png", 30.f, 0.5f);

    CCMenuItemSpriteExtra* viewLevelButton = CCMenuItemSpriteExtra::create(
        viewLevelButtonSprite,
        this,
        menu_selector(SliceSelectedPopup::onViewLevel)
    );

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

void SliceSelectedPopup::onDisableLevel(CCObject* btn)
{
    // Find entry
    auto editMenu = typeinfo_cast<WheelEditMenu*>(CCScene::get()->getChildByIDRecursive("wheel-edit-menu"_spr));
    if (editMenu == nullptr) {
        log::debug("[SliceSelectedPopup::onDisableLevel]: Disabling level failed: could not find wheel edit menu");
        onClose(btn);
        return;
    }

    WheelEditEntry* entry = editMenu->entryForSlice(m_slice);
    if (entry == nullptr) {
        log::debug("[SliceSelectedPopup::onDisableLevel]: Disabling level failed: could not find entry for slice");
        onClose(btn);
        return;
    }

    // Disable entry
    entry->toggle(false);

    // Close menu
    onClose(btn);
}
