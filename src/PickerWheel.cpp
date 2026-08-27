#include "PickerWheel.h"

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

PickerWheel::PickerWheel(GJLevelList* list)
{
    _slices = std::vector<PickerWheelSlice>(list->totalLevels());

    CCDictionaryExt<int, GJGameLevel*> levels = list->m_levelsDict->asExt<int, GJGameLevel*>();

    for (auto [key, level] : levels) {
        const int levelListIndex = list->orderForLevel(level->m_levelID);

        const PickerWheelSlice newSlice = {
            .level = level,
            .weight = 1u,
            .color = levelListIndex % 2 == 0 ? _defaultSliceColorA : _defaultSliceColorB
        };

        _slices[levelListIndex] = newSlice;
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

CCNode* PickerWheel::generatePickerWheelCircle(const float radius, const ccColor4F* color, const char* levelName)
{
    CCNode* sliceNode = CCNode::create();

    CCDrawNode* circle = CCDrawNode::create();
    circle->drawCircle({0, 0}, radius, *color, 0.f, {.r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f}, CircleSegmentCount);

    circle->setID("slice-background"_spr);
    circle->setZOrder(-1);
    sliceNode->addChild(circle);

    CCLabelBMFont* label = CCLabelBMFont::create(levelName, "goldFont.fnt");
    label->setID("slice-label"_spr);

    const float labelScale = std::min(MaxFontScale, radius * 0.7f / label->getContentSize().width);
    label->setScale(labelScale);
    label->setRotation(0.f);
    label->setPosition({radius * 0.95f, 0});
    label->setAnchorPoint({1.f, 0.45f});
    label->setZOrder(1);

    sliceNode->addChild(label);

    return sliceNode;
}

CCMenu* PickerWheel::generateWheelSliceNodes(const float windowHeight) const
{
    CCMenu* wheelSlices = CCMenu::create();

    const float radius = windowHeight * 0.4f;

    switch (_slices.size()) {
    case 0: {
        // When we have no levels, draw a placeholder wheel
        log::debug("No slices, generating placeholder wheel");

        wheelSlices->addChild(generatePickerWheelCircle(radius, _defaultSliceColorA, "No levels"));

        return wheelSlices;
    }
    case 1: {
        // When we only have one level in the list, we can just draw a circle
        log::debug("1 slice, generating circle wheel");

        wheelSlices->addChild(generatePickerWheelCircle(radius, _defaultSliceColorA, _slices[0].level->m_levelName.c_str()));

        return wheelSlices;
    }
    default:
        log::debug(">1 slices, generating arc segments");
        break;
    }

    unsigned int totalWeight = 0;
    float currentRotation = 0.f;

    for (const auto & slice : _slices)
        totalWeight += slice.weight;

    for (const auto & slice : _slices) {
        // Node for both the wheel slice and the text to go under
        CCNode* sliceNode = CCNode::create();

        const float angleDeg = 360.f * (static_cast<float>(slice.weight) / static_cast<float>(totalWeight));
        const float angleRad = kmDegreesToRadians(angleDeg);

        CCDrawNode* arc = CCDrawNode::create();
        std::vector<CCPoint> points;
        points.reserve(CircleSegmentCount + 3);
        points.emplace_back(0.f, 0.f);

        // Calculate arc points
        const int arcSegments = std::max(1, static_cast<int>(std::ceil((angleDeg / 360.f) * CircleSegmentCount)));
        for (int s = 0; s <= arcSegments; ++s) {
            const float t = static_cast<float>(s) / static_cast<float>(arcSegments);
            const float a = angleRad * t;
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

        arc->setID("slice-background"_spr);
        arc->setZOrder(-1);
        sliceNode->addChild(arc);

        // ~1 degree is where it's nearly impossible to even tell that there's text. Larger angles might still be unreadable, but you'd be able to tell the text is missing
        if (angleDeg > 1.f) {
            CCLabelBMFont* label = CCLabelBMFont::create(slice.level->m_levelName.c_str(), "goldFont.fnt");
            label->setID("slice-label"_spr);

            float labelScale = std::min(MaxFontScale, radius * 0.7f / label->getContentSize().width);

            float minY = 0.f;
            float maxY = 0.f;
            for (const auto& point : points) {
                minY = std::min(minY, point.y);
                maxY = std::max(maxY, point.y);
            }
            const float sliceHeight = maxY - minY;
            labelScale = std::min(labelScale, sliceHeight * 0.5f / label->getContentSize().height);
            label->setScale(labelScale);
            label->setRotation(angleDeg / -2.f);
            label->setPosition({radius * 0.95f * cos(angleRad / 2.f), radius * 0.95f * sin(angleRad / 2.f)});
            label->setAnchorPoint({1.f, 0.45f});
            label->setZOrder(1);

            sliceNode->addChild(label);
        }

        sliceNode->setRotation(currentRotation);
        currentRotation -= angleDeg;

        wheelSlices->addChild(sliceNode);
    }

    return wheelSlices;
}
