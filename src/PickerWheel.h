#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PickerWheel : public CCMenu
{
public:

    static PickerWheel* create(GJLevelList* list);

    bool init() override;

private:
    struct PickerWheelSlice
    {
        GJGameLevel* level;
        unsigned int weight;
        ccColor4F* color;
    };

    static ccColor4F* _defaultSliceColor;
    static ccColor4F* _defaultSliceColorA;
    static ccColor4F* _defaultSliceColorB;
    std::vector<PickerWheelSlice> _slices;

    explicit PickerWheel(const GJLevelList* list);

    void spinWheel(CCObject*);

    [[nodiscard]] CCMenu* generateWheelSliceNodes(float windowHeight) const;
};
