#include "PickerWheel.h"

#include "EaseWheelSpin.h"
#include "SliceSelectedPopup.h"
#include "Utils.h"

#include <cvolton.level-id-api/include/EditorIDs.hpp>

PickerWheel* PickerWheel::create(GJLevelList* list, float radius)
{
    auto ret = new PickerWheel(list, radius);
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

    setLayout(AnchorLayout::create());

    // Spin button
    ButtonSprite* spinButtonSprite = ButtonSprite::create("Spin", 0.5f);
    CCMenuItemSpriteExtra* spinButton = CCMenuItemSpriteExtra::create(
        spinButtonSprite,
        this,
        menu_selector(PickerWheel::onSpinWheel)
    );

    CCMenu* const spinButtonMenu = CCMenu::create();
    spinButtonMenu->setID("randomizer-menu"_spr);
    spinButtonMenu->addChild(spinButton);
    spinButtonMenu->setPosition({0, 0});
    spinButtonMenu->setZOrder(1);

    addChildAtPosition(spinButtonMenu, Anchor::Center);

    // Wheel outer menu, used to rotate the wheel without messing up any internal angle calculations
    CCMenu* wheelOuterMenu = CCMenu::create();
    wheelOuterMenu->setID("wheel-outer-menu"_spr);
    wheelOuterMenu->setPosition({0, 0});
    wheelOuterMenu->setAnchorPoint({0, 0});
    wheelOuterMenu->setZOrder(0);

    // Rotate to make the selected slice be at the top of the wheel
    wheelOuterMenu->setRotation(-90.f);


    // Wheel
    m_wheelMenu = CCMenu::create();
    m_wheelMenu->setID("wheel-menu"_spr);
    m_wheelMenu->setPosition({0, 0});
    m_wheelMenu->setAnchorPoint({0, 0});

    // Wheel slices
    m_slicesNode = generateWheelSliceNodes();
    m_slicesNode->setPosition({0, 0});
    m_slicesNode->setZOrder(-1);
    m_wheelMenu->addChild(m_slicesNode);

    // If the last slice uses color 1
    if (m_slices.size() > 1 && m_slices.size() % 2 == 1) {
        log::debug("Last slice uses same color as first slice, generating separator line");

        // start - end instead of end - start used since all the rotations are negative
        const float lastSliceAngle = m_slices[m_slices.size() - 1].startAngleDeg - m_slices[m_slices.size() - 1].endAngleDeg;
        const float firstSliceAngle = m_slices[0].startAngleDeg - m_slices[0].endAngleDeg;

        // Equation used means that a slice that is 1/100th of the wheel gives a thickness of 0.3,
        // maximum thickness is ~0.5, and thickness goes below 0 when a slice is 1/250th of the wheel
        const float lineThickness = std::min(
            -0.002f * (360.f / firstSliceAngle) + 0.5f,
            -0.002f * (360.f / lastSliceAngle) + 0.5f
        );

        if (lineThickness > 0.0f) {
            CCDrawNode* line = CCDrawNode::create();
            line->setID("end-separator"_spr);

            line->drawSegment({0, 0}, {m_radius, 0}, lineThickness, *Utils::DefaultListColorB);
            m_wheelMenu->addChild(line);
        }
    }

    wheelOuterMenu->addChild(m_wheelMenu);

    // Wheel outline
    CCDrawNode* outline = CCDrawNode::create();
    outline->setID("wheel-outline"_spr);
    outline->drawCircle({0, 0}, m_radius, {.r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f}, 1.f, *Utils::DefaultOutlineColor, CircleSegmentCount);
    outline->setZOrder(1);

    wheelOuterMenu->addChild(outline);

    // Ticker
    CCDrawNode* ticker = CCDrawNode::create();
    ticker->setID("ticker"_spr);

    CCPoint tickerPoints[] = {
        {0.f, 4.f},
        {10.f, 0.f},
        {0.f, -4.f}
    };
    ticker->drawPolygon(
        tickerPoints,
        3,
        *Utils::DefaultListColorB,
        0.5f,
        *Utils::DefaultOutlineColor
    );
    ticker->setPosition({14.f, 0.f});
    ticker->setZOrder(1);

    wheelOuterMenu->addChild(ticker);

    addChildAtPosition(wheelOuterMenu, Anchor::Center);

    setContentSize({2.f * m_radius, 2.f * m_radius});

    updateLayout();

    schedule(schedule_selector(PickerWheel::updateAudio), TimePerTickSound);
    scheduleUpdate();

    saveSettings();

    return true;
}

