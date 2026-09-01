#pragma once

#include <Geode/Geode.hpp>

#include "../PickerWheel.h"

using namespace geode::prelude;

class WheelEditEntry : public CCNode
{
public:
    static WheelEditEntry* create(PickerWheel::Slice* slice, ccColor4F* color, float width, float height = 30.f);

    bool init() override;

    [[nodiscard]] PickerWheel::Slice* getSlice() const { return m_slice; }

private:
    explicit WheelEditEntry(PickerWheel::Slice* slice, ccColor4F* color, float width, float height = 30.f);

    void updateWeight(std::string const& text) const;

    PickerWheel::Slice* m_slice;
    float m_width;
    float m_height;
    ccColor4F* m_color;

    CCLayerColor* m_background = nullptr;
};
