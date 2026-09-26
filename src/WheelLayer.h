#pragma once

#include <Geode/Geode.hpp>

#include "WheelEditMenu/WheelEditMenu.h"

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

    void updateTheme();

private:
    CCMenu* m_wheelAndEditMenu = nullptr;

    WheelEditMenu* m_wheelEditMenu = nullptr;

    bool m_editMenuOpen = false;
    bool m_editMenuMoving = false;

    CCSprite* m_background = nullptr;
    CCSprite* m_leftCornerDeco =  nullptr;
    CCSprite* m_rightCornerDeco = nullptr;

    void onBack(CCObject*);

    void onEdit(CCObject*);

    void onThemeEdit(CCObject*);

    void generateBackground(const CCSize& winSize);
};
