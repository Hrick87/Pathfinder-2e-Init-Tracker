#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <iostream>
#include <map>
#include <list>
#include <iomanip>
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
    std::multimap<int, Combatant, std::greater<int>> combatants;

    void manualInitiative();
    void autoInitiative();
    void initStrParser(std::string combatantSelections);
    void roll(std::vector<Combatant> aVec);
    void initConditionMap();
    void setCondition();

public:
    combatTracker(std::vector<Combatant>, std::vector<Combatant>);
    void handleInitiative();

    // void setPersistentTracker(multimap<int, Combatant> &combatants);
    void printConditions();
    //  void printPDamage();
    void printInitOrder(std::multimap<int, Combatant, std::greater<int>>::iterator currentTurn, std::multimap<int, Combatant, std::greater<int>> mapToPrint);
    void beginInitiative(std::multimap<int, Combatant, std::greater<int>> combantants);

    ~combatTracker();
};

#endif