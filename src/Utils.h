#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Utils
{
public:
    static void playSound(gd::string soundPath);
    static void playResourceSound(const gd::string& soundFileName);

    /**
     * Provides a way to get the frame for the icon of a given difficulty value.
     *
     * @remarks
     * There are functions @c GJDifficultySprite::getDifficultyFrame and @c GJLevelList::frameForListDifficulty that get
     * the frame for a difficulty number, but neither work properly, returning incorrect frames for multiple difficulties.
     * @c GJDifficultySprite::getDifficultyFrame also gives the frames for the difficulty icon with the difficulty name
     * below it, and we want just the icon, which is a different frame
     */
    static gd::string getDifficultyIconFrame(int difficulty);


    inline static std::string buttonTextures[] = {
        "GJ_button_01.png",
        "GJ_button_02.png",
        "GJ_button_03.png",
        "GJ_button_04.png",
        "GJ_button_05.png",
        "GJ_button_06.png",
    };
};
