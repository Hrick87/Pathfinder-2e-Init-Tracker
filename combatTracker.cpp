#define NUMOFSKILLS 16 // excludes perception and lores

#include "combatTracker.h"

const std::string conditionArr[42] = {
    "Doomed", "Dying", "Unconscious", "Wounded",
    "Hidden", "Observed", "Undetected", "Unnoticed",
    "Clumsy", "Drained", "Enfeebled", "Stupefied",
    "Blinded", "Concealed", "Dazzled", "Deafened", "Invisible",
    "Confused", "Controlled", "Encumbered", "Fascinated", "Fatigued",
    "Flatfooted", "Fleeing", "Frightened", "Grabbed", "Immobilized", "Paralyzed",
    "Petrified", "Prone", "Quickened", "Restrained", "Sickened", "Slowed",
    "Stunned", "Friendly", "Helpful", "Hostile", "Indifferent", "Unfriendly"};

combatTracker::combatTracker(std::vector<Combatant> playerVec, std::vector<Combatant> enemyVec)
{
    this->playerVec = playerVec;
    this->enemyVec = enemyVec;
    handleInitiative();
    beginInitiative();
}

void combatTracker::searchInitForTie(float &roll, Combatant combatant)
{
    for (auto iter = combatants.begin(); iter != combatants.end(); iter++)
    {
        if (iter->first == roll)
        {
            if (iter->second.getIsPlayer() == false && combatant.getIsPlayer() == true)
            {
                roll -= 0.1;
            }
            else if (iter->second.getIsPlayer() == true && combatant.getIsPlayer() == false)
            {
                roll += 0.1;
            }
        }
    }
}

void combatTracker::manualInitiative()
{
    std::cout << "Enter the initiative for each combatant\n";

    for (auto i = 0; i < playerVec.size(); i++)
    {
        float initNum = 0;
        std::cout << playerVec[i].getName() << ": ";
        initNum = validateIntEntry();            // get input validated
        searchInitForTie(initNum, playerVec[i]); // Changes initiative in the case of a player monster tie
        combatants.insert({initNum, playerVec[i]});
    }

    for (auto i = 0; i < enemyVec.size(); i++)
    {
        float initNum = 0;
        std::cout << enemyVec[i].getName() << ": ";
        initNum = validateIntEntry();           // get input validated
        searchInitForTie(initNum, enemyVec[i]); // Changes initiative in the case of a player monster tie
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
            float roll = (rand() % 21);
            int mod = aVec[x].getSkillMods().find("Perception")->second; // Rolls a D20 + skill mod
            int ogRoll = roll;
            roll += mod;

            std::cout << "Type the total of any other modifiers to add to " << aVec[x].getName() << "'s roll: ";
            std::string extraModStr = "0";
            std::getline(std::cin, extraModStr);

            if (extraModStr == "")
            {
                extraModStr = "0";
            }

            int extraMod = stoi(extraModStr);

            if (extraMod != 0)
            {
                roll += extraMod;
            }

            std::cout << "Rolled a " << roll << " ( Dice roll: " << ogRoll << " + "
                      << "perception modifier: " << mod;
            (extraMod != 0) ? std::cout << " + other bonuses: " << extraMod << " )" << std::endl : std::cout << " )" << std::endl;
            std::cout << std::endl;
            searchInitForTie(roll, aVec[x]); // Changes initiative in the case of a player monster tie
            combatants.insert(std::pair<float, Combatant>(roll, aVec[x]));
        }

        // reset flag
        alreadyRolled = false;
    }
}

