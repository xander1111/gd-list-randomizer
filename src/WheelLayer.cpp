#include "WheelLayer.h"

#include "PickerWheel.h"

WheelLayer::WheelLayer(GJLevelList* list) : _list(list) {}

WheelLayer* WheelLayer::create(GJLevelList* list)
{
    auto ret = new WheelLayer(list);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

CCScene* WheelLayer::scene(GJLevelList* list)
{
    WheelLayer* layer = create(list);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool WheelLayer::init()
{
    if (!CCLayer::init()) return false;

    setKeypadEnabled(true);

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Background
    //
    // Recreates the background commonly used by the game for menus
    CCSprite* background = CCSprite::create("GJ_gradientBG-hd.png");
    background->setID("background"_spr);

    const CCSize bgSize = background->getContentSize();

    background->setScaleX((winSize.width + 10) / bgSize.width);
    background->setScaleY((winSize.height + 10) / bgSize.height);

    background->setAnchorPoint({0, 0});
    background->setPosition({-5,-5});

    background->setColor({.r = 0, .g = 102, .b = 255});

    background->setZOrder(-2);
    addChild(background);


    // Corner decorations
    CCSprite* leftCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    CCSprite* rightCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");

    leftCorner->setID("left-corner"_spr);
    rightCorner->setID("right-corner"_spr);

    leftCorner->setAnchorPoint({0, 0});
    rightCorner->setAnchorPoint({1, 0});

    leftCorner->setPosition({-1,-1});
    rightCorner->setPosition({winSize.width + 1,-1});

    rightCorner->setFlipX(true);

    addChild(leftCorner);
    addChild(rightCorner);


    // Back button
    CCMenuItemSpriteExtra* exitButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(WheelLayer::onBack)
    );
    exitButton->setID("exit-button"_spr);


    CCMenu* const exitMenu = CCMenu::create();
    exitMenu->setID("exit-menu"_spr);

    exitMenu->addChild(exitButton);
    exitMenu->setPosition({24, winSize.height - 23});

    addChild(exitMenu);


    // Picker wheel
    CCMenu* pickerWheel = PickerWheel::create(_list);
    pickerWheel->setID("picker-wheel"_spr);

    pickerWheel->setPosition({winSize.width / 2, winSize.height / 2});

    addChild(pickerWheel);

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
