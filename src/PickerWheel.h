#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PickerWheel : public CCMenu
{
public:

    static PickerWheel* create(GJLevelList* list);

    bool init() override;

private:
    struct PickerWheelSlice
    {
        GJGameLevel* level;
        unsigned int weight;
        ccColor4F* color;
    };

    static ccColor4F* _defaultSliceColorA;
    static ccColor4F* _defaultSliceColorB;
    static ccColor4F* _defaultOutlineColor;
    std::vector<PickerWheelSlice> _slices;

    CCMenu* _wheelMenu = nullptr;

    // Number of segments to use for drawing circles
    static constexpr unsigned int CircleSegmentCount = 65;

    static constexpr float MaxFontScale = 1.f;

    explicit PickerWheel(GJLevelList* list);

    static CCNode* generatePickerWheelCircle(float radius, const ccColor4F* color, const char* levelName);

    void spinWheel(CCObject*);

    [[nodiscard]] CCMenu* generateWheelSliceNodes(float radius) const;
};
