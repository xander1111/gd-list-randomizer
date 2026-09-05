#pragma once

#include <Geode/Geode.hpp>

#include "WheelEditMenu/WheelEditEntry.h"

using namespace geode::prelude;

class WheelFilterLayer : public Popup
{
public:
    static WheelFilterLayer* create(CCArrayExt<WheelEditEntry*>* entries);

    bool init() override;

private:
    explicit WheelFilterLayer(CCArrayExt<WheelEditEntry*>* entries);

    CCArrayExt<WheelEditEntry*>* m_entries;

    bool m_filterCompleted = false;
};
