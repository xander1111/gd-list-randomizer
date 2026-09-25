#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Utils
{
public:
    static void playSound(std::string soundPath);
    static void playResourceSound(const std::string& soundFileName);

    /**
     * Provides a way to get the frame for the icon of a given difficulty value.
     *
     * @remarks
     * There are functions @c GJDifficultySprite::getDifficultyFrame and @c GJLevelList::frameForListDifficulty that get
     * the frame for a difficulty number, but neither work properly, returning incorrect frames for multiple difficulties.
     * @c GJDifficultySprite::getDifficultyFrame also gives the frames for the difficulty icon with the difficulty name
     * below it, and we want just the icon, which is a different frame
     */
    static std::string getDifficultyIconFrame(int difficulty);

    static std::string getListId(GJLevelList* list);

    enum ButtonColors : unsigned int {
        Green = 0,
        Cyan = 1,
        Pink = 2,
        Gray = 3,
        Blue = 4,
        Red = 5,
    };


    inline static std::string buttonTextures[] = {
        "GJ_button_01.png",
        "GJ_button_02.png",
        "GJ_button_03.png",
        "GJ_button_04.png",
        "GJ_button_05.png",
        "GJ_button_06.png",
    };
};
