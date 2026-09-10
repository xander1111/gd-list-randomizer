#include "TogglerWithLabel.h"

#include <utility>

TogglerWithLabel* TogglerWithLabel::create(Function<void(TogglerWithLabel*)> callback,
    const gd::string& labelText, const float togglerScale, const float labelScale)
{
    auto ret = new TogglerWithLabel(std::move(callback), togglerScale, labelText, labelScale);
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
        ->setAxisAlignment(AxisAlignment::Start)
    );

    setContentWidth(100.f);

    m_toggler = CCMenuItemExt::createTogglerWithStandardSprites(
        m_togglerScale,
        [this](CCObject*)
        {
            m_toggled = !m_toggled;
            m_callback(this);
        }
    );
    addChild(m_toggler);

    m_label = CCLabelBMFont::create(m_labelText.c_str(), "bigFont.fnt");
    m_label->limitLabelWidth(100.f - m_toggler->getContentWidth() - 10.f, m_labelScale, 0.15f);
    addChild(m_label);

    updateLayout();

    return true;
}

void TogglerWithLabel::toggle(const bool on) const
{
    m_toggler->toggle(on);
}

void TogglerWithLabel::toggleWithCallback(const bool on)
{
    toggle(on);
    m_callback(this);
}

TogglerWithLabel::TogglerWithLabel(Function<void(TogglerWithLabel*)> callback,
    const float togglerScale, gd::string labelText, const float labelScale)
    : m_callback(std::move(callback)), m_togglerScale(togglerScale), m_labelScale(labelScale),
    m_labelText(std::move(labelText)) {}
