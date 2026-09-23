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

    setTitle("Filter Enabled Levels");

    InfoAlertButton* infoButton = InfoAlertButton::create(
        "Filter Menu",
        "Enables and disables levels based on filters.\n"
        "\n"
        "A level will be enabled if it matches at least one filter in each category. There are four categories:\n"
        "<cg>Completed</c>/<cr>Uncompleted</c>, <cy>Rate type</c>, <ca>Difficulty</c>, and <cb>Length</c>.\n"
        "Additionally, if no filters are enabled within a category, that category will be ignored.",
        1.f
    );
    infoButton->setID("info-button"_spr);
    infoButton->setPosition({m_menuWidth - 7.0f, m_menuHeight - 7.0f});
    infoButton->setAnchorPoint({1.f, 1.f});

    m_buttonMenu->addChild(infoButton);

    // Filters
    CCMenu* filtersMenu = CCMenu::create();
    filtersMenu->setID("filters-menu"_spr);
    filtersMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding({0.f, 40.f, 0.f, 60.f})
    );
    filtersMenu->setContentSize({m_menuWidth, m_menuHeight});
    filtersMenu->setPosition({0.f, 0.f});
    filtersMenu->setAnchorPoint({0.f, 0.f});

    // Completed filters
    CCMenu* completedMenu = CCMenu::create();
    completedMenu->setID("completed-filters-menu"_spr);
    completedMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    completedMenu->setContentWidth(m_menuWidth);
    completedMenu->setPosition({m_menuWidth / 2.f, m_menuHeight - 60.f});

    TogglerWithLabel* completedToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Completed] = toggler->m_toggled; },
        "Completed"
    );
    completedToggler->setID("completed-toggler"_spr);
    completedMenu->addChild(completedToggler);

    TogglerWithLabel* uncompletedToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Uncompleted] = toggler->m_toggled; },
        "Uncompleted"
    );
    uncompletedToggler->setID("uncompleted-toggler"_spr);
    completedMenu->addChild(uncompletedToggler);

    completedMenu->updateLayout();
    filtersMenu->addChild(completedMenu);


    // Rate type filters
    CCMenu* rateMenu = CCMenu::create();
    rateMenu->setID("rate-filters-menu"_spr);
    rateMenu->setLayout(
        ColumnLayout::create()
        ->setAxisReverse(true)
        ->setAutoScale(false)
        ->setAutoGrowAxis(0.f)
        ->setGap(5.f)
    );


    // First row
    CCMenu* rateRowOne = CCMenu::create();
    rateRowOne->setID("row-one"_spr);
    rateRowOne->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    rateRowOne->setContentWidth(m_menuWidth);

    TogglerWithLabel* unratedToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Unrated] = toggler->m_toggled; },
        "No Star"
    );
    unratedToggler->setID("unrated-toggler"_spr);
    rateRowOne->addChild(unratedToggler);

    TogglerWithLabel* starRateToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[StarRate] = toggler->m_toggled; },
        "Star Rated"
    );
    starRateToggler->setID("star-rate-toggler"_spr);
    rateRowOne->addChild(starRateToggler);

    TogglerWithLabel* featuredToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Featured] = toggler->m_toggled; },
        "Featured"
    );
    featuredToggler->setID("featured-toggler"_spr);
    rateRowOne->addChild(featuredToggler);

    rateRowOne->updateLayout();
    rateMenu->addChild(rateRowOne);

    // Second row
    CCMenu* rateRowTwo = CCMenu::create();
    rateRowTwo->setID("row-two"_spr);
    rateRowTwo->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setPadding(Padding::horizontal(10.f))
    );
    rateRowTwo->setContentWidth(m_menuWidth);

    TogglerWithLabel* epicToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Epic] = toggler->m_toggled; },
        "Epic"
    );
    epicToggler->setID("epic-toggler"_spr);
    rateRowTwo->addChild(epicToggler);

    TogglerWithLabel* legendaryToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Legendary] = toggler->m_toggled; },
        "Legendary"
    );
    legendaryToggler->setID("legendary-toggler"_spr);
    rateRowTwo->addChild(legendaryToggler);

    TogglerWithLabel* mythicToggler = TogglerWithLabel::create(
        [this] (const TogglerWithLabel* toggler) { m_filters[Mythic] = toggler->m_toggled; },
        "Mythic"
    );
    mythicToggler->setID("mythic-toggler"_spr);
    rateRowTwo->addChild(mythicToggler);

    rateRowTwo->updateLayout();
    rateMenu->addChild(rateRowTwo);


    rateMenu->updateLayout();
    filtersMenu->addChild(rateMenu);


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
    filtersMenu->addChild(difficultyMenu);


    // Length filters
    CCMenu* lengthMenu = CCMenu::create();
    lengthMenu->setID("length-filters-menu"_spr);
    lengthMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Between)
        ->setPadding(Padding::horizontal(20.f))
    );
    lengthMenu->setContentWidth(m_menuWidth);
    lengthMenu->setPosition({m_menuWidth / 2.f, 65.f});

    CCSprite* clockIcon = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
    clockIcon->setID("clock-icon"_spr);
    lengthMenu->addChild(clockIcon);

    DimmingButton* tinyToggler = DimmingButton::create(
    CCLabelBMFont::create("Tiny", "bigFont.fnt"),
        [this] (const DimmingButton* button) { m_filters[Tiny] = button->isToggled(); }
    );
    tinyToggler->setID("tiny-toggler"_spr);
    tinyToggler->setScale(0.5f);
    lengthMenu->addChild(tinyToggler);

    DimmingButton* shortToggler = DimmingButton::create(
    CCLabelBMFont::create("Short", "bigFont.fnt"),
        [this] (const DimmingButton* button) { m_filters[Short] = button->isToggled(); }
    );
    shortToggler->setID("short-toggler"_spr);
    shortToggler->setScale(0.5f);
    lengthMenu->addChild(shortToggler);

    DimmingButton* mediumToggler = DimmingButton::create(
    CCLabelBMFont::create("Medium", "bigFont.fnt"),
        [this] (const DimmingButton* button) { m_filters[Medium] = button->isToggled(); }
    );
    mediumToggler->setID("medium-toggler"_spr);
    mediumToggler->setScale(0.5f);
    lengthMenu->addChild(mediumToggler);

    DimmingButton* longToggler = DimmingButton::create(
    CCLabelBMFont::create("Long", "bigFont.fnt"),
        [this] (const DimmingButton* button) { m_filters[Long] = button->isToggled(); }
    );
    longToggler->setID("long-toggler"_spr);
    longToggler->setScale(0.5f);
    lengthMenu->addChild(longToggler);

    DimmingButton* xlToggler = DimmingButton::create(
    CCLabelBMFont::create("XL", "bigFont.fnt"),
        [this] (const DimmingButton* button) { m_filters[XL] = button->isToggled(); }
    );
    xlToggler->setID("xl-toggler"_spr);
    xlToggler->setScale(0.5f);
    lengthMenu->addChild(xlToggler);

    DimmingButton* platToggler = DimmingButton::create(
    CCLabelBMFont::create("Plat", "bigFont.fnt"),
        [this] (const DimmingButton* button) { m_filters[Plat] = button->isToggled(); }
    );
    platToggler->setID("plat-toggler"_spr);
    platToggler->setScale(0.5f);
    lengthMenu->addChild(platToggler);

    lengthMenu->updateLayout();
    filtersMenu->addChild(lengthMenu);


    filtersMenu->updateLayout();
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

    // Check if all filters in a category are disabled, if they all are, skip the category when filtering.
    //
    // IMO, if all filters in a category are disabled, it makes more sense to skip filtering in that category since
    // otherwise you would end up with no levels enabled. And, if for example, you wanted to filter to any levels you
    // haven't completed yet, you would have to go through and turn on every single filter in every other category.
    bool skipCompletion = true;
    bool skipRate = true;
    bool skipDifficulty = true;
    bool skipLength = true;

    for (const auto& filter : CompletionTypes) {
        if (m_filters[filter]) {
            skipCompletion = false;
            break;
        }
    }

    for (const auto& filter : RateTypes) {
        if (m_filters[filter]) {
            skipRate = false;
            break;
        }
    }

    for (const auto& filter : DifficultyTypes) {
        if (m_filters[filter]) {
            skipDifficulty = false;
            break;
        }
    }

    for (const auto& filter : LengthTypes) {
        if (m_filters[filter]) {
            skipLength = false;
            break;
        }
    }


    for (const auto & entry : *m_entries) {
        bool enabledCompletion = false;
        bool enabledRate = false;
        bool enabledDifficulty = false;
        bool enabledLength = false;

        for (const auto& [filter, filterEnabled] : m_filters) {
            if (!filterEnabled)
                continue;

            GJGameLevel* level = entry->getSlice()->level;

            switch(filter) {
            case Completed:
                enabledCompletion = enabledCompletion || GameStatsManager::get()->hasCompletedLevel(level);
                break;

            case Uncompleted:
                enabledCompletion = enabledCompletion || !GameStatsManager::get()->hasCompletedLevel(level);
                break;


            case Unrated:
                enabledRate = enabledRate || level->m_stars == 0;
                break;

            case StarRate:
                enabledRate = enabledRate || (level->m_stars > 0 && level->m_featured == 0);
                break;

            case Featured:
                enabledRate = enabledRate || (level->m_featured > 0 && level->m_isEpic == 0);
                break;

            case Epic:
                enabledRate = enabledRate || level->m_isEpic == 1;
                break;

            case Legendary:
                enabledRate = enabledRate || level->m_isEpic == 2;
                break;

            case Mythic:
                enabledRate = enabledRate || level->m_isEpic == 3;
                break;


            case NA:
                enabledDifficulty = enabledDifficulty || (level->getAverageDifficulty() == 0 && level->m_demon == 0);
                break;

            case Auto:
                enabledDifficulty = enabledDifficulty || level->m_autoLevel;
                break;

            case Easy:
                enabledDifficulty = enabledDifficulty || (level->getAverageDifficulty() == 1 && level->m_demon == 0 && !level->m_autoLevel);
                break;

            case Normal:
                enabledDifficulty = enabledDifficulty || (level->getAverageDifficulty() == 2 && level->m_demon == 0);
                break;

            case Hard:
                enabledDifficulty = enabledDifficulty || (level->getAverageDifficulty() == 3 && level->m_demon == 0);
                break;

            case Harder:
                enabledDifficulty = enabledDifficulty || (level->getAverageDifficulty() == 4 && level->m_demon == 0);
                break;

            case Insane:
                enabledDifficulty = enabledDifficulty || (level->getAverageDifficulty() == 5 && level->m_demon == 0);
                break;

            case EasyDemon:
                enabledDifficulty = enabledDifficulty || (level->m_demonDifficulty == 3 && level->m_demon != 0);
                break;

            case MediumDemon:
                enabledDifficulty = enabledDifficulty || (level->m_demonDifficulty == 4 && level->m_demon != 0);
                break;

            case HardDemon:
                enabledDifficulty = enabledDifficulty || (level->m_demonDifficulty == 0 && level->m_demon != 0);
                break;

            case InsaneDemon:
                enabledDifficulty = enabledDifficulty || (level->m_demonDifficulty == 5 && level->m_demon != 0);
                break;

            case ExtremeDemon:
                enabledDifficulty = enabledDifficulty || (level->m_demonDifficulty == 6 && level->m_demon != 0);
                break;


            case Tiny:
                enabledLength = enabledLength || level->m_levelLength == 0;
                break;

            case Short:
                enabledLength = enabledLength || level->m_levelLength == 1;
                break;

            case Medium:
                enabledLength = enabledLength || level->m_levelLength == 2;
                break;

            case Long:
                enabledLength = enabledLength || level->m_levelLength == 3;
                break;

            case XL:
                enabledLength = enabledLength || level->m_levelLength == 4;
                break;

            case Plat:
                enabledLength = enabledLength || level->m_levelLength == 5;
                break;


            default:
                log::debug("Unknown filter type used");
                break;
            }
        }

        entry->toggle(
            (enabledCompletion || skipCompletion)
            && (enabledRate || skipRate)
            && (enabledDifficulty || skipDifficulty)
            && (enabledLength || skipLength),
            false);
    }

    PickerWheel* pickerWheel = CCScene::get()->getChildByType<WheelLayer>()->m_pickerWheel;
    pickerWheel->redrawWheel();

    onClose(btn);
}
