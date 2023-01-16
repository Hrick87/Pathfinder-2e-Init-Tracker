#ifndef COMBATIVE_H
#define COMBATIVE_H

#include <iostream>
#include <vector>
#include <map>
#include <list>

using namespace std;

class Combative
{
protected:
    string name;
    int initiative;
    int hp;
    int ac;
    map<string, int> condition; //TRACKS CONDITION AND VALUE

public:
    Combative();
    void printCombatantInfo();
    int getInit();
    string getName();
    int getHp();
    int getAc();
    void updateConditions(bool startOfTurn);
    map<string, int> getCondition();
    void setCondition();
    ~Combative();
};


#endif