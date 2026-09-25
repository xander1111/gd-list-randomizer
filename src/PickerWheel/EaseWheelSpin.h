#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class EaseWheelSpin : public CCActionEase
{
public:
    static EaseWheelSpin* create(CCActionInterval* pAction);

    CCObject* copyWithZone(CCZone* pZone) override;
    CCActionInterval* reverse() override;

    void update(float time) override;
};
