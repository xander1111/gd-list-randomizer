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

    // Default GD colors
    inline static auto DefaultListColorA = new ccColor4F(161.f / 255.f, 88.f / 255.f, 44.f / 255.f, 1.f);
    inline static auto DefaultListColorB = new ccColor4F(194.f / 255.f, 114.f / 255.f, 62.f / 255.f, 1.f);
    inline static auto DefaultOutlineColor = new ccColor4F(0.f, 0.f, 0.f, 1.f);
};
