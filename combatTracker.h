#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <iostream>
#include <map>
#include <list>
#include <iomanip>
#include <cmath>
#include "combatant.h" //delete??? may be redundant after refactoring
#include "initialize.h"
#include "rapidjson/document.h"

class combatTracker
{
private:
    /* GAME DATA */
    short int numCombatants;
    std::vector<Combatant> playerVec;
    std::vector<Combatant> enemyVec;
    std::map<std::string, std::vector<std::pair<std::string, int>>> conditions;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> persistentDamage;
    std::multimap<float, Combatant, std::greater<float>> combatants;

    void searchInitForTie(float &roll, Combatant);
    void manualInitiative();
    void autoInitiative();
    void initStrParser(std::string combatantSelections);
    void roll(std::vector<Combatant> aVec);
    void initConditionMap();
    void initPersistentMap();
    void setCondition();
    void setPersistentDamage();

public:
    combatTracker(std::vector<Combatant>, std::vector<Combatant>);
    void handleInitiative();
    void printConditions();
    void printInitOrder(std::multimap<float, Combatant, std::greater<float>>::iterator currentTurn, std::multimap<float, Combatant, std::greater<float>> mapToPrint);
    void beginInitiative();

    ~combatTracker();
};

#endif