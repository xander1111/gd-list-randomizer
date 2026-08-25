#include "WheelLayer.h"

WheelLayer::WheelLayer(CCArray* levels) : _levels(levels) {}

WheelLayer* WheelLayer::create(CCArray* levels)
{
    auto layer = new WheelLayer(levels);
    if (layer && layer->init()) {
        layer->autorelease();
    } else {
        delete layer;
        layer = nullptr;
    }

    return layer;
}

CCScene* WheelLayer::scene(CCArray* levels)
{
    WheelLayer* layer = create(levels);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool WheelLayer::init()
{
    if (!CCLayer::init()) return false;

    setKeypadEnabled(true);

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Back button
    CCMenuItemSpriteExtra* exitButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(WheelLayer::onBack)
    );
    exitButton->setID("exit-button"_spr);

    CCMenu* const exitMenu = CCMenu::create();
    exitMenu->addChild(exitButton);
    exitMenu->setPosition({24, winSize.height - 23});
    exitMenu->setID("exit-menu"_spr);

    addChild(exitMenu);


    // Temp randomizer button
    ButtonSprite* randomizerButtonSprite = ButtonSprite::create("Random", 0.3f);
    CCMenuItemSpriteExtra* randomizerButton = CCMenuItemSpriteExtra::create(
        randomizerButtonSprite,
        this,
        menu_selector(WheelLayer::onRandomizerButton)
    );

    CCMenu* const randomizerMenu = CCMenu::create();
    randomizerMenu->addChild(randomizerButton);
    randomizerMenu->setPosition({winSize.width / 2, winSize.height / 2});
    randomizerMenu->setID("randomizer-menu"_spr);

    addChild(randomizerMenu);

    return true;
}

void WheelLayer::keyBackClicked()
{
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void WheelLayer::onBack(CCObject*)
{
    keyBackClicked();
}

void WheelLayer::onRandomizerButton(CCObject*)
{
    GJGameLevel* levelPicked = typeinfo_cast<LevelCell*>(_levels->randomObject())->m_level;

    CCScene* levelScene = LevelInfoLayer::scene(levelPicked, false);
    CCTransitionFade* transitionFade = CCTransitionFade::create(0.5, levelScene);
    CCDirector::sharedDirector()->pushScene(transitionFade);
}
