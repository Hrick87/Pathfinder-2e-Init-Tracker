#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <iostream>
#include <ctype.h>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <fstream>
#include <experimental/filesystem>
#include "combatant.h"
#include <exception>

namespace fs = std::experimental::filesystem;

class initialize
{

public:
    initialize();
    // Getters
    std::vector<Combatant> getEnemies() { return enemyVec; }
    std::vector<Combatant> getPlayers() { return playerVec; }

private:
    std::vector<Combatant> playerVec;
    std::vector<Combatant> enemyVec;
    void manualPlayerEntry();
    void manualEnemyEntry();
    void readInPlayerJSON();
    void readInEnemyJSON();
    std::map<std::string, int> getPlayerSkillMods(rapidjson::Document &player);
    std::map<std::string, int> getEnemySkillMods(rapidjson::Document &enemy);
    int calculateAbilityMod(int abilityScore);
    int numberOfplayers;
    int numberOfEnemies;
};

#endif