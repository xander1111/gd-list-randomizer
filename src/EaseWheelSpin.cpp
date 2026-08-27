#include "EaseWheelSpin.h"

EaseWheelSpin* EaseWheelSpin::create(CCActionInterval* pAction)
{
    auto pRet = new EaseWheelSpin();
    if (pRet) {
        if (pRet->initWithAction(pAction)) {
            pRet->autorelease();
        } else {
            CC_SAFE_RELEASE_NULL(pRet);
        }
    }

    return pRet;
}

// This function is just copied from other CCActionEase subclasses in the cocos2dx source code
CCObject* EaseWheelSpin::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = nullptr;
    EaseWheelSpin* pCopy = nullptr;
    if(pZone && pZone->m_pCopyObject) {
        //in case of being called at sub class
        pCopy = typeinfo_cast<EaseWheelSpin*>(pZone->m_pCopyObject);
    } else {
        pCopy = new EaseWheelSpin();
        pNewZone = new CCZone(pCopy);
    }

    pCopy->initWithAction(typeinfo_cast<CCActionInterval*>(m_pInner->copy()->autorelease()));

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

// This function is also just copied from other CCActionEase subclasses in the cocos2dx source code
CCActionInterval* EaseWheelSpin::reverse()
{
    return create(m_pInner->reverse());
}

void EaseWheelSpin::update(float time)
{
    // Graph that I used to make this equation can be found here: https://www.desmos.com/calculator/pgxa9wqvrz
    time *= 8.f;

    if (time < 1) {
        time = powf(time, 3.f);
    } else {
        time = 3.f * powf(time - 8.f, 5.f) / 12005.f + 5.2f;
    }

    m_pInner->update(time);
}
