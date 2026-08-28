#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PickerWheel : public CCMenu
{
public:
    struct PickerWheelSlice
    {
        GJGameLevel* level;
        unsigned int weight;
        ccColor4F* color;

        mutable float startAngleDeg;
        mutable float endAngleDeg;
    };

    static PickerWheel* create(GJLevelList* list);

    bool init() override;

private:
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

    /**
     * Generates the wheel visuals, including a circle with a section for each slice in `_slices`
     *
     * @remarks Additionally sets the `angleStartDeg` and `angleEndDeg` values of each slice in `_slices` to match the
     * angle range that particular slice occupies
     *
     * @param radius radius of the wheel
     * @return a CCMenu object that contains the wheel slices
     */
    [[nodiscard]] CCMenu* generateWheelSliceNodes(float radius) const;
};
