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

    void onApplyFilters(CCObject* btn);

    CCArrayExt<WheelEditEntry*>* m_entries;

    std::unordered_map<std::string, bool> m_filters;
};
