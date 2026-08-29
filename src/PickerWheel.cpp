#include "PickerWheel.h"

#include "EaseWheelSpin.h"
#include "SliceSelectedPopup.h"
#include "Utils.h"

ccColor4F* PickerWheel::m_defaultSliceColorA = new ccColor4F(161.f / 255.f, 88.f / 255.f, 44.f / 255.f, 1.f);
ccColor4F* PickerWheel::m_defaultSliceColorB = new ccColor4F(194.f / 255.f, 114.f / 255.f, 62.f / 255.f, 1.f);
ccColor4F* PickerWheel::m_defaultOutlineColor = new ccColor4F(0.f, 0.f, 0.f, 1.f);

PickerWheel* PickerWheel::create(GJLevelList* list)
{
    auto ret = new PickerWheel(list);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
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
    spinButtonMenu->setZOrder(1);

    addChild(spinButtonMenu);


    // Wheel
    m_wheelMenu = CCMenu::create();
    m_wheelMenu->setID("wheel-menu"_spr);
    m_wheelMenu->setPosition({0, 0});
    m_wheelMenu->setAnchorPoint({0, 0});
    m_wheelMenu->setZOrder(0);


    // Wheel slices
    const float radius = winSize.height * 0.4f;;

    CCNode* wheelSlices = generateWheelSliceNodes(radius);
    wheelSlices->setPosition({0, 0});
    wheelSlices->setZOrder(-1);
    m_wheelMenu->addChild(wheelSlices);

    // Wheel outline
    CCDrawNode* outline = CCDrawNode::create();
    outline->drawCircle({0, 0}, radius, {.r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f}, 1.f, *m_defaultOutlineColor, CircleSegmentCount);
    outline->setZOrder(1);
    outline->setID("wheel-outline"_spr);

    m_wheelMenu->addChild(outline);

    // If the last slice uses color 1
    if (m_slices.size() > 2 && m_slices.size() % 2 == 1) {
        log::debug("Last slice uses same color as first slice, generating separator line");

        CCDrawNode* line = CCDrawNode::create();
        const float lineThickness = m_slices.size() > 50 ? 0.3f : 0.5f;

        line->drawSegment({0, 0}, {radius, 0}, lineThickness, *m_defaultSliceColorB);
        line->setID("end-separator"_spr);

        m_wheelMenu->addChild(line);
    }

    addChild(m_wheelMenu);

    schedule(schedule_selector(PickerWheel::updateAudio), TimePerTickSound);
    scheduleUpdate();

    return true;
}

void PickerWheel::update(float dt)
{
    if (m_idleSpin && !m_slices.empty())
        m_wheelMenu->setRotation(m_wheelMenu->getRotation() - IdleRotateRate * dt);
}

PickerWheel::PickerWheel(GJLevelList* list)
{
    m_slices = std::vector<PickerWheelSlice>(list->totalLevels());

    CCDictionaryExt<int, GJGameLevel*> levels = list->m_levelsDict->asExt<int, GJGameLevel*>();

    for (auto [key, level] : levels) {
        const int levelListIndex = list->orderForLevel(level->m_levelID);

        const PickerWheelSlice newSlice = {
            .level = level,
            .weight = 1u,
            .color = levelListIndex % 2 == 0 ? m_defaultSliceColorA : m_defaultSliceColorB
        };

        m_slices[levelListIndex] = newSlice;
    }
}

