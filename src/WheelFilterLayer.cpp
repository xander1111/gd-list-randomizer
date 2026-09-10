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
    if (!Popup::init(m_menuWidth, m_menuHeight))
        return false;

    setTitle("Filter enabled levels");

    // Filters
    CCMenu* filtersMenu = CCMenu::create();
    filtersMenu->setID("filters-menu"_spr);
    filtersMenu->setContentSize({m_mainLayer->getContentWidth(), m_menuHeight * m_filtersMenuHeightRatio});
    filtersMenu->setPosition({0.f, m_menuHeight * (m_filtersMenuHeightRatio / 2.f)});


    // First row
    CCMenu* rowOne = CCMenu::create();
    rowOne->setID("row-one"_spr);
    rowOne->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
    );
    rowOne->setContentWidth(m_menuWidth * 0.8f);
    rowOne->setPosition({m_menuWidth / 2.f, m_menuHeight * m_filtersMenuHeightRatio});

    TogglerWithLabel* completedToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Completed] = toggler->m_toggled; },
        "Completed"
    );
    completedToggler->setID("completed-toggler"_spr);
    rowOne->addChild(completedToggler);

    TogglerWithLabel* uncompletedToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Uncompleted] = toggler->m_toggled; },
        "Uncompleted"
    );
    uncompletedToggler->setID("uncompleted-toggler"_spr);
    rowOne->addChild(uncompletedToggler);

    TogglerWithLabel* coinsToggler = TogglerWithLabel::create(
    [this] (const TogglerWithLabel* toggler) { m_filters[Coins] = toggler->m_toggled; },
    "Coins"
);
    coinsToggler->setID("coins-toggler"_spr);
    rowOne->addChild(coinsToggler);

    rowOne->updateLayout();
    m_mainLayer->addChild(rowOne);

    m_mainLayer->addChild(filtersMenu);

    // Apply button
    ButtonSprite* applyButtonSprite = ButtonSprite::create("Apply", 0.5f);
    CCMenuItemSpriteExtra* applyButton = CCMenuItemSpriteExtra::create(
        applyButtonSprite,
        this,
        menu_selector(WheelFilterLayer::onApplyFilters)
    );
    applyButton->setID("apply-button"_spr);
    applyButton->setPosition({m_menuWidth / 2.f, m_menuHeight * 0.1f});

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

            switch(filter) {
            case Completed:
                enabled = enabled && entry->getSlice()->level->m_normalPercent == 100;
                break;

            case Uncompleted:
                enabled = enabled && entry->getSlice()->level->m_normalPercent != 100;
                break;

            case Coins:
                enabled = enabled && entry->getSlice()->level->m_coins > 0;
                break;

            default:
                log::debug("Unimplemented filter type used");
                break;
            }
        }

        entry->toggle(enabled, false);
    }

    PickerWheel* pickerWheel = CCScene::get()->getChildByType<WheelLayer>()->m_pickerWheel;
    pickerWheel->redrawWheel();

    onClose(btn);
}
