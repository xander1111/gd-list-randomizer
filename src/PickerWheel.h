#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PickerWheel : public CCMenu
{
public:
    struct PickerWheelSlice
    {
        // I think this doesn't need to be a `Ref` since it's theoretically held by the `GJLevelList` we get it from
        GJGameLevel* level;
        unsigned int weight;
        ccColor4F* color;

        mutable float startAngleDeg;
        mutable float endAngleDeg;
    };

    static PickerWheel* create(GJLevelList* list);

    bool init() override;

private:
    static ccColor4F* m_defaultSliceColorA;
    static ccColor4F* m_defaultSliceColorB;
    static ccColor4F* m_defaultOutlineColor;
    std::vector<PickerWheelSlice> m_slices;

    // Doesn't need to be a `Ref` since it gets added as a child of `this`
    CCMenu* m_wheelMenu = nullptr;

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
