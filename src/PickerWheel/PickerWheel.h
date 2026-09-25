#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PickerWheel : public CCMenu
{
public:
    struct SliceSettings
    {
        unsigned int weight;
        bool enabled;
    };

    struct Slice
    {
        // I think this doesn't need to be a `Ref` since it's theoretically held by the `GJLevelList` we get it from
        GJGameLevel* level;
        SliceSettings settings;

        ccColor4F* color;  // The color that the slice was most recently rendered with

        mutable float startAngleDeg;
        mutable float endAngleDeg;
    };

    static PickerWheel* create(GJLevelList* list, float radius);

    bool init() override;

    void update(float dt) override;

    std::vector<Slice>* getSlices() { return &m_slices; }

    void redrawWheel(bool fullRedraw = false);

    void addOnWheelSpin(const std::function<void()>& function);

    void addOnWheelSpinEnd(const std::function<void()>& function);

private:
    GJLevelList* m_list;

    std::vector<Slice> m_slices;

    CCNode* m_slicesNode = nullptr;

    CCMenu* m_wheelOuterMenu = nullptr;

    // Doesn't need to be a `Ref` since it gets added as a child of `this`
    CCMenu* m_wheelMenu = nullptr;

    CCDrawNode* m_ticker = nullptr;

    // End separator node, may be nullptr if the end separator was not needed when the wheel was last drawn
    CCDrawNode* m_endSeparator = nullptr;

    Slice* m_firstEnabledSlice = nullptr;
    Slice* m_lastEnabledSlice = nullptr;

    // Tracks the index of the slice that the wheel ticker is currently pointing at
    unsigned int m_currentlyPointedAtSlice = 0;

    // Tracks the absolute (mod 360 degrees) wheel rotation at last update
    float m_lastRotation = 0.0f;

    // Set to `true` to enable the slow idle spin animation
    bool m_idleSpin = Mod::get()->getSettingValue<bool>("initial-spinning");

    // True when the wheel is currently being spun to select a level
    bool m_spinning = false;

    // Set to true to cause a tick sound to be played on the next audio update
    bool m_playTick = false;

    float m_radius;

    unsigned int m_totalWeight = 0;

    unsigned int m_enabledSliceCount = 0;

    std::vector<std::function<void()>> m_onWheelSpinFuncs;

    std::vector<std::function<void()>> m_onWheelSpinEndFuncs;

    // Reference needed for removing and replacing when drawing the wheel
    CCMenuItemSpriteExtra* m_spinButton = nullptr;

    // Number of segments to use for drawing circles
    static constexpr unsigned int CircleSegmentCount = 65;

    // Maximum text scale for text on wheel slices
    static constexpr float MaxFontScale = 1.f;

    // Minimum time between playing a tick sound when the wheel spins
    static constexpr float TimePerTickSound = 0.03f;

    // Number of degrees to rotate per second when idly spinning
    static constexpr float IdleRotateRate = 6.0f;

    explicit PickerWheel(GJLevelList* list, float radius);

    void onSpinWheel(CCObject*);

    void updateAudio(float);

    void updateCurrentlyPointedAtSlice();

    void saveSettings() const;

    void redrawTicker();

    [[nodiscard]] bool needsEndSeparator() const;

    void addEndSeparator();

    void generateSpinButton();

    CCNode* generatePickerWheelCircle(const ccColor4F* color, const char* levelName) const;

    /**
     * Generates the wheel visuals, including a circle with a section for each slice in `_slices`
     *
     * @remarks Additionally sets the `angleStartDeg` and `angleEndDeg` values of each slice in `_slices` to match the
     * angle range that particular slice occupies
     *
     * @return a CCMenu object that contains the wheel slices
     */
    [[nodiscard]] CCMenu* generateWheelSliceNodes();

    void generateOutline();

    void generateTicker();
};


// Wheel configuration saving
template<>
struct matjson::Serialize<PickerWheel::SliceSettings>
{
    static Result<PickerWheel::SliceSettings> fromJson(Value const& value);

    static Value toJson(PickerWheel::SliceSettings const& value);
};
