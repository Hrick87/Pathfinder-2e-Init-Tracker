#ifndef COMBATIVE_H
#define COMBATIVE_H

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "entryValidation.h"

class Combatant
{

private:
    std::string name;
    int hp;
    int ac;
    std::map<std::string, int> skillMods;
    std::map<std::string, int> condition; // TRACKS CONDITION AND VALUE

public:
    Combatant();                                                                       // For manual entry of player data
    Combatant(std::string name, int hp, int ac, std::map<std::string, int> skillMods); // For JSON read in values
    void printCombatantInfo();
    std::string getName();
    int getHp();
    int getAc();
    void updateConditions(bool startOfTurn);
    std::map<std::string, int> getCondition();
    std::map<std::string, int> getSkillMods() { return skillMods; };

    void setCondition();

    ~Combatant();
};

#endif