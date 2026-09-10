#include "WheelFilterLayer.h"

#include "DimmingButton.h"
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
    rowTwo->setPosition({m_menuWidth / 2.f, m_menuHeight - 105.f});

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
    rowThree->setPosition({m_menuWidth / 2.f, m_menuHeight - 135.f});

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


    // Difficulty filters
    CCMenu* difficultyMenu = CCMenu::create();
    difficultyMenu->setID("difficulty-filters-menu"_spr);
    difficultyMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    difficultyMenu->setContentWidth(m_menuWidth);
    difficultyMenu->setPosition({m_menuWidth / 2.f, 110.f});

    DimmingButton* naToggler = DimmingButton::create(
        CCSprite::createWithSpriteFrameName("difficulty_00_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[NA] = button->isToggled(); }
    );
    naToggler->setID("na-toggler"_spr);
    naToggler->setScale(0.8f);
    difficultyMenu->addChild(naToggler);

    DimmingButton* easyToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_01_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[Easy] = button->isToggled(); }
    );
    easyToggler->setID("easy-toggler"_spr);
    easyToggler->setScale(0.8f);
    difficultyMenu->addChild(easyToggler);

    DimmingButton* normalToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_02_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[Normal] = button->isToggled(); }
    );
    normalToggler->setID("normal-toggler"_spr);
    normalToggler->setScale(0.8f);
    difficultyMenu->addChild(normalToggler);

    DimmingButton* hardToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_03_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[Hard] = button->isToggled(); }
    );
    hardToggler->setID("hard-toggler"_spr);
    hardToggler->setScale(0.8f);
    difficultyMenu->addChild(hardToggler);

    DimmingButton* harderToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_04_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[Harder] = button->isToggled(); }
    );
    harderToggler->setID("harder-toggler"_spr);
    harderToggler->setScale(0.8f);
    difficultyMenu->addChild(harderToggler);

    DimmingButton* insaneToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_05_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[Insane] = button->isToggled(); }
    );
    insaneToggler->setID("insane-toggler"_spr);
    insaneToggler->setScale(0.8f);
    difficultyMenu->addChild(insaneToggler);

    DimmingButton* easyDemonToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_07_btn2_001.png"),
        [this] (const DimmingButton* button) { m_filters[EasyDemon] = button->isToggled(); }
    );
    easyDemonToggler->setID("easy-demon-toggler"_spr);
    easyDemonToggler->setScale(0.8f);
    difficultyMenu->addChild(easyDemonToggler);

    DimmingButton* mediumDemonToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_08_btn2_001.png"),
        [this] (const DimmingButton* button) { m_filters[MediumDemon] = button->isToggled(); }
    );
    mediumDemonToggler->setID("medium-demon-toggler"_spr);
    mediumDemonToggler->setScale(0.8f);
    difficultyMenu->addChild(mediumDemonToggler);

    DimmingButton* hardDemonToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_06_btn2_001.png"),
        [this] (const DimmingButton* button) { m_filters[HardDemon] = button->isToggled(); }
    );
    hardDemonToggler->setID("hard-demon-toggler"_spr);
    hardDemonToggler->setScale(0.8f);
    difficultyMenu->addChild(hardDemonToggler);

    DimmingButton* insaneDemonToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_09_btn2_001.png"),
        [this] (const DimmingButton* button) { m_filters[InsaneDemon] = button->isToggled(); }
    );
    insaneDemonToggler->setID("insane-demon-toggler"_spr);
    insaneDemonToggler->setScale(0.8f);
    difficultyMenu->addChild(insaneDemonToggler);

    DimmingButton* extremeDemonToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_10_btn2_001.png"),
        [this] (const DimmingButton* button) { m_filters[ExtremeDemon] = button->isToggled(); }
    );
    extremeDemonToggler->setID("extreme-demon-toggler"_spr);
    extremeDemonToggler->setScale(0.8f);
    difficultyMenu->addChild(extremeDemonToggler);

    DimmingButton* autoToggler = DimmingButton::create(
    CCSprite::createWithSpriteFrameName("difficulty_auto_btn_001.png"),
        [this] (const DimmingButton* button) { m_filters[Auto] = button->isToggled(); }
    );
    autoToggler->setID("auto-toggler"_spr);
    autoToggler->setScale(0.8f);
    difficultyMenu->addChild(autoToggler);

    difficultyMenu->updateLayout();
    m_mainLayer->addChild(difficultyMenu);


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
        bool enabled = false;

        for (const auto& [filter, filterEnabled] : m_filters) {
            if (!filterEnabled)
                continue;

            GJGameLevel* level = entry->getSlice()->level;

            switch(filter) {
            case Completed:
                enabled = enabled || level->m_normalPercent == 100;
                break;

            case Uncompleted:
                enabled = enabled || level->m_normalPercent != 100;
                break;

            case Unrated:
                enabled = enabled || level->m_stars == 0;
                break;

            case StarRate:
                enabled = enabled || (level->m_stars > 0 && level->m_featured == 0);
                break;

            case Featured:
                enabled = enabled || (level->m_featured > 0 && level->m_isEpic == 0);
                break;

            case Epic:
                enabled = enabled || level->m_isEpic == 1;
                break;

            case Legendary:
                enabled = enabled || level->m_isEpic == 2;
                break;

            case Mythic:
                enabled = enabled || level->m_isEpic == 3;
                break;

            case NA:
                enabled = enabled || (level->getAverageDifficulty() == 0 && level->m_demon == 0);
                break;

            case Auto:
                enabled = enabled || level->m_autoLevel;
                break;

            case Easy:
                enabled = enabled || (level->getAverageDifficulty() == 1 && level->m_demon == 0 && !level->m_autoLevel);
                break;

            case Normal:
                enabled = enabled || (level->getAverageDifficulty() == 2 && level->m_demon == 0);
                break;

            case Hard:
                enabled = enabled || (level->getAverageDifficulty() == 3 && level->m_demon == 0);
                break;

            case Harder:
                enabled = enabled || (level->getAverageDifficulty() == 4 && level->m_demon == 0);
                break;

            case Insane:
                enabled = enabled || (level->getAverageDifficulty() == 5 && level->m_demon == 0);
                break;

            case EasyDemon:
                enabled = enabled || (level->m_demonDifficulty == 3 && level->m_demon != 0);
                break;

            case MediumDemon:
                enabled = enabled || (level->m_demonDifficulty == 4 && level->m_demon != 0);
                break;

            case HardDemon:
                enabled = enabled || (level->m_demonDifficulty == 0 && level->m_demon != 0);
                break;

            case InsaneDemon:
                enabled = enabled || (level->m_demonDifficulty == 5 && level->m_demon != 0);
                break;

            case ExtremeDemon:
                enabled = enabled || (level->m_demonDifficulty == 6 && level->m_demon != 0);
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
