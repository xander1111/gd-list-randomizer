#pragma once

#include <Geode/Geode.hpp>

#include "WheelEditMenu/WheelEditMenu.h"

using namespace geode::prelude;

class WheelLayer : public CCLayer
{
public:
    explicit WheelLayer(GJLevelList* list);

    static WheelLayer* create(GJLevelList* list);
    static CCScene* scene(GJLevelList* list);

    bool init() override;
    void keyBackClicked() override;

private:
    void onBack(CCObject*);

    void openProfile(CCObject*);

    GJLevelList* m_list;

    WheelEditMenu* m_wheelEditMenu = nullptr;

    bool m_editMenuOpen = true;  // TODO update to false once button to open edit menu is added
};
