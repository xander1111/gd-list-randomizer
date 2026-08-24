#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelListLayer.hpp>


class $modify(RandomizerLevelListLayer, LevelListLayer) {
	bool init(GJLevelList *list) {
		if (!LevelListLayer::init(list))
			return false;

		auto randomizerButtonSprite = ButtonSprite::create("Random", 0.3f);
		auto randomizerButton = CCMenuItemSpriteExtra::create(
			randomizerButtonSprite,
			this,
			menu_selector(RandomizerLevelListLayer::onButton)
		);

		auto menuLocation = this->getChildByID("right-side-menu");
		menuLocation->addChild(randomizerButton);
		randomizerButton->setID("random-button"_spr);
		menuLocation->updateLayout();

		return true;
	}

	void onButton(CCObject*) {
		auto levels = this->getChildByID("GJListLayer")
			->getChildByID("list-view")
			->getChildByType<TableView>(0)
			->getChildByType<CCContentLayer>(0)
			->getChildren();  // Should get all LevelCell objects in the list

		auto levelPicked = dynamic_cast<LevelCell *>(levels->randomObject())->m_level;

		auto levelScene = LevelInfoLayer::scene(levelPicked, false);
		auto transitionFade = CCTransitionFade::create(0.5, levelScene);
		CCDirector::sharedDirector()->pushScene(transitionFade);
	}
};
