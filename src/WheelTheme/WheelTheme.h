#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WheelTheme
{
public:
    // Colors

    unsigned int sliceColorCount;
    ccColor4F sliceColor1;
    ccColor4F sliceColor2;
    ccColor4F sliceColor3;
    ccColor4F sliceColor4;

    ccColor4F textColor;

    ccColor4F backgroundColor;
    unsigned int buttonColor;


    // Wheel behaviour

    float spinDuration;


    // Misc

    bool showCornerDecorations;
    bool showLevelNamesOnWheel;

    // Seems to be needed for serialization
    [[maybe_unused]] WheelTheme() = default;

    WheelTheme(const unsigned int sliceColorCount, const ccColor4F& sliceColor1, const ccColor4F& sliceColor2,
        const ccColor4F& sliceColor3, const ccColor4F& sliceColor4, const ccColor4F& textColor,
        const ccColor4F& backgroundColor, const unsigned int buttonColor, const float spinDuration,
        const bool showCornerDecorations, const bool showLevelNamesOnWheel)
        : sliceColorCount(sliceColorCount),
          sliceColor1(sliceColor1),
          sliceColor2(sliceColor2),
          sliceColor3(sliceColor3),
          sliceColor4(sliceColor4),
          textColor(textColor),
          backgroundColor(backgroundColor),
          buttonColor(buttonColor),
          spinDuration(spinDuration),
          showCornerDecorations(showCornerDecorations),
          showLevelNamesOnWheel(showLevelNamesOnWheel) {}

    static WheelTheme* getDefaultWheelTheme()
    {
        if (defaultTheme == nullptr)
            defaultTheme = generateDefaultTheme();
        return defaultTheme;
    }

    inline static WheelTheme* currentTheme = nullptr;

private:
    inline static WheelTheme* defaultTheme = nullptr;

    static WheelTheme* generateDefaultTheme()
    {
        return new WheelTheme(
            2,
            { .r = 161.f / 255.f, .g = 88.f / 255.f, .b = 44.f / 255.f, .a = 1.f },
            { .r = 194.f / 255.f, .g = 114.f / 255.f, .b = 62.f / 255.f, .a = 1.f },
            { .r = 0, .g = 0, .b = 0, .a = 1.f },
            { .r = 0, .g = 0, .b = 0, .a = 1.f },

            { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f },

            { .r = 0, .g = 102.f / 255.f, .b = 1.f, .a = 1.f },
            0,


            // .tickSound = ,
            // .selectSound = ,


            7.f,


            true,
            true
        );
    }
};

// Wheel theme saving

