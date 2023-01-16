#ifndef COMBATTRACKER_H
#define COMBATTRACKER_H

#include <iostream>
#include <map>
#include <list>
#include "combative.h"
#include "player.h"
#include "enemy.h"

class combatTracker
{
private:
    /* GAME DATA */
    short int numCombatants;
    list<Combative> combatants;

public:
    combatTracker();
    void setConditionTracker(list<Combative> &combatants);
    void setPersistentTracker(list<Combative> &combatants);
    void printConditions();
    void printPDamage();
    void printInitOrder(list<Combative> initOrder);
    void beginInitiative(list<Combative> combantants);

    ~combatTracker();
};

#endif