#pragma once

#include <Geode/Geode.hpp>

#include "WheelTheme/WheelTheme.h"

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
    inline static auto DefaultTheme = new WheelTheme {
        .sliceColorCount = 2,
        .sliceColor1 = { .r = 161.f / 255.f, .g = 88.f / 255.f, .b = 44.f / 255.f, .a = 1.f },
        .sliceColor2 = { .r = 194.f / 255.f, .g = 114.f / 255.f, .b = 62.f / 255.f, .a = 1.f },

        .textColor = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f },

        .outlineColorInner = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f },
        .outlineColorOuter = { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f },

        .backgroundColor = { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f },
        .buttonColor = WheelTheme::Green,


        // .tickSound = ,
        // .selectSound = ,


        .spinSpeed = 1.f,
        .spinDuration = 7.f,


        .showCornerDecorations = true,
        .showLevelNamesOnWheel = true,
    };
};