template<>
struct matjson::Serialize<ccColor4F>
{
    static Result<ccColor4F> fromJson(Value const& value)
    {
        if (!value.isObject()) return Err("not an object");

        float r, g, b, a;


        GEODE_UNWRAP_INTO_OR_ELSE(r, err, value["r"].asDouble())
        {
            log::info("Invalid color red data, using default value of 0");
            r = 0.;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(g, err, value["g"].asDouble()) {
            log::info("Invalid color green data, using default value of 0");
            g = 0.;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(b, err, value["b"].asDouble()) {
            log::info("Invalid color blue data, using default value of 0");
            b = 0.;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(a, err, value["a"].asDouble()) {
            log::info("Invalid color alpha data, using default value of 1");
            a = 1.;
        }

        return Ok(ccColor4F {
            .r = r,
            .g = g,
            .b = b,
            .a = a
        });
    }

    static Value toJson(ccColor4F const& value)
    {
        auto obj = Value();

        obj["r"] = value.r;
        obj["g"] = value.g;
        obj["b"] = value.b;
        obj["a"] = value.a;

        return obj;
    }
};

template<>
struct matjson::Serialize<WheelTheme>
{
    static Result<WheelTheme> fromJson(Value const& value)
    {
        if (!value.isObject()) return Err("not an object");

        unsigned int sliceColorCount;
        ccColor4F sliceColor1, sliceColor2, sliceColor3, sliceColor4;
        ccColor4F textColor;
        ccColor4F backgroundColor;
        unsigned int buttonColor;
        float spinDuration;
        bool showCornerDecorations, showLevelNamesOnWheel;


        GEODE_UNWRAP_INTO_OR_ELSE(sliceColorCount, err, value["sliceColorCount"].asUInt()) {
            log::info("Invalid theme sliceColorCount data, using default theme value");
            sliceColorCount = WheelTheme::getDefaultWheelTheme()->sliceColorCount;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(sliceColor1, err, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor1"])) {
            log::info("Invalid theme sliceColor1 data, using default theme value");
            sliceColor1 = WheelTheme::getDefaultWheelTheme()->sliceColor1;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(sliceColor2, err, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor2"])) {
            log::info("Invalid theme sliceColor2 data, using default theme value");
            sliceColor2 = WheelTheme::getDefaultWheelTheme()->sliceColor2;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(sliceColor3, err, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor3"])) {
            log::info("Invalid theme sliceColor3 data, using default theme value");
            sliceColor3 = WheelTheme::getDefaultWheelTheme()->sliceColor3;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(sliceColor4, err, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor4"])) {
            log::info("Invalid theme sliceColor4 data, using default theme value");
            sliceColor4 = WheelTheme::getDefaultWheelTheme()->sliceColor4;
        }


        GEODE_UNWRAP_INTO_OR_ELSE(textColor, err, matjson::Serialize<ccColor4F>::fromJson(value["textColor"])) {
            log::info("Invalid theme textColor data, using default theme value");
            textColor = WheelTheme::getDefaultWheelTheme()->textColor;
        }


        GEODE_UNWRAP_INTO_OR_ELSE(backgroundColor, err, matjson::Serialize<ccColor4F>::fromJson(value["backgroundColor"])) {
            log::info("Invalid theme backgroundColor data, using default theme value");
            backgroundColor = WheelTheme::getDefaultWheelTheme()->backgroundColor;
        }


        GEODE_UNWRAP_INTO_OR_ELSE(buttonColor, err, value["buttonColor"].asUInt()) {
            log::info("Invalid theme buttonColor data, using default theme value");
            buttonColor = WheelTheme::getDefaultWheelTheme()->buttonColor;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(spinDuration, err, value["spinDuration"].asDouble()) {
            log::info("Invalid theme spinDuration data, using default theme value");
            spinDuration = WheelTheme::getDefaultWheelTheme()->spinDuration;
        }


        GEODE_UNWRAP_INTO_OR_ELSE(showCornerDecorations, err, value["showCornerDecorations"].asBool()) {
            log::info("Invalid theme showCornerDecorations data, using default theme value");
            showCornerDecorations = WheelTheme::getDefaultWheelTheme()->showCornerDecorations;
        }

        GEODE_UNWRAP_INTO_OR_ELSE(showLevelNamesOnWheel, err, value["showLevelNamesOnWheel"].asBool()) {
            log::info("Invalid theme showLevelNamesOnWheel data, using default theme value");
            showLevelNamesOnWheel = WheelTheme::getDefaultWheelTheme()->showLevelNamesOnWheel;
        }

        return Ok(WheelTheme(
            sliceColorCount,
            sliceColor1,
            sliceColor2,
            sliceColor3,
            sliceColor4,

            textColor,

            backgroundColor,

            buttonColor,

            spinDuration,

            showCornerDecorations,
            showLevelNamesOnWheel
        ));
    }

    static Value toJson(WheelTheme const& value)
    {
        auto obj = Value();

        obj["sliceColorCount"] = value.sliceColorCount;

        obj["sliceColor1"] = value.sliceColor1;
        obj["sliceColor2"] = value.sliceColor2;
        obj["sliceColor3"] = value.sliceColor3;
        obj["sliceColor4"] = value.sliceColor4;

        obj["textColor"] = value.textColor;

        obj["backgroundColor"] = value.backgroundColor;

        obj["buttonColor"] = value.buttonColor;

        obj["spinDuration"] = value.spinDuration;
        obj["showCornerDecorations"] = value.showCornerDecorations;
        obj["showLevelNamesOnWheel"] = value.showLevelNamesOnWheel;

        return obj;
    }
};
