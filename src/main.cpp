#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelListLayer.hpp>

#include "WheelLayer.h"


class $modify(RandomizerLevelListLayer, LevelListLayer) {
    // ReSharper disable once CppHidingFunction
    bool init(GJLevelList* list) {
		if (!LevelListLayer::init(list))
			return false;

		ButtonSprite* randomizerButtonSprite = ButtonSprite::create("Random", 0.3f);
		CCMenuItemSpriteExtra* randomizerButton = CCMenuItemSpriteExtra::create(
			randomizerButtonSprite,
			this,
			menu_selector(RandomizerLevelListLayer::onButton)
		);

	    // The server delete button is part of the button-menu instead of the right-side-menu for whatever reason.
	    // This causes an extra button added to the right-side-menu to overlap with it, so it has to be handled differently.
        const bool serverDeleteVisible = list->m_accountID == GJAccountManager::get()->m_accountID;
		CCNode* menuLocation = this->getChildByID(serverDeleteVisible ? "left-side-menu" : "right-side-menu");

		menuLocation->addChild(randomizerButton);
		randomizerButton->setID("random-button"_spr);
		menuLocation->updateLayout();

		return true;
	}

	void onButton(CCObject*) {
		CCArray* levels = this->getChildByID("GJListLayer")
			->getChildByID("list-view")
			->getChildByType<TableView>(0)
			->getChildByType<CCContentLayer>(0)
			->getChildren();  // Should get all LevelCell objects in the list

	    CCScene* scene = WheelLayer::scene(levels);
        CCTransitionFade* transitionFade = CCTransitionFade::create(0.5, scene);
        CCDirector::sharedDirector()->pushScene(transitionFade);
	}
};
