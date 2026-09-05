#include "TogglerWithLabel.h"

#include <utility>

TogglerWithLabel* TogglerWithLabel::create(CCObject* target, Function<void(CCMenuItemToggler*)> callback,
    const gd::string& labelText, const float togglerScale, const float labelScale)
{
    auto ret = new TogglerWithLabel(target, std::move(callback), togglerScale, labelText, labelScale);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool TogglerWithLabel::init()
{
    if (!CCMenu::init())
        return false;

    setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setGap(10.f)
        ->setAutoGrowAxis(0.f)
    );

    m_toggler = CCMenuItemExt::createTogglerWithStandardSprites(m_togglerScale, std::move(m_callback));
    addChild(m_toggler);

    m_label = CCLabelBMFont::create(m_labelText.c_str(), "bigFont.fnt");
    m_label->setScale(m_labelScale);
    addChild(m_label);

    updateLayout();

    return true;
}

void TogglerWithLabel::toggle(const bool on) const
{
    m_toggler->toggle(on);
}

TogglerWithLabel::TogglerWithLabel(CCObject* target, Function<void(CCMenuItemToggler*)> callback,
                                   const float togglerScale, gd::string labelText, const float labelScale)
    : m_target(target), m_callback(std::move(callback)), m_togglerScale(togglerScale), m_labelScale(labelScale),
    m_labelText(std::move(labelText)) {}