void PickerWheel::update(float dt)
{
    if (m_idleSpin && !m_slices.empty())
        m_wheelMenu->setRotation(m_wheelMenu->getRotation() - IdleRotateRate * dt);
}

void PickerWheel::redrawSlices()
{
    if (m_wheelMenu == nullptr || m_slicesNode == nullptr)
        return;

    m_wheelMenu->removeChild(m_slicesNode, true);

    m_slicesNode = generateWheelSliceNodes();
    m_slicesNode->setPosition({0, 0});
    m_slicesNode->setZOrder(-1);
    m_wheelMenu->addChild(m_slicesNode);

    saveSettings();
}

void PickerWheel::addOnWheelSpin(const std::function<void()>& function)
{
    m_onWheelSpinFuncs.emplace_back(function);
}

void PickerWheel::addOnWheelSpinEnd(const std::function<void()>& function)
{
    m_onWheelSpinEndFuncs.emplace_back(function);
}

PickerWheel::PickerWheel(GJLevelList* list, const float radius)
    : m_list(list), m_radius(radius)
{
    m_slices = std::vector<Slice>(list->totalLevels());

    CCDictionaryExt<int, GJGameLevel*> levels = list->m_levelsDict->asExt<int, GJGameLevel*>();

    std::map<std::string, SliceSettings> settings;
    if (m_list->m_listType == GJLevelType::Editor)
        settings = Mod::get()->getSavedValue<std::map<std::string, SliceSettings>>("editor-" + std::to_string(EditorIDs::getID(m_list)), {});
    else
        settings = Mod::get()->getSavedValue<std::map<std::string, SliceSettings>>(m_list->m_listName, {});

    for (auto [key, level] : levels) {
        const int levelListIndex = list->orderForLevel(level->m_levelID);

        SliceSettings sliceSettings{};
        if (settings.contains(std::to_string(level->m_levelID))) {
            sliceSettings = settings[std::to_string(level->m_levelID)];

            if (sliceSettings.color == nullptr)
                sliceSettings.color = levelListIndex % 2 == 0 ? Utils::DefaultListColorA : Utils::DefaultListColorB;
        } else {
            sliceSettings = SliceSettings {
                .weight = 1u,
                .color = levelListIndex % 2 == 0 ? Utils::DefaultListColorA : Utils::DefaultListColorB
            };
        }

        const Slice newSlice = {
            .level = level,
            .settings = sliceSettings
        };

        m_slices[levelListIndex] = newSlice;
    }
}

