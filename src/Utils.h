#pragma once

#include <Geode/Geode.hpp>

class Utils
{
public:
    static void playSound(gd::string soundPath);
    static void playResourceSound(const gd::string& soundFileName);
};
