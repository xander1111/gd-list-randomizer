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

    void update(float dt) override;

private:
    static ccColor4F* m_defaultSliceColorA;
    static ccColor4F* m_defaultSliceColorB;
    static ccColor4F* m_defaultOutlineColor;
    std::vector<PickerWheelSlice> m_slices;

    // Doesn't need to be a `Ref` since it gets added as a child of `this`
    CCMenu* m_wheelMenu = nullptr;

    // Tracks the index of the slice that the wheel ticker is currently pointing at
    unsigned int m_currentlyPointedAtSlice = 0;

    // Set to `true` to enable the slow idle spin animation
    bool m_idleSpin = Mod::get()->getSettingValue<bool>("initial-spinning");

    // Number of segments to use for drawing circles
    static constexpr unsigned int CircleSegmentCount = 65;

    // Maximum text scale for text on wheel slices
    static constexpr float MaxFontScale = 1.f;

    // Minimum time between playing a tick sound when the wheel spins
    static constexpr float TimePerTickSound = 0.03f;

    // Number of degrees to rotate per second when idly spinning
    static constexpr float IdleRotateRate = 6.0f;

    explicit PickerWheel(GJLevelList* list);

    void spinWheel(CCObject*);

    void updateAudio(float);

    static CCNode* generatePickerWheelCircle(float radius, const ccColor4F* color, const char* levelName);

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
