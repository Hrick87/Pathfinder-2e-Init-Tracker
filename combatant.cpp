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
}

Combatant::Combatant(std::string name, int hp, int ac, std::map<std::string, int> skillMods)
{
    this->name = name;
    this->hp = hp;
    this->ac = ac;
    this->skillMods = skillMods;
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

std::map<std::string, int> Combatant::getCondition()
{
    return condition;
}

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

void Combatant::setCondition()
{

    int addCond = 0;
    int condVal = -1;
    char moreCons = 'c';

    while (moreCons == 'c')
    {
        std::cout << "Type the number of the condition to add." << std::endl;
        std::cin >> addCond;

        switch (addCond)
        {
        case 1:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Doomed", condVal});
            break;
        }
        case 2:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Dying", condVal});
            break;
        }
        case 3:
        {
            condition.insert({"Unconscious", condVal});
            break;
        }
        case 4:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Wounded", condVal});
            break;
        }
        case 5:
        {
            condition.insert({"Hidden", condVal});
            break;
        }
        case 6:
        {
            condition.insert({"Observed", condVal});
            break;
        }
        case 7:
        {
            condition.insert({"Undetected", condVal});
            break;
        }
        case 8:
        {
            condition.insert({"Unnoticed", condVal});
            break;
        }
        case 9:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Clumsy", condVal});
            break;
        }
        case 10:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Drained", condVal});
            break;
        }
        case 11:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Enfeebled", condVal});
            break;
        }
        case 12:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Stupified", condVal});
            break;
        }
        case 13:
        {
            condition.insert({"Blinded", condVal});
            break;
        }
        case 14:
        {
            condition.insert({"Concealed", condVal});
            break;
        }
        case 15:
        {
            condition.insert({"Dazzled", condVal});
            break;
        }
        case 16:
        {
            condition.insert({"Deafened", condVal});
            break;
        }
        case 17:
        {
            condition.insert({"Invisible", condVal});
            break;
        }
        case 18:
        {
            condition.insert({"Confused", condVal});
            break;
        }
        case 19:
        {

            condition.insert({"Controlled", condVal});
            break;
        }
        case 20:
        {
            condition.insert({"Encumbered", condVal});
            break;
        }
        case 21:
        {

            condition.insert({"Fascinated", condVal});
            break;
        }
        case 22:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Fatigued", condVal});
            break;
        }
        case 23:
        {
            condition.insert({"Flatfooted", condVal});
            break;
        }
        case 24:
        {
            condition.insert({"Fleeing", condVal});
            break;
        }
        case 25:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Frightened", condVal});
            break;
        }
        case 26:
        {
            condition.insert({"Grabbed", condVal});
            break;
        }
        case 27:
        {
            condition.insert({"Immobilized", condVal});
            break;
        }
        case 28:
        {
            condition.insert({"Paralyzed", condVal});
            break;
        }
        case 29:
        {
            condition.insert({"Petrified", condVal});
            break;
        }
        case 30:
        {
            condition.insert({"Prone", condVal});
            break;
        }
        case 31:
        {
            condition.insert({"Quickened", condVal});
            break;
        }
        case 32:
        {
            condition.insert({"Restrained", condVal});
            break;
        }
        case 33:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Sickened", condVal});
            break;
        }
        case 34:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Slowed", condVal});
            break;
        }
        case 35:
        {
            std::cout << "Set value of condition." << std::endl;
            std::cin >> condVal;
            condition.insert({"Stunned", condVal});
            break;
        }
        case 36:
        {
            condition.insert({"Friendly", condVal});
            break;
        }
        case 37:
        {
            condition.insert({"Helpful", condVal});
            break;
        }
        case 38:
        {
            condition.insert({"Hostile", condVal});
            break;
        }
        case 39:
        {
            condition.insert({"Indifferent", condVal});
            break;
        }
        case 40:
        {
            condition.insert({"Unfriendly", condVal});
            break;
        }
        default:
        {
            std::cout << "Please pick a valid number";
            break;
        }
        }

        std::cout << "Type c to add more conditions. Any other button to exit." << std::endl;
        std::cin >> moreCons;
    }
}

void Combatant::printCombatantInfo()
{

    std::cout << "Name: " << name << std::endl;
    std::cout << "HP: " << hp << std::endl;
    std::cout << "AC: " << ac << std::endl
              << std::endl;

    std::cout << "Conditions: ";
    if (condition.size() > 0)
    {
        for (std::map<std::string, int>::iterator itr = condition.begin(); itr != condition.end(); ++itr)
        {
            std::cout << itr->first << " ";

            if (itr->second > 0)
            {
                std::cout << itr->second << " ";
            }
        }
    }
    std::cout << std::endl;
}

Combatant::~Combatant()
{
}
