#define NUMOFSKILLS 16 // excludes perception and lores

#include "combatTracker.h"

combatTracker::combatTracker(std::vector<Combatant> playerVec, std::vector<Combatant> enemyVec)
{
    this->playerVec = playerVec;
    this->enemyVec = enemyVec;
    handleInitiative();
    beginInitiative(combatants);
}

void combatTracker::manualInitiative()
{
    std::cout << "Enter the initiative for each combatant\n";

    for (auto i = 0; i < playerVec.size(); i++)
    {
        int initNum = 0;
        std::cout << playerVec[i].getName() << ": ";
        initNum = validateIntEntry(); // get input validated
        combatants.insert({initNum, playerVec[i]});
    }

    for (auto i = 0; i < enemyVec.size(); i++)
    {
        int initNum = 0;
        std::cout << enemyVec[i].getName() << ": ";
        initNum = validateIntEntry(); // get input validated
        combatants.insert({initNum, enemyVec[i]});
    }
}

void combatTracker::roll(std::vector<Combatant> aVec)
{
    srand(time(0));

    bool alreadyRolled = false;

    for (int x = 0; x < aVec.size(); x++)
    {
        for (auto it : combatants)
        {
            if (aVec[x].getName() == it.second.getName())
            {
                alreadyRolled = true; // found an initiative already
            }
        }

        if (alreadyRolled == false)
        {
            int roll = (rand() % 21);
            int mod = aVec[x].getSkillMods().find("Perception")->second; // Rolls a D20 + skill mod
            int ogRoll = roll;
            roll += mod;

            std::cout << "Type any extra bonuses to add to " << aVec[x].getName() << "'s roll: ";
            int extraMod = INT8_MIN;
            std::cin >> extraMod;

            if (extraMod != INT8_MIN)
            {
                roll += extraMod;
            }

            std::cout << "Rolled a " << roll << " ( Dice roll: " << ogRoll << " + "
                      << "perception modifier: " << mod;
            (extraMod != INT8_MIN) ? std::cout << " + other bonuses: " << extraMod << " )" << std::endl : std::cout << " )" << std::endl;
            std::cout << std::endl;
            combatants.insert(std::pair<int, Combatant>(roll, aVec[x]));
        }

        // reset flag
        alreadyRolled = false;
    }
}

void combatTracker::autoInitiative()
{

    // TO DO: IMPLEMENT FULL AUTO INITIATIVE AND STRING PARSING OF USER INPUT
    std::cout << std::endl
              << "Type the corresponding number of any combatant not using Perception for their initiative, \nthen type the number of the respective skill in the following format:\n\"3\"(combatant number) \"4\"(skill number) \"1\"(combatant number) \"1\"(skill number) so that it looks like:\n3 4 1 1\n";

    unsigned long int i = 0;

    // Rolls all combatants initiatives with perception by default, any specific skills will overwrite the perception roll
    // This is to prevent possible segfaults from someone not having an initiative roll

    for (; i < playerVec.size(); i++)
    {
        std::cout << i << ". " << playerVec[i].getName() << std::endl;
    }

    for (auto x = 0; x < enemyVec.size(); x++)
    {
        std::cout << x + i << ". " << enemyVec[x].getName() << std::endl;
    }

    std::cout << std::endl;

    std::string skills[] = {"Acrobatics", "Arcana", "Athletics", "Crafting", "Deception",
                            "Diplomacy", "Intimidation", "Medicine", "Nature",
                            "Occultism", "Performance", "Religion", "Society", "Stealth",
                            "Survival", "Thievery"};

    for (auto x = 0; x < NUMOFSKILLS; x++)
    {
        std::cout << x << ". " << skills[x] << " ";
        if ((x + 1) % 2 == 0)
        {
            std::cout << std::endl;
        }
    }

    std::cout << std::endl;

    std::string combatantSelections = validateCombatantSelection();

    initStrParser(combatantSelections);

    roll(playerVec);
    roll(enemyVec);
}

