#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WheelLayer : public CCLayer
{
public:
    explicit WheelLayer(CCArray* levels);

    static WheelLayer* create(CCArray* levels);
    static CCScene* scene(CCArray* levels);

    bool init() override;
    void keyBackClicked() override;

private:
    void onBack(CCObject*);
    void onRandomizerButton(CCObject*);

    CCArray* _levels;
};
