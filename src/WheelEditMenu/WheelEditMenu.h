#pragma once

#include <alphalaneous.alphas-ui-pack/include/nodes/scroll/AdvancedScrollLayer.hpp>
#include <Geode/Geode.hpp>

#include "WheelEditEntry.h"
#include "../PickerWheel.h"

using namespace geode::prelude;

class WheelEditMenu : public CCMenu
{
public:
    static WheelEditMenu* create(std::vector<PickerWheel::Slice>* slices, float width, float height);

    bool init() override;

    void onWheelSpin() const;

    void onWheelSpinEnd() const;

private:
    explicit WheelEditMenu(std::vector<PickerWheel::Slice>* slices, float width, float height);

    void updateSearch(std::string const& input) const;

    void setListLayerContentSize() const;

    std::vector<PickerWheel::Slice>* m_slices;
    float m_width;
    float m_height;
    alpha::ui::AdvancedScrollLayer* m_levelListLayer = nullptr;
    std::vector<WheelEditEntry*> m_entries = {};
};
