#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelListLayer.hpp>

#include "WheelLayer.h"


class $modify(RandomizerLevelListLayer, LevelListLayer) {
    // ReSharper disable once CppHidingFunction
    bool init(GJLevelList* list) {
		if (!LevelListLayer::init(list))
			return false;

		CCMenuItemSpriteExtra* randomizerButton = CCMenuItemExt::createSpriteExtra(
			ButtonSprite::create("Random", 0.3f),
			[this](CCMenuItemSpriteExtra*)
			{
			    CCScene* scene = WheelLayer::scene(m_levelList);
                CCTransitionFade* transitionFade = CCTransitionFade::create(0.5, scene);
                CCDirector::sharedDirector()->pushScene(transitionFade);
			}
		);

        // The 'claim-button' at the end of the left-side-menu on a featured list isn't quite sized properly, so we need
        // to add a bit of spacing to not overlap with that button
        randomizerButton->setLayoutOptions(AxisLayoutOptions::create()->setNextGap(15.f));

        CCNode* menuLocation = getChildByID("left-side-menu");

		menuLocation->addChild(randomizerButton);
		randomizerButton->setID("random-button"_spr);
		menuLocation->updateLayout();

		return true;
	}
};