void PickerWheel::spinWheel(CCObject*)
{
    if (m_slices.empty())
        return;

    // Once the user has spun the wheel, disable the idle spin animation
    m_idleSpin = false;

    // Prevent repeated spins from causing wheel rotation to grow past float accuracy
    m_wheelMenu->setRotation(fmod(m_wheelMenu->getRotation(), 360.f));

    // Pick a random slice to land on
    const int sliceIndexPicked = random::generate<int, int>(0, m_slices.size());
    PickerWheelSlice* slicePicked = &m_slices[sliceIndexPicked];
    GJGameLevel* levelPicked = slicePicked->level;

    // Pick a random angle within the selected slice's angle range, plus a 10 full rotations
    //
    // Since we don't rotate to a target angle, but rather add an amount of rotation, we start from the current rotation
    // to account for whatever rotation the wheel had before spinning
    const float rotateAngle = -m_wheelMenu->getRotation()
        - random::generate(slicePicked->endAngleDeg, slicePicked->startAngleDeg)
        - 7200.f;

    log::debug("Picked random slice: level name: {}, slice angle range: ({}, {}), random rotation angle: {}", levelPicked->m_levelName, slicePicked->startAngleDeg, slicePicked->endAngleDeg, rotateAngle);

    CCRotateBy* rotate = CCRotateBy::create(7.f, rotateAngle);
    EaseWheelSpin* rotateEase = EaseWheelSpin::create(rotate);

    const auto showLevelPopup = CallFuncExt::create([slicePicked]
    {
        SliceSelectedPopup::create(slicePicked)->show();

        Utils::playResourceSound("selectLevel.ogg");
    });

    CCSequence* seq = CCSequence::create(rotateEase, showLevelPopup, nullptr);

    m_wheelMenu->runAction(seq);
}

void PickerWheel::updateAudio(float)
{
    if (m_idleSpin)
        // Don't play ticks when the wheel is just idly spinning
        return;

    if (m_slices.empty())
        return;

    bool playTick = false;

    // Find the first slice we are in the angle range of. Repeatedly checks incase we pass over multiple slices in one frame
    const float currentRotation = fmod(m_wheelMenu->getRotation(), 360.f);

    // While outside the angle range of the current slice
    while (currentRotation < m_slices[m_currentlyPointedAtSlice].endAngleDeg || m_slices[m_currentlyPointedAtSlice].startAngleDeg < currentRotation) {
        // We are pointing to a different slice than we were last update, play a tick noise to indicate this
        playTick = true;

        m_currentlyPointedAtSlice = (m_currentlyPointedAtSlice + 1) % m_slices.size();
    }

    if (playTick)
        Utils::playResourceSound("tick.ogg");
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

CCMenu* PickerWheel::generateWheelSliceNodes(const float radius) const
{
    CCMenu* wheelSlices = CCMenu::create();
    wheelSlices->setID("wheel-slices"_spr);

    switch (m_slices.size()) {
    case 0: {
        // When we have no levels, draw a placeholder wheel
        log::debug("No slices, generating placeholder wheel");

        wheelSlices->addChild(generatePickerWheelCircle(radius, m_defaultSliceColorA, "No levels"));

        return wheelSlices;
    }
    case 1: {
        // When we only have one level in the list, we can just draw a circle
        log::debug("1 slice, generating circle wheel");

        wheelSlices->addChild(generatePickerWheelCircle(radius, m_defaultSliceColorA, m_slices[0].level->m_levelName.c_str()));

        m_slices[0].startAngleDeg = 0.f;
        m_slices[0].endAngleDeg = -360.f;

        return wheelSlices;
    }
    default:
        log::debug(">1 slices, generating arc segments");
        break;
    }

    unsigned int totalWeight = 0;
    unsigned int processedSlicesWeight = 0;

    for (const auto & slice : m_slices)
        totalWeight += slice.weight;

    for (auto & slice : m_slices) {
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

        // Set slice angle and store its angle range
        const float startAngle = static_cast<float>(processedSlicesWeight) / static_cast<float>(totalWeight) * -360.f;
        const float endAngle = static_cast<float>(processedSlicesWeight + slice.weight) / static_cast<float>(totalWeight) * -360.f;

        sliceNode->setRotation(startAngle);
        slice.startAngleDeg = startAngle;
        slice.endAngleDeg = endAngle;

        processedSlicesWeight += slice.weight;

        wheelSlices->addChild(sliceNode);
    }

    return wheelSlices;
}
