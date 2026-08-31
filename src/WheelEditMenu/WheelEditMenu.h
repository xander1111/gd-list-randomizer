#pragma once

#include <Geode/Geode.hpp>

#include "../PickerWheel.h"

using namespace geode::prelude;

class WheelEditMenu : public CCMenu
{
public:
    static WheelEditMenu* create(std::vector<PickerWheel::PickerWheelSlice>* slices, float width, float height);

    bool init() override;

private:
    explicit WheelEditMenu(std::vector<PickerWheel::PickerWheelSlice>* slices, float width, float height);

    std::vector<PickerWheel::PickerWheelSlice>* m_slices;
    float m_width;
    float m_height;
};
