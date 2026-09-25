#include "WheelLayer.h"

#include "PickerWheel.h"
#include "Utils.h"
#include "WheelTheme/WheelThemeEditLayer.h"

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


    const gd::string listId = Utils::getListId(m_list);

    // A little awkward to create a new object from an existing one here, but shouldn't really be an issue since a
    // single WheelTheme object isn't too large
    WheelTheme::currentTheme = new WheelTheme(Mod::get()->getSavedValue<WheelTheme>(listId + "-theme", *WheelTheme::getDefaultWheelTheme()));

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Background
    generateBackground(winSize);

    m_leftCornerDeco = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    m_rightCornerDeco = CCSprite::createWithSpriteFrameName("GJ_sideArt_001.png");

    m_leftCornerDeco->setID("left-corner"_spr);
    m_rightCornerDeco->setID("right-corner"_spr);

    m_leftCornerDeco->setAnchorPoint({0, 0});
    m_rightCornerDeco->setAnchorPoint({1, 0});

    m_leftCornerDeco->setPosition({-1,-1});
    m_rightCornerDeco->setPosition({winSize.width + 1,-1});

    m_rightCornerDeco->setFlipX(true);

    addChild(m_leftCornerDeco);
    addChild(m_rightCornerDeco);

    m_leftCornerDeco->setVisible(WheelTheme::currentTheme->showCornerDecorations);
    m_rightCornerDeco->setVisible(WheelTheme::currentTheme->showCornerDecorations);


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
    CCMenuItemSpriteExtra* exitButton = CCMenuItemExt::createSpriteExtra(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        std::bind_front(&WheelLayer::onBack, this)
    );
    exitButton->setID("exit-button"_spr);

    topMenu->addChild(exitButton);

    // Top-right menu, contains the wheel edit and theme edit buttons
    CCMenu* const topRightMenu = CCMenu::create();
    topRightMenu->setID("top-right-menu"_spr);
    topRightMenu->setLayout(
        RowLayout::create()
        ->setAutoScale(false)
        ->setAutoGrowAxis(0.f)
    );

    // Open edit menu button
    CCSprite* editButtonSprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    editButtonSprite->setScale(0.6f);

    CCMenuItemSpriteExtra* editButton = CCMenuItemExt::createSpriteExtra(
        editButtonSprite,
        std::bind_front(&WheelLayer::onEdit, this)
    );
    editButton->setID("edit-button"_spr);

    topRightMenu->addChild(editButton);

    // Edit theme button
    CCSprite* themeButtonSprite = CCSprite::createWithSpriteFrameName("GJ_paintBtn_001.png");
    themeButtonSprite->setScale(0.8f);

    CCMenuItemSpriteExtra* themeButton = CCMenuItemExt::createSpriteExtra(
        themeButtonSprite,
        std::bind_front(&WheelLayer::onThemeEdit, this)
    );
    themeButton->setID("theme-button"_spr);

    topRightMenu->addChild(themeButton);


    topRightMenu->updateLayout();

    topMenu->addChild(topRightMenu);

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

    // Set size to fit both the wheel and the edit menu, plus a gap of 10
    m_wheelAndEditMenu->setContentSize({winSize.height * 0.8f * 2.f + 10.f, winSize.height * 0.8f});

    // Picker wheel
    m_pickerWheel = PickerWheel::create(m_list, winSize.height * 0.4f);
    m_pickerWheel->setID("picker-wheel"_spr);
    m_pickerWheel->setPosition({m_wheelAndEditMenu->getContentWidth() / 2.f, m_wheelAndEditMenu->getContentHeight() / 2.f});

    m_wheelAndEditMenu->addChild(m_pickerWheel);

    // Wheel edit menu
    m_editMenuOpen = false;

    m_wheelEditMenu = WheelEditMenu::create(m_pickerWheel->getSlices(), winSize.height * 0.8f, winSize.height * 0.8f);
    m_wheelEditMenu->setID("wheel-edit-menu"_spr);
    m_wheelEditMenu->setVisible(false);
    // Start the edit menu off the side of the screen so it can be animated moving in later
    m_wheelEditMenu->setPosition({winSize.width + m_wheelEditMenu->getContentWidth(), m_wheelAndEditMenu->getContentHeight() / 2.f});

    m_pickerWheel->addOnWheelSpin(std::bind_front(&WheelEditMenu::onWheelSpin, m_wheelEditMenu));
    m_pickerWheel->addOnWheelSpinEnd(std::bind_front(&WheelEditMenu::onWheelSpinEnd, m_wheelEditMenu));

    m_wheelAndEditMenu->addChild(m_wheelEditMenu);

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

    listNameLabel->limitLabelWidth(240.f, 0.8f, 0.1f);

    titleMenu->addChild(listNameLabel);

    // List creator
    CCLabelBMFont* listCreatorLabel = CCLabelBMFont::create(m_list->m_creatorName.c_str(), "goldFont.fnt");

    listCreatorLabel->limitLabelWidth(90.f, 0.8f, 0.1f);

    CCMenuItemSpriteExtra* listCreatorButton = CCMenuItemExt::createSpriteExtra(
        listCreatorLabel,
        [this](CCMenuItemSpriteExtra*)
        {
            ProfilePage::create(m_list->m_accountID, false)->show();
        }
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
    CC_SAFE_DELETE(WheelTheme::currentTheme);
    CCDirector::sharedDirector()->popSceneWithTransition(0.5f, kPopTransitionFade);
}

