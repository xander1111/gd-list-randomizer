#pragma once

#include <Geode/Geode.hpp>

#include "PickerWheel.h"

using namespace geode::prelude;

class SliceSelectedPopup : public Popup
{
public:
    static SliceSelectedPopup* create(PickerWheel::PickerWheelSlice* slice);

    bool init(PickerWheel::PickerWheelSlice* slice);

private:
    explicit SliceSelectedPopup(PickerWheel::PickerWheelSlice* slice);

    void onViewLevel(CCObject*);

    PickerWheel::PickerWheelSlice* _slice;
};
