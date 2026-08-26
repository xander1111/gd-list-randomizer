#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class PickerWheel : public CCMenu
{
public:

    static PickerWheel* create(GJLevelList* list);

    bool init() override;

private:
    explicit PickerWheel(GJLevelList* list);

    void spinWheel(CCObject*);

    struct PickerWheelSlice
    {
        GJGameLevel* level;
        int weight;
        ccColor3B color;
    };

    GJLevelList* _list;
};