void WheelLayer::updateTheme()
{
    removeChild(m_background, true);

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    generateBackground(winSize);

    m_leftCornerDeco->setVisible(WheelTheme::currentTheme->showCornerDecorations);
    m_rightCornerDeco->setVisible(WheelTheme::currentTheme->showCornerDecorations);

    m_pickerWheel->redrawWheel(true);
}

void WheelLayer::onBack(CCObject*)
{
    keyBackClicked();
}

void WheelLayer::onEdit(CCObject*)
{
    m_editMenuOpen = !m_editMenuOpen;

    // Edit menu should be made visible before moving on screen, but shouldn't be made invisible before moving off screen
    if (m_editMenuOpen)
        m_wheelEditMenu->setVisible(true);

    const CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // Find where the menus will end up
    const CCPoint wheelEndPoint = {
        m_editMenuOpen ? m_pickerWheel->getContentWidth() / 2.f : m_wheelAndEditMenu->getContentWidth() / 2.f,
        m_wheelAndEditMenu->getContentHeight() / 2.f
    };
    const CCPoint editEndPoint = {
        m_editMenuOpen ? m_wheelAndEditMenu->getContentWidth() - m_wheelEditMenu->getContentWidth() / 2.f : winSize.width + m_wheelEditMenu->getContentWidth(),
        m_wheelAndEditMenu->getContentHeight() / 2.f
    };

    CCEaseInOut* wheelMove = CCEaseInOut::create(CCMoveTo::create(.5f, wheelEndPoint), 2.0f);
    CCEaseInOut* editMove = CCEaseInOut::create(CCMoveTo::create(.5f, editEndPoint), 2.0f);

    const auto updateEditMenuVisibility = CallFuncExt::create([this]
    {
        // Edit menu should be made invisible after moving off screen
        m_wheelEditMenu->setVisible(m_editMenuOpen);
    });
    CCSequence* editMoveSeq = CCSequence::create(editMove, updateEditMenuVisibility, nullptr);

    m_pickerWheel->runAction(wheelMove);
    m_wheelEditMenu->runAction(editMoveSeq);
}

void WheelLayer::onThemeEdit(CCObject*)
{
    WheelThemeEditLayer::create(WheelTheme::currentTheme)->show();
}

void WheelLayer::generateBackground(const CCSize& winSize)
{
    m_background = CCSprite::create("GJ_gradientBG-hd.png");
    m_background->setID("background"_spr);

    const CCSize bgSize = m_background->getContentSize();

    m_background->setScaleX((winSize.width + 10) / bgSize.width);
    m_background->setScaleY((winSize.height + 10) / bgSize.height);

    m_background->setAnchorPoint({0, 0});
    m_background->setPosition({-5,-5});

    m_background->setColor(to3B(ccc4BFromccc4F(WheelTheme::currentTheme->backgroundColor)) /*{.r = 0, .g = 102, .b = 255}*/);

    m_background->setZOrder(-2);
    addChild(m_background);
}