void combatTracker::initStrParser(std::string combatantSelections)
{
    std::istringstream ss(combatantSelections);
    std::map<std::string, int> skillMods;
    std::map<int, std::string> initOrder;
    std::string selections = " ";
    srand(time(0));

    bool first = true;
    bool invalidEntry = false;
    std::string index = " ";

    std::vector<int> combatantIndex;
    std::vector<int> skillIndex;

    // Create int arrays of the user entered command
    while (ss >> selections)
    {
        if (first)
        {
            combatantIndex.push_back(stoi(selections));
            first = false;
        }
        else
        {
            skillIndex.push_back(stoi(selections));
            first = true;
        }
    }

    for (unsigned long x = 0; x < combatantIndex.size(); x++)
    {
        if (combatantIndex[x] < playerVec.size())
        {
            skillMods = playerVec[combatantIndex[x]].getSkillMods();
            for (auto iter = skillMods.begin(); iter != skillMods.end(); iter++)
            {
                std::cout << iter->first << std::endl;
            }
        }
        else if (combatantIndex[x] < playerVec.size() + enemyVec.size())
        {
            skillMods = enemyVec[combatantIndex[x] - playerVec.size()].getSkillMods();

            for (auto iter = skillMods.begin(); iter != skillMods.end(); iter++)
            {
                std::cout << iter->first << std::endl;
            }
        }
        else
        {
            std::cerr << "Combatant number entered is invalid and does not exist.\nSkipping this entry...\n";
            continue;
        }

        auto iter = skillMods.begin();
        if (skillIndex[x] < skillMods.size())
        {
            std::advance(iter, skillIndex[x] + 1);
            int mod = iter->second;
            int roll = (rand() % 21); // Rolls a D20

            if (combatantIndex[x] < playerVec.size())
            {
                std::cout << "Type any extra bonuses to add to " << playerVec[combatantIndex[x]].getName() << "'s roll: ";

                int extraMod = INT8_MIN;

                std::cin >> extraMod;

                int ogRoll = roll;
                roll += mod;

                if (extraMod != INT8_MIN)
                {
                    roll += extraMod;
                }

                std::cout << "Rolled a " << roll << " ( Dice roll: " << ogRoll << " + " << iter->first << " modifier: " << mod;
                (extraMod != INT8_MIN) ? std::cout << " + other modifiers: " << extraMod << " )" << std::endl : std::cout << " )" << std::endl;
                std::cout << std::endl;
                combatants.insert(std::pair<int, Combatant>(roll, playerVec[combatantIndex[x]]));
            }
            else
            {
                std::cout << "Type any extra bonuses to add to " << enemyVec[combatantIndex[x] - playerVec.size()].getName() << "'s roll: ";
                int extraMod = INT8_MIN;
                int ogRoll = roll;
                std::cin >> extraMod;
                roll += mod;

                if (extraMod != INT8_MIN)
                {
                    roll += extraMod;
                }

                std::cout << "Rolled a " << roll << " ( Dice roll: " << ogRoll << " + " << iter->first << " modifier: " << mod;
                (extraMod != INT8_MIN) ? std::cout << " + other modifiers: " << extraMod << " )" << std::endl : std::cout << " )" << std::endl;
                std::cout << std::endl;
                combatants.insert(std::pair<int, Combatant>(roll, enemyVec[combatantIndex[x] - playerVec.size()]));
            }
        }
        else
        {
            std::cerr << "Skill number entered is invalid and does not exist.\nSkipping this entry...\n";
        }
    }
}

void combatTracker::handleInitiative()
{
    std::cout << "1. Roll initiative for me\n2. Manually enter initiative\n";
    char userInput = ' ';

    while (std::cin >> userInput)
    {
        if (userInput == '1')
        {
            autoInitiative();
            break;
        }
        else if (userInput == '2')
        {
            manualInitiative();
            break;
        }
        else
        {
            std::cerr << "Please enter a valid number\n";
        }
    }
}