void PickerWheel::onSpinWheel(CCObject*)
{
    if (m_slices.empty())
        return;

    m_spinning = true;

    for (const auto& func : m_onWheelSpinFuncs)
        func();

    // Once the user has spun the wheel, disable the idle spin animation
    m_idleSpin = false;

    // Prevent repeated spins from causing wheel rotation to grow past float accuracy
    m_wheelMenu->setRotation(fmod(m_wheelMenu->getRotation(), 360.f));

    // Pick a random slice to land on
    const double roll = random::generate<double, double>(0., 1.);

    // Linear search to find the slice with probability range that includes `roll`
    int sliceIndexPicked = 0;
    double cumulativeProb = 0.;

    while (sliceIndexPicked < m_slices.size()) {
        const double sliceProbability = static_cast<double>(m_slices[sliceIndexPicked].settings.weight) / static_cast<double>(m_totalWeight);

        if (cumulativeProb + sliceProbability > roll)
            break;

        cumulativeProb += sliceProbability;
        sliceIndexPicked++;
    }

    Slice* slicePicked = &m_slices[sliceIndexPicked];
    GJGameLevel* levelPicked = slicePicked->level;

    // Pick a random angle within the selected slice's angle range, plus a 10 full rotations
    //
    // Since we don't rotate to a target angle, but rather add an amount of rotation, we start from the current rotation
    // to account for whatever rotation the wheel had before spinning
    const float rotateAngle = -m_wheelMenu->getRotation()
        + random::generate(slicePicked->endAngleDeg, slicePicked->startAngleDeg)
        - 7200.f;

    log::debug("Picked random slice: level name: {}, slice angle range: ({}, {}), random rotation angle: {}", levelPicked->m_levelName, slicePicked->startAngleDeg, slicePicked->endAngleDeg, rotateAngle);

    CCRotateBy* rotate = CCRotateBy::create(7.f, rotateAngle);
    EaseWheelSpin* rotateEase = EaseWheelSpin::create(rotate);

    const auto onSpinEnd = CallFuncExt::create([slicePicked, this]
    {
        this->m_spinning = false;

        SliceSelectedPopup::create(slicePicked)->show();

        Utils::playResourceSound("selectLevel.ogg");

        for (const auto& func : m_onWheelSpinEndFuncs)
            func();
    });

    CCSequence* seq = CCSequence::create(rotateEase, onSpinEnd, nullptr);

    m_wheelMenu->runAction(seq);
}

void PickerWheel::updateAudio(float)
{
    if (!m_spinning)
        // Don't play ticks when the wheel isn't actually being spun
        return;

    if (m_slices.empty())
        return;

    if (m_wheelMenu->getRotation() > 0.f)
        // This never happens normally, but it's been happening when I manually set the rotation using DevTools and it causes an infinite loop, freezing the game
        return;

    bool playTick = false;

    const float currentRotation = fmod(m_wheelMenu->getRotation(), 360.f);

    // If we are pointing to a different slice, find the slice we are pointing at
    // Repeatedly checks instead of just incrementing as it is possible to pass over multiple slices in one frame
    while (currentRotation < m_slices[m_currentlyPointedAtSlice].endAngleDeg || m_slices[m_currentlyPointedAtSlice].startAngleDeg < currentRotation) {
        // We are pointing to a different slice than we were last update, play a tick noise to indicate this
        playTick = true;

        // Find the slice we are now pointing at
        m_currentlyPointedAtSlice = (m_currentlyPointedAtSlice + 1) % m_slices.size();
    }

    // The previous loop doesn't detect passing over one or more low-weight slices within one frame and ending up back
    // on the same high-weight slice. To pass over other slices but still end up pointing at the same slice, we would
    // have to have passed over the end of the wheel since the end of the wheel has to be between slices, and cannot be
    // in the middle of a slice.
    // To detect this edge case, we check if we rotated backwards, which actually means we looped around the end of the
    // wheel.
    if (m_lastRotation < currentRotation)
        playTick = true;

    m_lastRotation = currentRotation;

    if (playTick)
        Utils::playResourceSound("tick.ogg");
}

void PickerWheel::saveSettings() const
{
    auto allSettings = std::map<std::string, SliceSettings>();

    for (auto slice : m_slices)
        allSettings[std::to_string(slice.level->m_levelID)] = slice.settings;

    if (m_list->m_listType == GJLevelType::Editor)
        Mod::get()->setSavedValue("editor-" + std::to_string(EditorIDs::getID(m_list)), allSettings);
    else
        Mod::get()->setSavedValue(std::to_string(m_list->m_listID), allSettings);
}

