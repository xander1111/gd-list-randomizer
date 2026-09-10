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
    enum FilterTypes {
        Completed,
        Uncompleted,
        Coins,
        Unrated,
        StarRate,
        Featured,
        Epic,
        Legendary,
        Mythic,
    };

    // Same size as `MoreSearchLayer` (the advanced options menu on the search screen)
    static constexpr float m_menuWidth = 440.f;
    static constexpr float m_menuHeight = 290.f;

    CCArrayExt<WheelEditEntry*>* m_entries;

    std::unordered_map<FilterTypes, bool> m_filters;

    explicit WheelFilterLayer(CCArrayExt<WheelEditEntry*>* entries);

    void onApplyFilters(CCObject* btn);
};
