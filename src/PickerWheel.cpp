#include "PickerWheel.h"

#include <Geode/utils/cocos.hpp>

ccColor4F* PickerWheel::_defaultSliceColorA = new ccColor4F(0.4f, 0.4f, 0.4f, 1.f);
ccColor4F* PickerWheel::_defaultSliceColorB = new ccColor4F(0.8f, 0.8f, 0.8f, 1.f);

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
    bool firstSlice = true;
    bool evenSlice = true;
    for (auto [key, level] : levels) {
        PickerWheelSlice newSlice = {
            .level = level,
            .weight = firstSlice ? 100u : 1u,  // TODO undo temp test
            .color = evenSlice ? _defaultSliceColorA : _defaultSliceColorB
        };

        _slices.emplace_back(newSlice);

        firstSlice = false;
        evenSlice = !evenSlice;
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

    switch (_slices.size()) {
    case 0:
        // TODO add circle and maybe placeholder text
        break;
    case 1:
        // TODO just add a circle
        break;
    default:
        break;
    }

    unsigned int totalWeight = 0;
    float currentRotation = 0.f;
    const float radius = windowHeight * 0.4f;

    for (const auto & slice : _slices)
        totalWeight += slice.weight;

    for (const auto & slice : _slices) {
        const float angleDeg = 360.f * (static_cast<float>(slice.weight) / static_cast<float>(totalWeight));
        CCDrawNode* arc = CCDrawNode::create();
        std::vector<CCPoint> points;
        points.reserve(CircleSegmentCount + 3);
        points.emplace_back(0.f, 0.f);

        const int arcSegments = std::max(1, static_cast<int>(std::ceil((angleDeg / 360.f) * CircleSegmentCount)));
        for (int s = 0; s <= arcSegments; ++s) {
            const float t = static_cast<float>(s) / static_cast<float>(arcSegments);
            const float a = kmDegreesToRadians(angleDeg * t);
            points.emplace_back(radius * cos(a), radius * sin(a));
        }
        points.emplace_back(0.f, 0.f);

        arc->drawPolygon(
            points.data(),
            static_cast<unsigned int>(points.size()),
            *slice.color,
            0.f,
            {.r = 0, .g = 0, .b = 0, .a = 1},
            BorderAlignment::Center
        );

        // ~1 degree is where it's pretty much impossible to even tell that there's text
        if (angleDeg > 1.f) {
            // TODO add level name text to the slice
        }

        arc->setRotation(currentRotation);
        currentRotation -= angleDeg;

        wheelSlices->addChild(arc);
    }

    return wheelSlices;
}