void combatTracker::autoInitiative()
{

    // TO DO: IMPLEMENT FULL AUTO INITIATIVE AND STRING PARSING OF USER INPUT
    std::cout << std::endl
              << "Type the corresponding number of any combatant not using Perception for their initiative, \nthen type the number of the respective skill in the following format:\n\"3\"(combatant number) \"4\"(skill number) \"1\"(combatant number) \"1\"(skill number) so that it looks like:\n3 4 1 1\nor type nothing and hit enter to roll perception only\n";

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

    if (combatantSelections != "")
    {
        initStrParser(combatantSelections);
    }

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
            float roll = (rand() % 21); // Rolls a D20

            if (combatantIndex[x] < playerVec.size())
            {
                std::cout << "Type the total of any other modifiers to add to " << playerVec[combatantIndex[x]].getName() << "'s roll: ";

                std::string extraModStr = "0";

                std::getline(std::cin, extraModStr);
                if (extraModStr == "")
                {
                    extraModStr = "0";
                }
                int extraMod = stoi(extraModStr);
                int ogRoll = roll;
                roll += mod;

                if (extraMod != 0)
                {
                    roll += extraMod;
                }

                std::cout << "Rolled a " << roll << " ( Dice roll: " << ogRoll << " + " << iter->first << " modifier: " << mod;
                (extraMod != 0) ? std::cout << " + other modifiers: " << extraMod << " )" << std::endl : std::cout << " )" << std::endl;
                std::cout << std::endl;
                searchInitForTie(roll, playerVec[combatantIndex[x]]); // Changes initiative in the case of a player monster tie
                combatants.insert(std::pair<float, Combatant>(roll, playerVec[combatantIndex[x]]));
            }
            else
            {
                std::cout << "Type the total of any other modifiers to add to " << enemyVec[combatantIndex[x] - playerVec.size()].getName() << "'s roll: ";

                std::string extraModStr = "0";
                std::getline(std::cin, extraModStr);
                if (extraModStr == "")
                {
                    extraModStr = "0";
                }
                int extraMod = stoi(extraModStr);
                int ogRoll = roll;
                roll += mod;

                if (extraMod != 0)
                {
                    roll += extraMod;
                }

                std::cout << "Rolled a " << roll << " ( Dice roll: " << ogRoll << " + " << iter->first << " modifier: " << mod;
                (extraMod != 0) ? std::cout << " + other modifiers: " << extraMod << " )" << std::endl : std::cout << " )" << std::endl;
                std::cout << std::endl;
                searchInitForTie(roll, enemyVec[combatantIndex[x] - playerVec.size()]); // Changes initiative in the case of a player monster tie
                combatants.insert(std::pair<float, Combatant>(roll, enemyVec[combatantIndex[x] - playerVec.size()]));
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

void combatTracker::printInitOrder(std::multimap<float, Combatant, std::greater<float>>::iterator currentTurn, std::multimap<float, Combatant, std::greater<float>> mapToPrint)
{
    std::cout << "Initiative Order\n\n";

    std::string longestName = "";

    // find longest name in the initative for setw()
    for (auto itr = mapToPrint.begin(); itr != mapToPrint.end(); itr++)
    {
        std::string tempName = itr->second.getName();

        if (longestName.length() < tempName.length())
        {
            longestName = tempName;
        }
    }

    // Print initiative order
    for (auto itr = mapToPrint.begin(); itr != mapToPrint.end(); itr++)
    {
        std::string conStr = itr->second.getName() + ": " + std::to_string((int)floor(itr->first));
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

void combatTracker::setPersistentDamage()
{
    std::cout << "Type the numbers of the combatant to add to, modify or remove persistent damage from in the format: 1 4 2.\n\n";
    int count = 1;
    std::string selectionStr = "0";
    for (std::multimap<float, Combatant, std::greater<float>>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        std::cout << count << ". " << itr->second.getName() << std::endl;
        count++;
    }

    std::cin.ignore();
    std::getline(std::cin, selectionStr);

    std::istringstream iss(selectionStr);

    int selection = 0;
    std::vector<int> combatantNums;

    while (iss >> selection)
    {
        std::cout << selection << std::endl;
        combatantNums.push_back(selection);
    }

    for (auto iter = combatantNums.begin(); iter != combatantNums.end(); iter++)
    {
        auto iter2 = combatants.begin();
        std::advance(iter2, *iter - 1);

        std::cout << "Type the type of damage followed by its value or associated dice roll in the format: fire 2 bleed 2d4\n";
        std::cout << "Multiple entries at once are allowed. If an entry already exists you will be prompted to either modify the value or remove it.\n";
        std::string persistentDamageEntry = "";
        // std::cin.ignore();
        std::getline(std::cin, persistentDamageEntry);

        std::istringstream ss(persistentDamageEntry);
        std::string persistentDamageContainer = "";
        bool firstLoop = true;
        std::string persistentDamageName = " ";
        std::string persistentDamageValue = " ";
        bool persistentDamageExists = false;

        while (ss >> persistentDamageContainer)
        {
            if (firstLoop) // first loop grabs name of persistent damage
            {
                firstLoop = false;
                persistentDamageName = persistentDamageContainer;
            }
            else // second loop grabs value of persistent damage then begins insert check
            {
                firstLoop = true;
                persistentDamageValue = persistentDamageContainer;

                std::vector<std::pair<std::string, std::string>> vecToCheck = persistentDamage[iter2->second.getName()];
                int posOfDamage = 0;

                for (auto i = vecToCheck.begin(); i != vecToCheck.end(); i++)
                {
                    if (i->first == persistentDamageName)
                    {
                        persistentDamageExists = true;
                        break;
                    }
                    posOfDamage++;
                }

                if (persistentDamageExists)
                {
                    std::cout << persistentDamageName << " damage already exists on this combatant.\nWould you like to\n1. remove\n2. change value of damage\n";
                    while (1)
                    {
                        int whichOption = validateIntEntry();
                        if (whichOption == 1)
                        {
                            persistentDamage[iter2->second.getName()].erase(persistentDamage[iter2->second.getName()].begin() + posOfDamage);
                            break;
                        }
                        else if (whichOption == 2)
                        {
                            persistentDamage[iter2->second.getName()][posOfDamage].second = persistentDamageValue;
                            break;
                        }
                        else
                        {
                            std::cerr << "Please enter a valid number.\n";
                        }
                    }
                }
                else
                {
                    persistentDamage[iter2->second.getName()].push_back({persistentDamageName, persistentDamageValue});
                }
            }
        }
    }
}

template <typename First, typename... T>
bool is_in(First &&first, T &&...t)
{
    return ((first == t) || ...);
}

void combatTracker::setCondition()
{
    std::cout << "Type numbers of the combatants to update conditions of in the format: 2 1 4.\n\n";
    int count = 1;
    std::string selectionStr = "0";

    for (std::multimap<float, Combatant, std::greater<float>>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        std::cout << count << ". " << itr->second.getName() << std::endl;
        count++;
    }

    std::cin.ignore();
    std::getline(std::cin, selectionStr);

    std::istringstream iss(selectionStr);

    int selection = 0;
    std::vector<int> combatantNums;

    while (iss >> selection)
    {
        combatantNums.push_back(selection);
    }

    printConditions();

    std::string addCondStr = "0";

    bool setValue = false; // checks if user had to set a value which means cin.ignore is necessary for next loop

    for (auto iter = combatantNums.begin(); iter != combatantNums.end(); iter++)
    {
        auto iter2 = combatants.begin();
        std::advance(iter2, *iter - 1);
        std::cout << "Type the numbers of the conditions to modify " << iter2->second.getName() << " in the format: 1 2 7 4" << std::endl;

        if (setValue)
        {
            std::cin.ignore();
        }

        setValue = false;

        std::getline(std::cin, addCondStr);

        std::istringstream ss(addCondStr);
        int addCond = 0;
        int condVal = 0;
        bool conditionExists = false;

        while (ss >> addCond)
        {
            std::vector<std::pair<std::string, int>> vecToCheck = conditions[iter2->second.getName()];
            int posOfCond = 0;

            for (auto i = vecToCheck.begin(); i != vecToCheck.end(); i++)
            {
                if (i->first == conditionArr[addCond - 1])
                {

                    conditionExists = true;
                    break;
                }
                posOfCond++;
            }

            if (!conditionExists)
            {
                if (is_in(addCond, 1, 2, 4, 9, 10, 11, 12, 22, 25, 33, 34, 35))
                {
                    setValue = true;
                    std::cout << "Set value of " << conditionArr[addCond - 1] << std::endl;
                    condVal = validateIntEntry();
                    // INSERT CONDITION HERE WITH VALUE INPUTTED
                    conditions[iter2->second.getName()].push_back({conditionArr[addCond - 1], condVal});
                }
                else if (addCond < 41)
                {
                    // INSERT CONDITION HERE WITH VALUE -1
                    conditions[iter2->second.getName()].push_back({conditionArr[addCond - 1], -1});
                }
                else
                {
                    std::cout << "Please pick a valid number";
                }
            }
            else
            {
                conditionExists = false;
                std::cout << conditionArr[addCond - 1] << " already exists, would you like to\n1. Cancel\n2. Remove the condition\n";
                if (is_in(addCond, 1, 2, 4, 9, 10, 11, 12, 22, 25, 33, 34, 35))
                {
                    std::cout << "3. Change the conditions value\n";
                }

                while (1)
                {
                    int conditionExistsEntry = validateIntEntry();
                    if (conditionExistsEntry == 1)
                    {
                        break;
                    }
                    else if (conditionExistsEntry == 2)
                    {
                        conditions[iter2->second.getName()].erase(conditions[iter2->second.getName()].begin() + posOfCond);
                        break;
                    }
                    else if (conditionExistsEntry == 3)
                    {
                        std::cout << "New value of " << conditionArr[addCond - 1] << ": ";
                        int newVal = validateIntEntry();
                        conditions[iter2->second.getName()].at(posOfCond).second = newVal;
                        break;
                    }
                    else
                    {
                        std::cerr << "Please enter a valid number choice\n";
                    }
                }
            }
        }
    }
}

void combatTracker::initPersistentMap()
{
    for (auto iter = combatants.begin(); iter != combatants.end(); iter++)
    {
        std::vector<std::pair<std::string, std::string>> tempVec;
        persistentDamage.insert({iter->second.getName(), tempVec});
    }
}

void combatTracker::initConditionMap()
{
    for (auto iter = combatants.begin(); iter != combatants.end(); iter++)
    {
        std::vector<std::pair<std::string, int>> tempVec;
        conditions.insert({iter->second.getName(), tempVec});
    }
}

void combatTracker::beginInitiative()
{

    char userInput = ' ';
    // bool startOfTurn = true;

    initConditionMap(); // fill condition map with names of players in the initative

    // ROUNDS
    for (short int z = 1;; z++)
    {

        // TURNS
        for (auto itr = combatants.begin(); itr != combatants.end();)
        {
            // startOfTurn = true;
            //  UPDATE STUNNED AT BEGINNING OF TURN
            //  itr->updateConditions(startOfTurn);

            // std::cout << "\033[2J\033[1;1H"; // clears screen with escape sequences

            std::cout << "ROUND " << z << "\n\n";

            // Initiative Order
            printInitOrder(itr, combatants);

            // CURRENT COMBATANT
            std::cout << "Current Turn\n\n";
            itr->second.printCombatantInfo();
            std::cout << "Conditions: ";
            for (int x = 0; x < conditions[itr->second.getName()].size(); x++)
            {
                std::cout << conditions[itr->second.getName()][x].first;
                if (conditions[itr->second.getName()][x].second > -1)
                {
                    std::cout << " " << conditions[itr->second.getName()][x].second;
                    if (x < conditions[itr->second.getName()].size() - 1)
                    {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << std::endl;
            std::cout << "Persistent Damage: ";
            for (int x = 0; x < persistentDamage[itr->second.getName()].size(); x++)
            {
                std::cout << persistentDamage[itr->second.getName()][x].first;

                std::cout << " " << persistentDamage[itr->second.getName()][x].second;
                if (x < persistentDamage[itr->second.getName()].size() - 1)
                {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl
                      << std::endl;

            std::cout << "n for next turn, p for previous turn, x to change current combatants initiative (Delay)\n";
            std::cout << "c to add, modify, or remove a condition on a combatant, and d to add, modify, or remove persistent damage on a combatant\n";

            while (1)
            {
                std::cin >> userInput;
                if (tolower(userInput) == 'n')
                {
                    itr++;
                    break;
                }
                else if (tolower(userInput) == 'p')
                {
                    // in the case current combatant is first in the initiative when p is typed
                    if (itr == combatants.begin() && z > 1)
                    {
                        itr = combatants.end();
                        itr--;
                        z--;
                    }
                    else if (itr == combatants.begin() && z < 2) //
                    {
                        // do nothing
                    }
                    else
                    {
                        itr--;
                    }

                    break;
                }
                else if (tolower(userInput) == 'x')
                {
                    std::cout << itr->second.getName() << "\'s new initiative value: ";
                    int newInitiative = validateIntEntry();
                    Combatant tempCombatant = itr->second;
                    itr = combatants.erase(itr);
                    combatants.insert(std::pair<float, Combatant>(newInitiative, tempCombatant));

                    break;
                }
                else if (userInput == 'c')
                {
                    setCondition();
                    itr--; // stay on current player, do not go to next in the initative
                    z--;   // stay on current round
                    break;
                }
                else if (userInput == 'd')
                {
                    setPersistentDamage();
                    itr--; // stay on current player, do not go to next in the initative
                    z--;   // stay on current round
                    break;
                }
                else
                {
                    std::cerr << "Invalid command\n";
                }
            }
            // startOfTurn = false;

            // UPDATE FRIGHTENED AT END OF TURN
            // itr->updateConditions(startOfTurn);
        }
    }
}

combatTracker::~combatTracker()
{
}
