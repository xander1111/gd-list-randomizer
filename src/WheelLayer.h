#pragma once

#include <Geode/Geode.hpp>

#include "WheelEditMenu/WheelEditMenu.h"
#include "WheelTheme/WheelTheme.h"

using namespace geode::prelude;

class WheelLayer : public CCLayer
{
public:
    PickerWheel* m_pickerWheel = nullptr;

    GJLevelList* m_list;

    explicit WheelLayer(GJLevelList* list);

    static WheelLayer* create(GJLevelList* list);
    static CCScene* scene(GJLevelList* list);

    bool init() override;
    void keyBackClicked() override;

    void updateTheme() const;

private:
    void onBack(CCObject*);

    void onProfileClicked(CCObject*);

    void onEdit(CCObject*);

    void onThemeEdit(CCObject*);

    CCMenu* m_wheelAndEditMenu = nullptr;

    WheelEditMenu* m_wheelEditMenu = nullptr;

    bool m_editMenuOpen = false;
};
