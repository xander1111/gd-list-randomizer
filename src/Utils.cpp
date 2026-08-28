#include "Utils.h"

#include <utility>

void Utils::playSound(gd::string soundPath)
{
    FMODAudioEngine* fmod = FMODAudioEngine::get();
    fmod->m_globalChannel->setPaused(false);

    fmod->playEffectAsync(std::move(soundPath));
}

void Utils::playResourceSound(const gd::string& soundFileName)
{
    playSound(geode::utils::string::pathToString(geode::Mod::get()->getResourcesDir() / soundFileName));
}
