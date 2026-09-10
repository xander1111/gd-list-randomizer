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

    // First row
    CCMenu* rowOne = CCMenu::create();
    rowOne->setID("filters-row-one"_spr);
    rowOne->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    rowOne->setContentWidth(m_menuWidth);
    rowOne->setPosition({m_menuWidth / 2.f, m_menuHeight - 60.f});

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


    // Second row
    CCMenu* rowTwo = CCMenu::create();
    rowTwo->setID("filters-row-two"_spr);
    rowTwo->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    rowTwo->setContentWidth(m_menuWidth);
    rowTwo->setPosition({m_menuWidth / 2.f, m_menuHeight - 90.f});

    TogglerWithLabel* unratedToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Unrated] = toggler->m_toggled; },
        "No Star"
    );
    unratedToggler->setID("unrated-toggler"_spr);
    rowTwo->addChild(unratedToggler);

    TogglerWithLabel* starRateToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[StarRate] = toggler->m_toggled; },
        "Star Rated"
    );
    starRateToggler->setID("star-rate-toggler"_spr);
    rowTwo->addChild(starRateToggler);

    TogglerWithLabel* featuredToggler = TogglerWithLabel::create(
    [this] (const TogglerWithLabel* toggler) { m_filters[Featured] = toggler->m_toggled; },
    "Featured"
);
    featuredToggler->setID("featured-toggler"_spr);
    rowTwo->addChild(featuredToggler);

    rowTwo->updateLayout();
    m_mainLayer->addChild(rowTwo);


    // Third row
    CCMenu* rowThree = CCMenu::create();
    rowThree->setID("filters-row-three"_spr);
    rowThree->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    rowThree->setContentWidth(m_menuWidth);
    rowThree->setPosition({m_menuWidth / 2.f, m_menuHeight - 120.f});

    TogglerWithLabel* epicToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Epic] = toggler->m_toggled; },
        "Epic"
    );
    epicToggler->setID("epic-toggler"_spr);
    rowThree->addChild(epicToggler);

    TogglerWithLabel* legendaryToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Legendary] = toggler->m_toggled; },
        "Legendary"
    );
    legendaryToggler->setID("legendary-toggler"_spr);
    rowThree->addChild(legendaryToggler);

    TogglerWithLabel* mythicToggler = TogglerWithLabel::create(
    [this] (const TogglerWithLabel* toggler) { m_filters[Mythic] = toggler->m_toggled; },
    "Mythic"
);
    mythicToggler->setID("mythic-toggler"_spr);
    rowThree->addChild(mythicToggler);

    rowThree->updateLayout();
    m_mainLayer->addChild(rowThree);


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

            GJGameLevel* level = entry->getSlice()->level;

            switch(filter) {
            case Completed:
                enabled = enabled && level->m_normalPercent == 100;
                break;

            case Uncompleted:
                enabled = enabled && level->m_normalPercent != 100;
                break;

            case Coins:
                enabled = enabled && level->m_coins > 0;
                break;

            case Unrated:
                enabled = enabled && level->m_stars == 0;
                break;

            case StarRate:
                enabled = enabled && level->m_stars > 0;
                break;

            case Featured:
                enabled = enabled && level->m_featured > 0;
                break;

            case Epic:
                enabled = enabled && level->m_isEpic > 0;
                break;

            case Legendary:
                enabled = enabled && level->m_isEpic > 1;
                break;

            case Mythic:
                enabled = enabled && level->m_isEpic > 2;
                break;

            default:
                log::debug("Unknown filter type used");
                break;
            }
        }

        entry->toggle(enabled, false);
    }

    PickerWheel* pickerWheel = CCScene::get()->getChildByType<WheelLayer>()->m_pickerWheel;
    pickerWheel->redrawWheel();

    onClose(btn);
}
