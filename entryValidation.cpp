#include "entryValidation.h"

int validateIntEntry()
{
    std::string testStr = " ";
    bool breakOut = false;

    while (std::cin >> testStr)
    {
        for (auto i = 0; i < testStr.size(); i++)
        {
            if (isdigit(testStr[i]) && i + 1 == testStr.size())
            {
                breakOut = true;
            }
            else if (!isdigit(testStr[i]))
            {
                std::cout << "Please enter valid number.\n";
                break;
            }
        }

        if (breakOut)
        {
            break;
        }
    }

    return stoi(testStr);
}

std::string validateCombatantSelection()
{

    std::string combatantSelections = "";
    bool breakOut = false;
    std::smatch m;
    std::regex p("(?:[0-9]+\\s[0-9]+\\s?)+"); // matches even number of digits spaced apart
    std::cin.ignore();
    while (std::getline(std::cin, combatantSelections))
    {
        // std::cout << "inside combatantSelections: " << combatantSelections << std::endl;
        if (regex_match(combatantSelections, m, p) || combatantSelections == "")
        {
            breakOut = true;
        }
        else
        {
            std::cerr << "Invalid format, please type the corresponding number of any combatant not using Perception for their initiative, \nthen type the number of the respective skill in the following format:\n\"3\"(combatant number) \"4\"(skill number) \"1\"(combatant number) \"1\"(skill number) so that it looks like\n3 4 1 1\n";
        }

        if (breakOut)
        {
            break;
        }
    }
    return combatantSelections;
}