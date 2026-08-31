#pragma once

#include <Geode/Geode.hpp>

#include "../PickerWheel.h"

using namespace geode::prelude;

class WheelEditEntry : public CCNode
{
public:
    static WheelEditEntry* create(PickerWheel::PickerWheelSlice* slice, ccColor4F* color, float width, float height = 30.f);

    bool init() override;

private:
    explicit WheelEditEntry(PickerWheel::PickerWheelSlice* slice, ccColor4F* color, float width, float height = 30.f);

    PickerWheel::PickerWheelSlice* m_slice;
    float m_width;
    float m_height;
    ccColor4F* m_color;

    CCLayerColor* m_background = nullptr;
};
