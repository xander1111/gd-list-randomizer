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
        // Completion
        Completed,
        Uncompleted,

        // Rate type
        Unrated,
        StarRate,
        Featured,
        Epic,
        Legendary,
        Mythic,

        // Difficulty
        NA,
        Auto,
        Easy,
        Normal,
        Hard,
        Harder,
        Insane,
        EasyDemon,
        MediumDemon,
        HardDemon,
        InsaneDemon,
        ExtremeDemon,

        // Length
        Tiny,
        Short,
        Medium,
        Long,
        XL,
        Plat,
    };

    static constexpr FilterTypes CompletionTypes[] = {
        Completed,
        Uncompleted,
    };

    static constexpr FilterTypes RateTypes[] = {
        Unrated,
        StarRate,
        Featured,
        Epic,
        Legendary,
        Mythic,
    };

    static constexpr FilterTypes DifficultyTypes[] = {
        NA,
        Auto,
        Easy,
        Normal,
        Hard,
        Harder,
        Insane,
        EasyDemon,
        MediumDemon,
        HardDemon,
        InsaneDemon,
        ExtremeDemon,
    };

    static constexpr FilterTypes LengthTypes[] = {
        Tiny,
        Short,
        Medium,
        Long,
        XL,
        Plat,
    };

    // Same size as `MoreSearchLayer` (the advanced options menu on the search screen)
    static constexpr float m_menuWidth = 440.f;
    static constexpr float m_menuHeight = 290.f;

    CCArrayExt<WheelEditEntry*>* m_entries;

    std::unordered_map<FilterTypes, bool> m_filters;

    explicit WheelFilterLayer(CCArrayExt<WheelEditEntry*>* entries);

    void onApplyFilters(CCObject* btn);
};
