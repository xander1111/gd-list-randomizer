#include "PickerWheel.h"

#include <Geode/utils/cocos.hpp>

ccColor4F* PickerWheel::_defaultSliceColor = new ccColor4F(0.4f, 0.4f, 0.4f, 1.f);

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
    if (!CCMenu::init()) return false;

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // TODO Temp spin button sprite, will make this look better later
    // Spin button
    ButtonSprite* spinButtonSprite = ButtonSprite::create("Spin", 0.5f);
    CCMenuItemSpriteExtra* spinButton = CCMenuItemSpriteExtra::create(
        spinButtonSprite,
        this,
        menu_selector(PickerWheel::spinWheel)
    );

    CCMenu* const spinButtonMenu = CCMenu::create();
    spinButtonMenu->addChild(spinButton);
    spinButtonMenu->setPosition({0, 0});
    spinButtonMenu->setID("randomizer-menu"_spr);

    addChild(spinButtonMenu);


    // Wheel slices
    CCNode* wheelSlices = generateWheelSliceNodes(winSize.height);
    wheelSlices->setPosition({0, 0});
    wheelSlices->setZOrder(-1);
    addChild(wheelSlices);

    return true;
}

PickerWheel::PickerWheel(const GJLevelList* list)
{
    CCDictionaryExt<int, GJGameLevel*> levels = list->m_levelsDict->asExt<int, GJGameLevel*>();
    for (auto [index, level] : levels) {
        PickerWheelSlice newSlice = {
            .level = level,
            .weight = 1,
            .color = _defaultSliceColor
        };

        _slices.emplace_back(newSlice);
    }
}

void PickerWheel::spinWheel(CCObject*)
{
    if (GJGameLevel* levelPicked = random::choice(_slices).level) {
        CCScene* levelScene = LevelInfoLayer::scene(levelPicked, false);
        CCTransitionFade* transitionFade = CCTransitionFade::create(0.5, levelScene);
        CCDirector::sharedDirector()->pushScene(transitionFade);
    } else {
        log::error("LevelListLayer::onRandomizerButton - random object picked is not of type GJGameLevel*");
    }
}

CCMenu* PickerWheel::generateWheelSliceNodes(const float windowHeight) const
{
    CCMenu* wheelSlices = CCMenu::create();

    // TODO Special cases:
    // Exactly 2 levels - triangle masks don't work in this case, need to use rectangles or something
    // Exactly 1 level - Don't need any masks
    // 0 levels - Needs some placeholder

    const unsigned int totalWeight = _slices.size();  // TODO sum weights of slices instead of counting number of levels

    for (int i = 0; i < _slices.size(); ++i) {
        const PickerWheelSlice& slice = _slices[i];

        const float angleDeg = 360.f * (static_cast<float>(slice.weight) / static_cast<float>(totalWeight));
        const float angleRad = kmDegreesToRadians(angleDeg);

        // Create circle to use for arc
        CCDrawNode* circle = CCDrawNode::create();
        circle->drawCircle(
            {0, 0},
            windowHeight * 0.4f,
            *slice.color,
            1.f,
            {.r = 0, .g = 0, .b = 0, .a = 1},
            50
        );

        // Mask circle to only render the desired arc
        float l = windowHeight * 2.f;

        CCDrawNode* mask = CCDrawNode::create();
        CCPoint points[] = {
            {0, 0},
            {l, 0},
            {l * cos(angleRad), l * sin(angleRad)},
            {0, 0}  // Theoretically this point shouldn't be needed, but the mask doesn't seem to properly handle small angles without it
        };
        mask->drawPolygon(
            points,
            4,
            {.r = 0, .g = 0, .b = 0, .a = 1},
            0.f,
            {.r = 0, .g = 0, .b = 0, .a = 1},
            BorderAlignment::Center
        );

        CCClippingNode* clipper = CCClippingNode::create(mask);
        clipper->addChild(circle);

        // TODO add level name text to the slice

        clipper->setRotation(angleDeg * static_cast<float>(i));

        wheelSlices->addChild(clipper);
    }

    return wheelSlices;
}
