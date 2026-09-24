#pragma once

#include <Geode/Geode.hpp>

#include "WheelEditMenu/WheelEditMenu.h"

using namespace geode::prelude;

class WheelLayer : public CCLayer
{
public:
    PickerWheel* m_pickerWheel = nullptr;

    explicit WheelLayer(GJLevelList* list);

    static WheelLayer* create(GJLevelList* list);
    static CCScene* scene(GJLevelList* list);

    bool init() override;
    void keyBackClicked() override;

    void updateTheme();

private:
    void onBack(CCObject*);

    void onProfileClicked(CCObject*);

    void onEdit(CCObject*);

    void onThemeEdit(CCObject*);

    GJLevelList* m_list;

    CCMenu* m_wheelAndEditMenu = nullptr;

    WheelEditMenu* m_wheelEditMenu = nullptr;

    bool m_editMenuOpen = false;
};
