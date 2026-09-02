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

    static constexpr float Height = 30.f;

private:
    explicit WheelEditEntry(PickerWheel::Slice* slice, ccColor4F* color, float width);

    void updateWeight(std::string const& text) const;

    PickerWheel::Slice* m_slice;
    float m_width;
    ccColor4F* m_color;

    CCLayerColor* m_background = nullptr;
    TextInput* m_weightField = nullptr;
};
