#include "combative.h"

Combative::Combative()
{
    string name;
    int initiative;
    int hp;
    int ac;

    cout << "Name: ";
    cin >> name;

    cout << "Initiative: ";
    cin >> initiative;

    cout << "HP: ";
    cin >> hp;

    cout << "AC: ";
    cin >> ac;
    cout << endl;

    this->name = name;
    this->initiative = initiative;
    this->hp = hp;
    this->ac = ac;
}

string Combative::getName()
{
    return name;
}

int Combative::getInit()
{
    return initiative;
}

int Combative::getHp()
{
    return hp;
}

int Combative::getAc()
{
    return ac;
}

map<string, int> Combative::getCondition()
{
    return condition;
}

void Combative::updateConditions(bool startOfTurn)
{
    for (map<string, int>::iterator itr = condition.begin(); itr != condition.end(); ++itr)
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

void Combative::setCondition()
{

    int addCond = 0;
    int condVal = -1;
    char moreCons = 'c';

    while (moreCons == 'c')
    {
        cout << "Type the number of the condition to add." << endl;
        cin >> addCond;

        switch (addCond)
        {
        case 1:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
            condition.insert({"Doomed", condVal});
            break;
        }
        case 2:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
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
            cout << "Set value of condition." << endl;
            cin >> condVal;
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
            cout << "Set value of condition." << endl;
            cin >> condVal;
            condition.insert({"Clumsy", condVal});
            break;
        }
        case 10:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
            condition.insert({"Drained", condVal});
            break;
        }
        case 11:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
            condition.insert({"Enfeebled", condVal});
            break;
        }
        case 12:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
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
            cout << "Set value of condition." << endl;
            cin >> condVal;
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
            cout << "Set value of condition." << endl;
            cin >> condVal;
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
            cout << "Set value of condition." << endl;
            cin >> condVal;
            condition.insert({"Sickened", condVal});
            break;
        }
        case 34:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
            condition.insert({"Slowed", condVal});
            break;
        }
        case 35:
        {
            cout << "Set value of condition." << endl;
            cin >> condVal;
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
            cout << "Please pick a valid number";
            break;
        }
        }

        cout << "Type c to add more conditions. Any other button to exit." << endl;
        cin >> moreCons;
    }
}

void Combative::printCombatantInfo()
{

    cout << "Conditions: ";
    if (condition.size() > 0)
    {
        for (map<string, int>::iterator itr = condition.begin(); itr != condition.end(); ++itr)
        {
            cout << itr->first << " ";

            if (itr->second > 0)
            {
                cout << itr->second << " ";
            }
        }
    }
    cout << endl;
    cout << "Name: " << name << endl;
    cout << "Initiative: " << initiative << endl;
    cout << "HP: " << hp << endl;
    cout << "AC: " << ac << endl
         << endl;
}

Combative::~Combative()
{
}
