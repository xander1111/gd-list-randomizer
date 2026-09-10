#pragma once

#include <Geode/Geode.hpp>

#include "../PickerWheel.h"

using namespace geode::prelude;

class WheelEditEntry : public CCNode
{
public:
    static WheelEditEntry* create(PickerWheel::Slice* slice, ccColor4F* color, float width);

    bool init() override;

    [[nodiscard]] PickerWheel::Slice* getSlice() const { return m_slice; }

    void setEnabled(bool enabled) const;

    void setSearchVisible(bool visible);

    void toggle(bool enabled, bool redrawWheel = true);

    [[nodiscard]] bool getSearchVisible() const { return m_searchVisible; };

    static constexpr float Height = 20.f;

private:
    explicit WheelEditEntry(PickerWheel::Slice* slice, ccColor4F* color, float width);

    void updateWeight(std::string const& text) const;

    void onToggle(CCObject*);

    PickerWheel::Slice* m_slice;
    float m_width;
    ccColor4F* m_color;

    bool m_searchVisible = true;

    /// We use our own `m_toggled` field to track whether this entry is toggled since toggler callbacks seem to get
    /// called before the toggler's `m_toggled` field gets updated. If we used the toggler's `m_toggled` field in a
    /// callback, it would give us the value that the toggler was previously set to, rather than the value it is being
    /// updated to.
    bool m_toggled = true;

    CCLayerColor* m_background = nullptr;
    CCMenu* m_optionsMenu = nullptr;
    TextInput* m_weightField = nullptr;
    CCMenuItemToggler* m_toggleButton = nullptr;
};