CCNode* PickerWheel::generatePickerWheelCircle(const ccColor4F* color, const char* levelName) const
{
    CCNode* sliceNode = CCNode::create();

    CCDrawNode* circle = CCDrawNode::create();
    circle->setID("slice-background"_spr);
    circle->drawCircle({0, 0}, m_radius, *color, 0.f, {.r = 0.f, .g = 0.f, .b = 0.f, .a = 0.f}, CircleSegmentCount);

    circle->setZOrder(-1);
    sliceNode->addChild(circle);

    CCLabelBMFont* label = CCLabelBMFont::create(levelName, "goldFont.fnt");
    label->setID("slice-label"_spr);

    const float labelScale = std::min(MaxFontScale, m_radius * 0.7f / label->getContentSize().width);
    label->setScale(labelScale);
    label->setRotation(0.f);
    label->setPosition({m_radius * 0.95f, 0});
    label->setAnchorPoint({1.f, 0.45f});
    label->setZOrder(1);

    sliceNode->addChild(label);

    return sliceNode;
}

CCMenu* PickerWheel::generateWheelSliceNodes()
{
    CCMenu* wheelSlices = CCMenu::create();
    wheelSlices->setID("wheel-slices"_spr);

    switch (m_slices.size()) {
    case 0: {
        // When we have no levels, draw a placeholder wheel
        log::debug("No slices, generating placeholder wheel");

        wheelSlices->addChild(generatePickerWheelCircle(Utils::DefaultListColorA, "No levels"));

        return wheelSlices;
    }
    case 1: {
        // When we only have one level in the list, we can just draw a circle
        log::debug("1 slice, generating circle wheel");

        wheelSlices->addChild(generatePickerWheelCircle(Utils::DefaultListColorA, m_slices[0].level->m_levelName.c_str()));

        m_slices[0].startAngleDeg = 0.f;
        m_slices[0].endAngleDeg = -360.f;

        return wheelSlices;
    }
    default:
        log::debug(">1 slices, generating arc segments");
        break;
    }

    m_totalWeight = 0;
    unsigned int processedSlicesWeight = 0;

    for (const auto & slice : m_slices)
        m_totalWeight += slice.settings.weight;

    for (auto & slice : m_slices) {
        // Node for both the wheel slice and the text to go under
        CCNode* sliceNode = CCNode::create();

        const float angleDeg = 360.f * (static_cast<float>(slice.settings.weight) / static_cast<float>(m_totalWeight));
        const float angleRad = kmDegreesToRadians(angleDeg);

        CCDrawNode* arc = CCDrawNode::create();
        arc->setID("slice-background"_spr);
        std::vector<CCPoint> points;
        points.reserve(CircleSegmentCount + 3);
        points.emplace_back(0.f, 0.f);

        // Calculate arc points
        const int arcSegments = std::max(1, static_cast<int>(std::ceil((angleDeg / 360.f) * CircleSegmentCount)));
        for (int s = 0; s <= arcSegments; ++s) {
            const float t = static_cast<float>(s) / static_cast<float>(arcSegments);
            const float a = angleRad * t;
            points.emplace_back(m_radius * cos(a), m_radius * sin(a));
        }
        points.emplace_back(0.f, 0.f);

        arc->drawPolygon(
            points.data(),
            static_cast<unsigned int>(points.size()),
            *slice.settings.color,
            0.f,
            {.r = 0, .g = 0, .b = 0, .a = 1},
            BorderAlignment::Center
        );

        arc->setZOrder(-1);
        sliceNode->addChild(arc);

        // ~1 degree is where it's nearly impossible to even tell that there's text. Larger angles might still be unreadable, but you'd be able to tell the text is missing
        if (angleDeg > 1.f) {
            CCLabelBMFont* label = CCLabelBMFont::create(slice.level->m_levelName.c_str(), "goldFont.fnt");
            label->setID("slice-label"_spr);

            // Find maximum possible scale to fit the text into the slice
            // Calculation explanations/visualizations here: https://www.desmos.com/calculator/qkrhzaq1fo

            const CCPoint* maxYPoint = &points[0];
            for (auto& point : points) {
                if (maxYPoint->y < point.y)
                    maxYPoint = &point;
            }

            // Highest Y value is not directly above the other end of the arc, and the 'height' I want is actually the
            // base of the isosceles triangle that fills the arc
            const float sliceHeight = sqrt(powf(maxYPoint->y, 2.f) + powf(m_radius - maxYPoint->x, 2.f));

            const float maxWidth = sliceHeight / (sliceHeight / m_radius + label->getContentHeight() / label->getContentWidth());
            const float maxScale = maxWidth / label->getContentWidth();

            // Further limit the width of the text so it doesn't run into the 'spin' button
            float labelScale = std::min(MaxFontScale, m_radius * 0.7f / label->getContentSize().width);
            labelScale = std::min(labelScale, maxScale);
            label->setScale(labelScale);

            label->setRotation(angleDeg / -2.f);
            label->setPosition({m_radius * 0.95f * cos(angleRad / 2.f), m_radius * 0.95f * sin(angleRad / 2.f)});
            label->setAnchorPoint({1.f, 0.4f});
            label->setZOrder(1);

            sliceNode->addChild(label);
        }

        // Set slice angle and store its angle range
        const float startAngle = static_cast<float>(processedSlicesWeight) / static_cast<float>(m_totalWeight) * -360.f;
        const float endAngle = static_cast<float>(processedSlicesWeight + slice.settings.weight) / static_cast<float>(m_totalWeight) * -360.f;

        sliceNode->setRotation(startAngle);
        // Because cocos2dx uses positive rotation to mean clockwise, which is opposite of what mathematics and thus, the
        // `sin` and `cos` functions from `cmath` use (positive is CCW), we have to get the rotation in the opposite
        // direction before using it when comparing to cocos2dx rotations (which is what these fields are used for)
        slice.startAngleDeg = -endAngle - 360.f;
        slice.endAngleDeg = -startAngle - 360.f;

        processedSlicesWeight += slice.settings.weight;

        wheelSlices->addChild(sliceNode);
    }

    return wheelSlices;
}

