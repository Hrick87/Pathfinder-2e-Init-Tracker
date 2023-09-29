#include "combatant.h"

Combatant::Combatant()
{
    std::cout << "Name: ";
    std::cin >> this->name;

    std::cout << "HP: ";
    this->hp = validateIntEntry();

    std::cout << "AC: ";
    this->ac = validateIntEntry();
    std::cout << std::endl;

    while (1)
    {
        std::cout << "Is this a player? y/n\n";
        char yesOrNo = ' ';
        std::cin >> yesOrNo;
        if (tolower(yesOrNo) == 'y')
        {
            this->isPlayer = true;
            break;
        }
        else if (tolower(yesOrNo) == 'n')
        {
            this->isPlayer = false;
            break;
        }
        else
        {
            std::cerr << "Please enter a vlid entry\n";
        }
    }
}

Combatant::Combatant(std::string name, int hp, int ac, std::map<std::string, int> skillMods, bool isPlayer)
{
    this->name = name;
    this->hp = hp;
    this->ac = ac;
    this->skillMods = skillMods;
    this->isPlayer = isPlayer;
}

std::string Combatant::getName()
{
    return name;
}

int Combatant::getHp()
{
    return hp;
}

int Combatant::getAc()
{
    return ac;
}

/*
void Combatant::updateConditions(bool startOfTurn)
{
    for (std::map<std::string, int>::iterator itr = condition.begin(); itr != condition.end(); ++itr)
    {
        if ((itr->second > 0 && itr->first == "Frightened" && startOfTurn == false) || (itr->second > 0 && itr->first == "Stunned" && startOfTurn == true))
        {
            itr->second = (itr->second) - 1;
        }

        if (itr->second == 0)
        {
            condition.erase(itr);

            if (itr != condition.end())
            {
                break;
            }
        }
    }
}
*/

void Combatant::printCombatantInfo()
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "HP: " << hp << std::endl;
    std::cout << "AC: " << ac << std::endl;
}

Combatant::~Combatant()
{
}
