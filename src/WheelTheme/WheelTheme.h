#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct WheelTheme
{
    enum ButtonColor
    {
        Green,
        Blue,
        Pink
    };

    // Colors

    unsigned int sliceColorCount;
    ccColor4F sliceColor1;
    ccColor4F sliceColor2;
    ccColor4F sliceColor3;
    ccColor4F sliceColor4;

    ccColor4F textColor;

    ccColor4F outlineColorInner;
    ccColor4F outlineColorOuter;

    ccColor4F backgroundColor;
    ButtonColor buttonColor;


    // Sounds
    // TODO figure out how to implement custom sounds
    //gd::string tickSound;
    //gd::string selectSound;


    // Wheel behaviour

    float spinSpeed;
    float spinDuration;


    // Misc

    bool showCornerDecorations;
    bool showLevelNamesOnWheel;
};