void combatTracker::printInitOrder(std::multimap<int, Combatant, std::greater<int>>::iterator currentTurn)
{
    std::cout << "Initiative Order\n\n";

    std::string longestName = "";

    // find longest name in the initative for setw()
    for (auto itr = combatants.begin(); itr != combatants.end(); itr++)
    {
        std::string tempName = itr->second.getName();

        if (longestName.length() < tempName.length())
        {
            longestName = tempName;
        }
    }

    // Print initiative order
    for (auto itr = combatants.begin(); itr != combatants.end(); itr++)
    {
        std::string conStr = itr->second.getName() + ": " + std::to_string(itr->first);
        std::cout << conStr;
        if (currentTurn->second.getName() == itr->second.getName())
        {
            // The four makes room for the ": (initiative number)"
            // using a set number for setw() may break on longer names so dynamic approach is better
            std::cout << std::setw((longestName.length() + 4 + 5) - conStr.length()) << " <---";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}

/*
void combatTracker::printConditions()
{
    std::cout << "Death and Dying:\n1.Doomed\n2.Dying\n3.Unconscious\n4.Wounded\n\n";
    std::cout << "Degrees of Detection:\n5.Hidden\n6.Observed\n7.Undetected\n8.Unnoticed\n\n";
    std::cout << "Lowered Abilities:\n9.Clumsy\n10.Drained\n11.Enfeebled\n12.Stupefied\n\n";
    std::cout << "Senses:\n13.Blinded\n14.Concealed\n15.Dazzled\n16.Deafened\n17.Invisible\n\n";
    std::cout << "Other:\n18.Confused\n19.Controlled\n20.Encumbered\n21.Fascinated\n22.Fatigued\n";
    std::cout << "23.Flatfooted\n24.Fleeing\n25.Frightened\n26.Grabbed\n27.Immobilized\n28.Paralyzed\n";
    std::cout << "29.Petrified\n30.Prone\n31.Quickened\n32.Restrained\n33.Sickened\n34.Slowed\n";
    std::cout << "35.Stunned\n\n";
    std::cout << "Attitudes:\n36.Friendly\n37.Helpful\n38.Hostile\n39.Indifferent\n40.Unfriendly\n\n";
}
*/

/*
void combatTracker::printPDamage()
{
    std::cout << "Death and Dying:\n1.Doomed\n2.Dying\n3.Unconscious\n4.Wounded\n\n";
    std::cout << "Degrees of Detection:\n5.Hidden\n6.Observed\n7.Undetected\n8.Unnoticed\n\n";
    std::cout << "Lowered Abilities:\n9.Clumsy\n10.Drained\n11.Enfeebled\n12.Stupefied\n\n";
    std::cout << "Senses:\n13.Blinded\n14.Concealed\n15.Dazzled\n16.Deafened\n17.Invisible\n\n";
    std::cout << "Other:\n18.Confused\n19.Controlled\n20.Encumbered\n21.Fascinated\n22.Fatigued\n";
    std::cout << "23.Flatfooted\n24.Fleeing\n25.Frightened\n26.Grabbed\n27.Immobilized\n28.Paralyzed\n";
    std::cout << "29.Petrified\n30.Prone\n31.Quickened\n32.Restrained\n33.Sickened\n34.Slowed\n";
    std::cout << "35.Stunned\n\n";
    std::cout << "Attitudes:\n36.Friendly\n37.Helpful\n38.Hostile\n39.Indifferent\n40.Unfriendly\n\n";
}
*/

/*
void combatTracker::setPersistentTracker(multimap<int, Combatant> &combatants)
{
    std::cout << "Select combatant to add persistent damage to.\n\n";
    int count = 0;
    int selection = 0;
    map<int, Combatant> selected;

    for (multimap<int, Combatant>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        std::cout << count << ": " << itr->getName() << endl;
        selected.insert({count, *itr});
        count++;
    }

    cin >> selection;
    Combatant *chosen = &(selected.at(selection));
    std::cout << "Pick a persistent damage(s) to apply\n\n";

    printPDamage();

    chosen->setCondition();
    string chosenName = chosen->getName();

    for (std::multimap<int, Combatant, std::greater<int>>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        if (chosenName == (itr->getName()))
        {
            std::multimap<int, Combatant, std::greater<int>>::iterator it1 = combatants.erase(itr);
            combatants.insert(it1, *chosen);
            break;
        }
    }
}
*/

/*
void combatTracker::setConditionTracker(multimap<int, Combatant> &combatants)
{
    std::cout << "Select combatant to add condition to.\n\n";
    int count = 0;
    int selection = 0;
    map<int, Combatant> selected;

    for (std::multimap<int, Combatant, std::greater<int>>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        std::cout << count << ": " << itr->getName() << endl;
        selected.insert({count, *itr});
        count++;
    }

    cin >> selection;
    Combatant *chosen = &(selected.at(selection));
    std::cout << "Pick a condition(s) to apply\n\n";

    printConditions();

    chosen->setCondition();
    string chosenName = chosen->getName();

    for (std::multimap<int, Combatant, std::greater<int>>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        if (chosenName == (itr->getName()))
        {
            std::multimap<int, Combatant, std::greater<int>>::iterator it1 = combatants.erase(itr);
            combatants.insert(it1, *chosen);
            break;
        }
    }
}
*/

void combatTracker::beginInitiative(std::multimap<int, Combatant, std::greater<int>> combatants)
{

    char userInput = ' ';
    bool startOfTurn = true;

    // ROUNDS
    for (short int z = 1;; z++)
    {

        // TURNS
        for (std::multimap<int, Combatant, std::greater<int>>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
        {
            startOfTurn = true;
            // UPDATE STUNNED AT BEGINNING OF TURN
            // itr->updateConditions(startOfTurn);

            std::cout << "\033[2J\033[1;1H"; // clears screen with escape sequences

            std::cout << "ROUND " << z << "\n\n";

            // CURRENT COMBATANT
            printInitOrder(itr);

            std::cout << "Current Turn\n\n";
            itr->second.printCombatantInfo();
            std::cout << "n to end turn, c to add a condition to a combatant,\n";
            std::cout << "p to add persistent damage,\n";
            std::cout << "r to remove condition or persistent damage,\n";
            std::cout << "v to change value of condition or persistent damage\n\n";

            while (1)
            {
                std::cin >> userInput;
                if (userInput == 'n')
                {
                    break;
                }
                else if (userInput == 'c')
                {
                    // setConditionTracker(combatants);
                    //  PREVIOUS COMBATANT
                    for (std::multimap<int, Combatant, std::greater<int>>::iterator itr2 = combatants.begin(); itr2 != itr; ++itr2)
                    {
                        itr2->second.printCombatantInfo();
                    }
                    // CURRENT COMBATANT
                    std::cout << "Current Turn\n\n";
                    itr->second.printCombatantInfo();
                    std::cout << "n to end turn, c to add a condition to a combatant,\n";
                    std::cout << "p to add persistent damage,\n";
                    std::cout << "r to remove condition or persistent damage,\n";
                    std::cout << "v to change value of condition or persistent damage\n\n";
                }
                else if (userInput == 'p')
                {
                    // setPersistentTracker(combatants);
                    //  PREVIOUS COMBATANT
                    for (std::multimap<int, Combatant, std::greater<int>>::iterator itr2 = combatants.begin(); itr2 != itr; ++itr2)
                    {
                        itr2->second.printCombatantInfo();
                    }
                    // CURRENT COMBATANT
                    std::cout << "Current Turn\n\n";
                    itr->second.printCombatantInfo();
                    std::cout << "n to end turn, c to add a condition to a combatant,\n";
                    std::cout << "p to add persistent damage,\n";
                    std::cout << "r to remove condition or persistent damage,\n";
                    std::cout << "v to change value of condition or persistent damage\n\n";
                }
                else if (userInput == 'r')
                {
                    std::cout << "UNIMPLEMENTED\n";
                }
                else if (userInput == 'v')
                {
                    std::cout << "UNIMPLEMENTED\n";
                }
                else
                {
                    std::cerr << "Invalid command\n";
                }
            }
            startOfTurn = false;

            // UPDATE FRIGHTENED AT END OF TURN
            // itr->updateConditions(startOfTurn);
        }
    }
}

combatTracker::~combatTracker()
{
}
