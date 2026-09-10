#include "DimmingButton.h"

DimmingButton* DimmingButton::create(CCNode* sprite, Function<void(DimmingButton*)> callback, const bool on)
{
    auto ret = new DimmingButton(std::move(callback));
    if (ret && ret->init(sprite, on)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool DimmingButton::init(CCNode* sprite, const bool on)
{
    if (!CCMenu::init())
        return false;

    m_button = CCMenuItemExt::createSpriteExtra(
        sprite,
        [this] (CCMenuItemSprite* button)
        {
            toggleWithCallback(!m_toggled);
        }
    );

    setContentSize(m_button->getContentSize());

    m_button->setPosition({getContentSize() / 2.f});

    toggle(on);

    addChild(m_button);

    return true;
}

void DimmingButton::toggle(const bool on)
{
    m_toggled = on;

    if (m_toggled)
        m_button->setColor({.r = 255, .g = 255, .b = 255});
    else
        m_button->setColor({.r = 125, .g = 125, .b = 125});
}

void DimmingButton::toggleWithCallback(const bool on)
{
    toggle(on);
    m_callback(this);
}

DimmingButton::DimmingButton(Function<void(DimmingButton*)> callback)
    : m_callback(std::move(callback)){}
