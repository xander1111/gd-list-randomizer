#include "WheelFilterLayer.h"

#include "TogglerWithLabel.h"
#include "WheelLayer.h"

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
    filtersMenu->setContentSize({m_mainLayer->getContentWidth(), m_mainLayer->getContentHeight() * 0.7f});
    filtersMenu->setPosition({0.f, 290.f * 0.15f});

    TogglerWithLabel* completedToggler = TogglerWithLabel::create(
        this,
        [this] (const TogglerWithLabel* toggler)
        {
            m_filters["completed"] = toggler->m_toggled;
        },
        "Completed"
    );
    completedToggler->setID("completed-toggler"_spr);

    filtersMenu->addChild(completedToggler);

    m_mainLayer->addChild(filtersMenu);

    // Apply button
    ButtonSprite* applyButtonSprite = ButtonSprite::create("Apply", 0.5f);
    CCMenuItemSpriteExtra* applyButton = CCMenuItemSpriteExtra::create(
        applyButtonSprite,
        this,
        menu_selector(WheelFilterLayer::onApplyFilters)
    );
    applyButton->setID("apply-button"_spr);
    applyButton->setPosition({440.f / 2.f, 290.f * 0.1f});

    m_buttonMenu->addChild(applyButton);

    return true;
}

WheelFilterLayer::WheelFilterLayer(CCArrayExt<WheelEditEntry*>* entries) : m_entries(entries) {}

void WheelFilterLayer::onApplyFilters(CCObject* btn)
{
    for (const auto & entry : *m_entries) {
        bool enabled = true;

        for (const auto& [filter, filterEnabled] : m_filters) {
            if (!filterEnabled)
                continue;

            if (filter == "completed") {
                enabled = enabled && entry->getSlice()->level->m_normalPercent == 100;
            }
        }

        entry->toggle(enabled);
    }

    onClose(btn);
}
