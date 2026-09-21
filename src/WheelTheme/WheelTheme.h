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

    ccColor4F outlineColorInner;
    ccColor4F outlineColorOuter;

    ccColor4F backgroundColor;
    unsigned int buttonColor;


    // Sounds
    // TODO figure out how to implement custom sounds
    //gd::string tickSound;
    //gd::string selectSound;


    // Wheel behaviour

    unsigned int spinSpeed;
    float spinDuration;


    // Misc

    bool showCornerDecorations;
    bool showLevelNamesOnWheel;


    WheelTheme(const unsigned int sliceColorCount, const ccColor4F& sliceColor1, const ccColor4F& sliceColor2,
        const ccColor4F& sliceColor3, const ccColor4F& sliceColor4, const ccColor4F& textColor,
        const ccColor4F& outlineColorInner, const ccColor4F& outlineColorOuter, const ccColor4F& backgroundColor,
        const unsigned int buttonColor, const unsigned int spinSpeed, const float spinDuration,
        const bool showCornerDecorations, const bool showLevelNamesOnWheel)
        : sliceColorCount(sliceColorCount),
          sliceColor1(sliceColor1),
          sliceColor2(sliceColor2),
          sliceColor3(sliceColor3),
          sliceColor4(sliceColor4),
          textColor(textColor),
          outlineColorInner(outlineColorInner),
          outlineColorOuter(outlineColorOuter),
          backgroundColor(backgroundColor),
          buttonColor(buttonColor),
          spinSpeed(spinSpeed),
          spinDuration(spinDuration),
          showCornerDecorations(showCornerDecorations),
          showLevelNamesOnWheel(showLevelNamesOnWheel) {}

    static WheelTheme* getDefaultWheelTheme()
    {
        if (defaultTheme == nullptr)
            defaultTheme = generateDefaultTheme();
        return defaultTheme;
    }

