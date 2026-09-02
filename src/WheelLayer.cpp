#include "WheelLayer.h"

#include "PickerWheel.h"
#include "Utils.h"

WheelLayer::WheelLayer(GJLevelList* list) : m_list(list) {}

WheelLayer* WheelLayer::create(GJLevelList* list)
{
    auto ret = new WheelLayer(list);
    if (ret && ret->init()) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

CCScene* WheelLayer::scene(GJLevelList* list)
{
    WheelLayer* layer = create(list);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool WheelLayer::init()
{
    if (!CCLayer::init()) return false;

    setKeypadEnabled(true);

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Background
    //
    // Recreates the background commonly used by the game for menus
    CCSprite* background = CCSprite::create("GJ_gradientBG-hd.png");
    background->setID("background"_spr);

    const CCSize bgSize = background->getContentSize();

    background->setScaleX((winSize.width + 10) / bgSize.width);
    background->setScaleY((winSize.height + 10) / bgSize.height);

    background->setAnchorPoint({0, 0});
    background->setPosition({-5,-5});

    background->setColor({.r = 0, .g = 102, .b = 255});

    background->setZOrder(-2);
    addChild(background);


    // Corner decorations
    CCSprite* leftCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    CCSprite* rightCorner = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");

    leftCorner->setID("left-corner"_spr);
    rightCorner->setID("right-corner"_spr);

    leftCorner->setAnchorPoint({0, 0});
    rightCorner->setAnchorPoint({1, 0});

    leftCorner->setPosition({-1,-1});
    rightCorner->setPosition({winSize.width + 1,-1});

    rightCorner->setFlipX(true);

    addChild(leftCorner);
    addChild(rightCorner);


    // Top of screen back button and edit button menu
    CCMenu* const topMenu = CCMenu::create();
    topMenu->setID("top-menu"_spr);
    topMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setPadding(Padding::horizontal(8.f))
        ->setAxisAlignment(AxisAlignment::Between)
    );
    topMenu->setPosition({0, winSize.height - 23});
    topMenu->setAnchorPoint({0.f, 0.5f});

    // Back button
    CCMenuItemSpriteExtra* exitButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(WheelLayer::onBack)
    );
    exitButton->setID("exit-button"_spr);

    topMenu->addChild(exitButton);

    // Open edit menu button
    // TODO
    CCSprite* editButtonSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    editButtonSprite->setScale(0.6f);

    CCMenuItemSpriteExtra* editButton = CCMenuItemSpriteExtra::create(
        editButtonSprite,
        this,
        menu_selector(WheelLayer::onEdit)
    );
    editButton->setID("edit-button"_spr);

    topMenu->addChild(editButton);

    topMenu->updateLayout();

    addChild(topMenu);


    // Layout to space list title and wheel properly
    CCMenu* wheelAndTitleMenu = CCMenu::create();
    wheelAndTitleMenu->setID("wheel-and-title-menu"_spr);
    wheelAndTitleMenu->setLayout(
        ColumnLayout::create()
            ->setAutoScale(false)
            ->setGap(15.f)
    );


    m_wheelAndEditMenu = CCMenu::create();
    m_wheelAndEditMenu->setID("wheel-and-edit-menu"_spr);
    m_wheelAndEditMenu->setLayout(RowLayout::create()->setAutoScale(false)->setGap(10.f));

    // Picker wheel
    PickerWheel* pickerWheel = PickerWheel::create(m_list, winSize.height * 0.4f);
    pickerWheel->setID("picker-wheel"_spr);

    m_wheelAndEditMenu->addChild(pickerWheel);

    // Wheel edit menu
    m_wheelEditMenu = WheelEditMenu::create(pickerWheel->getSlices(), winSize.height * 0.8f, winSize.height * 0.8f);
    m_wheelEditMenu->setID("wheel-edit-menu"_spr);
    m_wheelEditMenu->setVisible(m_editMenuOpen);
    m_wheelAndEditMenu->addChild(m_wheelEditMenu);


    m_wheelAndEditMenu->updateLayout();
    wheelAndTitleMenu->addChild(m_wheelAndEditMenu);


    // List title
    CCMenu* titleMenu = CCMenu::create();
    titleMenu->setID("title-menu"_spr);
    titleMenu->setLayout(
        RowLayout::create()
            ->setAutoScale(false)
            ->setGap(10.f)
    );

    // Difficulty icon
    CCMenu* difficultyIcon = CCMenu::create();
    difficultyIcon->setID("difficulty-icon"_spr);
    difficultyIcon->setLayout(AnchorLayout::create());
    difficultyIcon->setContentSize({30.f, 30.f});

    const gd::string difficultyFrame = Utils::getDifficultyIconFrame(m_list->m_difficulty);

    CCSprite* difficultySprite = CCSprite::createWithSpriteFrameName(difficultyFrame.c_str());
    difficultySprite->setID("difficulty-sprite"_spr);
    difficultySprite->setScale(0.8f);

    difficultyIcon->addChildAtPosition(difficultySprite, Anchor::Center);

    // Difficulty icon feature coin
    if (m_list->m_featured) {
        CCSprite* featureCoin = CCSprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
        featureCoin->setID("feature-coin"_spr);
        featureCoin->setScale(0.8f);
        featureCoin->setZOrder(-1);

        difficultyIcon->addChildAtPosition(featureCoin, Anchor::Center, {0.f, -4.8f});
    }

    difficultyIcon->updateLayout();
    titleMenu->addChild(difficultyIcon);

    // List name
    CCLabelBMFont* listNameLabel = CCLabelBMFont::create(m_list->m_listName.c_str(), "bigFont.fnt");
    listNameLabel->setID("title-label"_spr);

    const float listNameLabelScale = std::min(0.8f, 240.f / listNameLabel->getContentWidth());
    listNameLabel->setScale(listNameLabelScale);

    titleMenu->addChild(listNameLabel);

    // List creator
    CCLabelBMFont* listCreatorLabel = CCLabelBMFont::create(m_list->m_creatorName.c_str(), "goldFont.fnt");

    const float listCreatorLabelScale = std::min(0.8f, 90.f / listCreatorLabel->getContentWidth());
    listCreatorLabel->setScale(listCreatorLabelScale);

    CCMenuItemSpriteExtra* listCreatorButton = CCMenuItemSpriteExtra::create(
        listCreatorLabel,
        this,
        menu_selector(WheelLayer::onProfileClicked)
    );
    listCreatorButton->setID("creator-name"_spr);

    titleMenu->addChild(listCreatorButton);

    titleMenu->updateLayout();

    wheelAndTitleMenu->addChild(titleMenu);
    wheelAndTitleMenu->updateLayout();
    addChild(wheelAndTitleMenu);

    return true;
}

void WheelLayer::keyBackClicked()
{
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void WheelLayer::onBack(CCObject*)
{
    keyBackClicked();
}

void WheelLayer::onProfileClicked(CCObject*)
{
    ProfilePage::create(m_list->m_accountID, false)->show();
}

void WheelLayer::onEdit(CCObject*)
{
    m_editMenuOpen = !m_editMenuOpen;

    // TODO make edit menu appearing have an animation
    m_wheelEditMenu->setVisible(m_editMenuOpen);
    m_wheelAndEditMenu->updateLayout();
}