Result<PickerWheel::SliceSettings> matjson::Serialize<PickerWheel::SliceSettings>::fromJson(Value const& value)
{
    if (!value.isObject()) return Err("not an object");

    unsigned int weight;
    int colorId;

    try {
        GEODE_UNWRAP_INTO(weight, value["weight"].asUInt());
    } catch (const std::exception&) {
        log::info("Invalid slice wight data, using default value of 1");
        weight = 1;
    }

    try {
        // TODO store and retrieve colors properly
        //   Colors will be able to be set to either one of four theme colors, or individually. If the color is set
        //   individually, it needs to save the RGBA values instead of which theme color to point to
        GEODE_UNWRAP_INTO(colorId, value["colorId"].asInt());
    } catch (const std::exception&) {
        log::info("Invalid slice colorId data, using default value of 0");
        colorId = -1;
    }

    ccColor4F* color = nullptr;

    switch (colorId) {
    case 0:
        color = Utils::DefaultListColorA;
        break;
    case 1:
        color = Utils::DefaultListColorB;
        break;
    default:
        color = nullptr;
        break;
    }

    return Ok(PickerWheel::SliceSettings{ .weight = weight, .color = color });
}

matjson::Value matjson::Serialize<PickerWheel::SliceSettings>::toJson(PickerWheel::SliceSettings const& value)
{
    auto obj = Value();

    obj["weight"] = value.weight;

    if (value.color == Utils::DefaultListColorA) {
        obj["colorId"] = 0;
    } else if (value.color == Utils::DefaultListColorB) {
        obj["colorId"] = 1;
    } else {
        obj["colorId"] = -1;
    }

    return obj;
}