private:
    inline static WheelTheme* defaultTheme = nullptr;

    static WheelTheme* generateDefaultTheme()
    {
        return new WheelTheme(
            2,
            { .r = 161.f / 255.f, .g = 88.f / 255.f, .b = 44.f / 255.f, .a = 1.f },
            { .r = 194.f / 255.f, .g = 114.f / 255.f, .b = 62.f / 255.f, .a = 1.f },
            {},
            {},

            { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f },

            { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f },
            { .r = 1.f, .g = 1.f, .b = 1.f, .a = 1.f },

            { .r = 0.f, .g = 0.f, .b = 0.f, .a = 1.f },
            0,


            // .tickSound = ,
            // .selectSound = ,


            4u,
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

        try {
            GEODE_UNWRAP_INTO(r, value["r"].asDouble());
        } catch (const std::exception&) {
            log::info("Invalid color red data, using default value of 0");
            r = 0.;
        }

        try {
            GEODE_UNWRAP_INTO(g, value["g"].asDouble());
        } catch (const std::exception&) {
            log::info("Invalid color green data, using default value of 0");
            g = 0.;
        }

        try {
            GEODE_UNWRAP_INTO(b, value["b"].asDouble());
        } catch (const std::exception&) {
            log::info("Invalid color blue data, using default value of 0");
            b = 0.;
        }

        try {
            GEODE_UNWRAP_INTO(a, value["a"].asDouble());
        } catch (const std::exception&) {
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
        ccColor4F outlineColorInner, outlineColorOuter;
        ccColor4F backgroundColor;
        unsigned int buttonColor;
        unsigned int spinSpeed;
        float spinDuration;
        bool showCornerDecorations, showLevelNamesOnWheel;


        try {
            GEODE_UNWRAP_INTO(sliceColorCount, value["sliceColorCount"].asUInt());
        } catch (const std::exception&) {
            log::info("Invalid theme sliceColorCount data, using default theme value");
            sliceColorCount = WheelTheme::getDefaultWheelTheme()->sliceColorCount;
        }

        try {
            GEODE_UNWRAP_INTO(sliceColor1, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor1"]));
        } catch (const std::exception&) {
            log::info("Invalid theme sliceColor1 data, using default theme value");
            sliceColor1 = WheelTheme::getDefaultWheelTheme()->sliceColor1;
        }

        try {
            GEODE_UNWRAP_INTO(sliceColor2, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor2"]));
        } catch (const std::exception&) {
            log::info("Invalid theme sliceColor2 data, using default theme value");
            sliceColor2 = WheelTheme::getDefaultWheelTheme()->sliceColor2;
        }

        try {
            GEODE_UNWRAP_INTO(sliceColor3, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor3"]));
        } catch (const std::exception&) {
            log::info("Invalid theme sliceColor3 data, using default theme value");
            sliceColor3 = WheelTheme::getDefaultWheelTheme()->sliceColor3;
        }

        try {
            GEODE_UNWRAP_INTO(sliceColor4, matjson::Serialize<ccColor4F>::fromJson(value["sliceColor4"]));
        } catch (const std::exception&) {
            log::info("Invalid theme sliceColor4 data, using default theme value");
            sliceColor4 = WheelTheme::getDefaultWheelTheme()->sliceColor4;
        }


        try {
            GEODE_UNWRAP_INTO(textColor, matjson::Serialize<ccColor4F>::fromJson(value["textColor"]));
        } catch (const std::exception&) {
            log::info("Invalid theme textColor data, using default theme value");
            textColor = WheelTheme::getDefaultWheelTheme()->textColor;
        }


        try {
            GEODE_UNWRAP_INTO(outlineColorInner, matjson::Serialize<ccColor4F>::fromJson(value["outlineColorInner"]));
        } catch (const std::exception&) {
            log::info("Invalid theme outlineColorInner data, using default theme value");
            outlineColorInner = WheelTheme::getDefaultWheelTheme()->outlineColorInner;
        }

        try {
            GEODE_UNWRAP_INTO(outlineColorOuter, matjson::Serialize<ccColor4F>::fromJson(value["outlineColorOuter"]));
        } catch (const std::exception&) {
            log::info("Invalid theme outlineColorOuter data, using default theme value");
            outlineColorOuter = WheelTheme::getDefaultWheelTheme()->outlineColorOuter;
        }


        try {
            GEODE_UNWRAP_INTO(backgroundColor, matjson::Serialize<ccColor4F>::fromJson(value["backgroundColor"]));
        } catch (const std::exception&) {
            log::info("Invalid theme backgroundColor data, using default theme value");
            backgroundColor = WheelTheme::getDefaultWheelTheme()->backgroundColor;
        }


        try {
            GEODE_UNWRAP_INTO(buttonColor, value["buttonColor"].asUInt());
        } catch (const std::exception&) {
            log::info("Invalid theme buttonColor data, using default theme value");
            backgroundColor = WheelTheme::getDefaultWheelTheme()->backgroundColor;
        }


        try {
            GEODE_UNWRAP_INTO(spinSpeed, value["spinSpeed"].asUInt());
        } catch (const std::exception&) {
            log::info("Invalid theme spinSpeed data, using default theme value");
            spinSpeed = WheelTheme::getDefaultWheelTheme()->spinSpeed;
        }

        try {
            GEODE_UNWRAP_INTO(spinDuration, value["spinDuration"].asDouble());
        } catch (const std::exception&) {
            log::info("Invalid theme spinDuration data, using default theme value");
            spinDuration = WheelTheme::getDefaultWheelTheme()->spinDuration;
        }


        try {
            GEODE_UNWRAP_INTO(showCornerDecorations, value["showCornerDecorations"].asBool());
        } catch (const std::exception&) {
            log::info("Invalid theme showCornerDecorations data, using default theme value");
            showCornerDecorations = WheelTheme::getDefaultWheelTheme()->showCornerDecorations;
        }

        try {
            GEODE_UNWRAP_INTO(showLevelNamesOnWheel, value["showLevelNamesOnWheel"].asBool());
        } catch (const std::exception&) {
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

            outlineColorInner,
            outlineColorOuter,

            backgroundColor,

            buttonColor,

            spinSpeed,
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

        obj["outlineColorInner"] = value.outlineColorInner;
        obj["outlineColorOuter"] = value.outlineColorOuter;

        obj["backgroundColor"] = value.backgroundColor;

        obj["buttonColor"] = value.buttonColor;

        obj["spinSpeed"] = value.spinSpeed;
        obj["spinDuration"] = value.spinDuration;
        obj["showCornerDecorations"] = value.showCornerDecorations;
        obj["showLevelNamesOnWheel"] = value.showLevelNamesOnWheel;

        return obj;
    }
};
