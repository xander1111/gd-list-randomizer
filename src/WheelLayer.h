#pragma once

#include <Geode/Geode.hpp>

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

    GJLevelList* m_list;
};
