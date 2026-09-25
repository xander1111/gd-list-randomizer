#include "Utils.h"

#include <cvolton.level-id-api/include/EditorIDs.hpp>

void Utils::playSound(gd::string soundPath)
{
    FMODAudioEngine* fmod = FMODAudioEngine::get();
    fmod->m_globalChannel->setPaused(false);

    fmod->playEffectAsync(std::move(soundPath));
}

void Utils::playResourceSound(const gd::string& soundFileName)
{
    playSound(string::pathToString(Mod::get()->getResourcesDir() / soundFileName));
}

gd::string Utils::getDifficultyIconFrame(int difficulty)
{
    if (difficulty == -1)
        // For whatever reason, `GJLevelList::frameForListDifficulty` handles everything but difficulty -1 (N/A) correctly
        return "diffIcon_00_btn_001.png";

    if (0 <= difficulty && difficulty <= 10)
        return GJLevelList::frameForListDifficulty(difficulty, DifficultyIconType::NoText);

    log::debug("[Utils::getDifficultyIcon]: Unknown difficulty value: {}", difficulty);
    return "diffIcon_00_btn_001.png";
}

gd::string Utils::getListId(GJLevelList* list)
{
    if (list->m_listType == GJLevelType::Editor)
        return "editor-" + std::to_string(EditorIDs::getID(list));
    else
        return std::to_string(list->m_listID);
}
