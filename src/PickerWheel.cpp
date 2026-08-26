#include "PickerWheel.h"

PickerWheel* PickerWheel::create(GJLevelList* list)
{
    auto menu = new PickerWheel(list);
    if (menu && menu->init()) {
        menu->autorelease();
    } else {
        delete menu;
        menu = nullptr;
    }

    return menu;
}

bool PickerWheel::init()
{
    // Temp randomizer button
    ButtonSprite* randomizerButtonSprite = ButtonSprite::create("Random", 0.3f);
    CCMenuItemSpriteExtra* randomizerButton = CCMenuItemSpriteExtra::create(
        randomizerButtonSprite,
        this,
        menu_selector(PickerWheel::spinWheel)
    );

    CCMenu* const randomizerMenu = CCMenu::create();
    randomizerMenu->addChild(randomizerButton);
    randomizerMenu->setPosition({0, 0});
    randomizerMenu->setID("randomizer-menu"_spr);

    addChild(randomizerMenu);

    return true;
}

PickerWheel::PickerWheel(GJLevelList* list) : _list(list) {}

void PickerWheel::spinWheel(CCObject*)
{
    if (const auto levelPicked = typeinfo_cast<GJGameLevel*>(_list->m_levelsDict->randomObject())) {
        CCScene* levelScene = LevelInfoLayer::scene(levelPicked, false);
        CCTransitionFade* transitionFade = CCTransitionFade::create(0.5, levelScene);
        CCDirector::sharedDirector()->pushScene(transitionFade);
    } else {
        log::error("LevelListLayer::onRandomizerButton - random object picked is not of type GJGameLevel*");
    }
}
