#include "WheelFilterLayer.h"

#include "TogglerWithLabel.h"

WheelFilterLayer* WheelFilterLayer::create(CCArrayExt<WheelEditEntry*>* entries)
{
    auto ret = new WheelFilterLayer(entries);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

bool WheelFilterLayer::init()
{
    if (!Popup::init(440.f, 290.f))  // Same size as `MoreSearchLayer` (the advanced options menu on the search screen)
        return false;

    setTitle("Filter enabled levels");

    // Filters
    CCMenu* filtersMenu = CCMenu::create();
    filtersMenu->setID("filters-menu"_spr);
    filtersMenu->setContentSize({m_mainLayer->getContentWidth(), m_mainLayer->getContentHeight() * 0.8f});\
    filtersMenu->setPosition({0.f, 0.f});

    TogglerWithLabel* completedToggler = TogglerWithLabel::create(
        this,
        [this] (CCMenuItemToggler* toggler)
        {
            m_filterCompleted = toggler->isToggled();
        },
        "Completed"
    );
    completedToggler->toggle(true);

    filtersMenu->addChild(completedToggler);


    m_mainLayer->addChild(filtersMenu);

    return true;
}

WheelFilterLayer::WheelFilterLayer(CCArrayExt<WheelEditEntry*>* entries) : m_entries(entries) {}
