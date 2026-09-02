#pragma once

#include <Geode/Geode.hpp>

#include "PickerWheel.h"

using namespace geode::prelude;

class SliceSelectedPopup : public Popup
{
public:
    static SliceSelectedPopup* create(PickerWheel::Slice* slice);

    bool init() override;

private:
    explicit SliceSelectedPopup(PickerWheel::Slice* slice);

    void onViewLevel(CCObject*);

    PickerWheel::Slice* m_slice;
};
