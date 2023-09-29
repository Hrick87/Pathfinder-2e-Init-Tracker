#define NUMOFSKILLS 17 // includes perception
#define NUMOFABILITIES 6

#include "initialize.h"

// Perception is handled manually in getEnemySkills()
const std::string SKILL_LIST[16] = {
    "Acrobatics", "Arcana", "Athletics", "Crafting",
    "Deception", "Diplomacy", "Intimidation",
    "Medicine", "Nature", "Occultism", "Performance",
    "Religion", "Society", "Stealth", "Survival", "Thievery"};

namespace fs = std::experimental::filesystem;

initialize::initialize()
{
    std::cout << "1. Read players and enemies from JSON files\n2. Manually input players and enemies\n";

    char userInput = ' ';

    while (std::cin >> userInput)

        if (toupper(userInput) == '1')
        {
            std::cout << "Reading from JSON files...\n";
            readInPlayerJSON(); // populates playerVec with Combatant objects and their respective JSON data
            readInEnemyJSON();  // populates enemyVec with Combatant objects and their respective JSON data
            break;
        }
        else if (toupper(userInput) == '2')
        {
            manualPlayerEntry(); // populates playerVec with Combatant objects manually inputted
            manualEnemyEntry();  // populates enemyVec with Combatant objects manually inputted
            break;
        }
        else
        {
            std::cout << "Please enter valid number.\n";
        }
}

void initialize::manualPlayerEntry()
{
    char anotherPlayer = 'Y';

    std::cout << "Enter Character Information\n";
    Combatant newPlayer;
    std::cout << "Enter another player?\nY/N\n";

    playerVec.push_back(newPlayer);

    while (std::cin >> anotherPlayer)
    {
        if (toupper(anotherPlayer) == 'Y')
        {
            std::cout << "Enter Character Information\n";
            Combatant newPlayer;
            playerVec.push_back(newPlayer);
            std::cout << "Enter another player?\nY/N\n";
        }
        else if (toupper(anotherPlayer) == 'N')
        {
            break;
        }
        else
        {
            std::cerr << "Please enter a valid command\n";
        }
    }
}

void initialize::manualEnemyEntry()
{
    char anotherEnemy = 'Y';

    std::cout << "Enter Enemy Information\n";
    Combatant newEnemy;
    std::cout << "Enter another Enemy?\nY/N\n";

    enemyVec.push_back(newEnemy);

    while (std::cin >> anotherEnemy)
    {
        if (toupper(anotherEnemy) == 'Y')
        {
            std::cout << "Enter enemy information\n";
            Combatant newEnemy;
            enemyVec.push_back(newEnemy);
            std::cout << "Enter another enemy?\nY/N\n";
        }
        else if (toupper(anotherEnemy) == 'N')
        {
            break;
        }
        else
        {
            std::cerr << "Please enter a valid command\n";
        }
    }
}
std::map<std::string, int> initialize::getPlayerSkillMods(rapidjson::Document &player)
{

    int playerLevel = player["build"]["level"].GetInt();

    /*str, dex, con, int, wis, cha*/
    int abilityMods[NUMOFABILITIES] = {0};

    // convert ability scores to ability mods
    int i = 0;
    for (auto &m : player["build"]["abilities"].GetObject())
    {
        if (i > 5)
        {
            break;
        }

        abilityMods[i] = calculateAbilityMod(m.value.GetInt());
        i++;
    }

    // Account for each lore skill + number of normal skills
    std::map<std::string, int> skillMods;

    // perception is in an earlier special location
    skillMods.insert(std::pair<std::string, int>("Perception", playerLevel + abilityMods[3] + player["build"]["proficiencies"]["perception"].GetInt()));

    int x = 0;

    for (rapidjson::Value::ConstMemberIterator itr = player["build"]["proficiencies"].MemberBegin() + 17; x < skillMods.size(); itr++)
    {
        // Begin adding lores at index 17

        if (x >= 16)
        {
            for (rapidjson::SizeType y = 0; y < player["build"]["lores"].Size(); y++)
            {
                skillMods.insert(std::pair<std::string, int>(player["build"]["lores"][y][0u].GetString(), abilityMods[3] + playerLevel + player["build"]["lores"][y][1u].GetInt()));
                x++;
            }
        }
        else if (x == 10) // Perception already in this index
        {
            // do nothing
            x++;
        }
        else // starts at Acrobatics and makes its way down the list, skipping perception and ending before lores
        {
            /*static const char *kTypeNames[] =
                {"Null", "False", "True", "Object", "Array", "String", "Number"};
            std::cout << "x = " << x << " and the value stored here is a: " << kTypeNames[itr->value.GetType()] << " with the name: " << itr->name.GetString() << std::endl;
            */
            skillMods.insert(std::pair<std::string, int>(itr->name.GetString(), playerLevel + itr->value.GetInt()));
            x++;
        }
    }

    // ADD ABILITY MODS TO SKILL MODS EXCEPT PERCEPTION AND LORE WHICH IS TAKEN CARE OF ABOVE

    // str based skills
    skillMods["athletics"] += abilityMods[0]; // Athletics

    // dex based skills
    skillMods["acrobatics"] += abilityMods[1]; // Acrobatics
    skillMods["stealth"] += abilityMods[1];    // Stealth
    skillMods["thievery"] += abilityMods[1];   // Thievery

    // int based skills
    skillMods["arcana"] += abilityMods[3];    // Arcana
    skillMods["crafting"] += abilityMods[3];  // Crafting
    skillMods["occultism"] += abilityMods[3]; // Occultism
    skillMods["religion"] += abilityMods[3];  // Religion
    skillMods["society"] += abilityMods[3];   // Society

    // wis based skills
    skillMods["medicine"] += abilityMods[4]; // Medicine
    skillMods["nature"] += abilityMods[4];   // Nature
    skillMods["survival"] += abilityMods[4]; // Survival

    // cha based skills
    skillMods["deception"] += abilityMods[5];    // Deception
    skillMods["diplomacy"] += abilityMods[5];    // Diplomacy
    skillMods["intimidation"] += abilityMods[5]; // Intimidation
    skillMods["performance"] += abilityMods[5];  // Performance

    return skillMods;
}

void initialize::readInPlayerJSON()
{
    int numOfPlayerFiles = 0;

    for (auto &p : fs::directory_iterator("jsons/players"))
    {
        numOfPlayerFiles++;
    }

    char readBuffer[65536];
    const char *fileNameFormat = "jsons/players/player%d.json";
    char fileName[strlen(fileNameFormat) + 1];

    for (int x = 1; x <= numOfPlayerFiles; x++)
    {
        snprintf(fileName, sizeof(fileName), fileNameFormat, x);
        std::cout << fileName << std::endl;
        FILE *fp = fopen(fileName, "rb"); // non-Windows use "r"

        if (fp == NULL)
        {
            std::cerr << "file did not open correctly\n"
                      << std::endl;
        }
        else
        {

            rapidjson::Document player;
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

            player.ParseStream(is);

            fclose(fp);

            std::string charName = player["build"]["name"].GetString();

            std::cout << "Character name is: " << charName << std::endl;

            int charHp = player["build"]["attributes"]["ancestryhp"].GetInt() +
                         player["build"]["attributes"]["classhp"].GetInt() +
                         player["build"]["attributes"]["bonushp"].GetInt() +
                         player["build"]["attributes"]["bonushpPerLevel"].GetInt();

            std::cout << "Hit points are: " << charHp << std::endl;

            int charAc = player["build"]["acTotal"]["acTotal"].GetInt();

            std::cout << "AC is: " << charAc << std::endl;

            std::map<std::string, int> skillMods = getPlayerSkillMods(player);

            // std::cout << charName << "'s lore is: " << skillMods[17] << std::endl;

            Combatant newPlayer(charName, charHp, charAc, skillMods, true);

            try
            {
                playerVec.push_back(newPlayer);
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}

int initialize::calculateAbilityMod(int abilityScore)
{
    int mod = 0;
    if (abilityScore < 8)
    {
        mod = -2;
    }
    else if (abilityScore < 10)
    {
        mod = -1;
    }
    else if (abilityScore < 12)
    {
        mod = 0;
    }
    else if (abilityScore < 14)
    {
        mod = 1;
    }
    else if (abilityScore < 16)
    {
        mod = 2;
    }
    else if (abilityScore < 18)
    {
        mod = 3;
    }
    else if (abilityScore < 20)
    {
        mod = 4;
    }
    else if (abilityScore < 22)
    {
        mod = 5;
    }
    else if (abilityScore < 24)
    {
        mod = 6;
    }
    else
    {
        std::cerr << "ability score is outside normal game rules\n";
    }

    return mod;
}

void initialize::readInEnemyJSON()
{

    for (auto &p : fs::directory_iterator("jsons/enemies"))
    {
        const char *fileName = p.path().c_str();

        char readBuffer[65536];

        std::cout << fileName << std::endl;
        FILE *fp = fopen(fileName, "rb"); // non-Windows use "r"

        if (fp == NULL)
        {
            std::cerr << "file did not open correctly\n"
                      << std::endl;
        }
        else
        {
            rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

            rapidjson::Document enemy;

            enemy.ParseStream(is);

            fclose(fp);

            std::string enemyName = "";
            // Try and grab name from token first so it doesn't contain context in the name
            if (enemy.HasMember("prototypeToken"))
            {
                enemyName = enemy["prototypeToken"]["name"].GetString();
            }
            else
            {
                // If prototype is missing grab from the name field instead
                enemyName = enemy["name"].GetString();
            }

            std::cout << "Enemy name is: " << enemyName << std::endl;

            int enemyHp = enemy["system"]["attributes"]["hp"]["value"].GetInt();

            std::cout << "Hit points are: " << enemyHp << std::endl;

            int enemyAc = enemy["system"]["attributes"]["ac"]["value"].GetInt();

            std::cout << "Enemy AC is: " << enemyAc << std::endl;

            // IMPLEMENT ENEMY SKILL MODS
            std::map<std::string, int> skillMods = getEnemySkillMods(enemy);

            Combatant newEnemy(enemyName, enemyHp, enemyAc, skillMods, false);

            enemyVec.push_back(newEnemy);
        }
    }
}

std::map<std::string, int> initialize::getEnemySkillMods(rapidjson::Document &enemy)
{
    std::map<std::string, int> skillMods;
    const rapidjson::Value &itemArray = enemy["items"];

    skillMods.insert(std::pair<std::string, int>("Perception", enemy["system"]["attributes"]["perception"]["value"].GetInt()));

    for (rapidjson::SizeType i = 0; i < itemArray.Size(); i++)
    {
        std::string itemName = itemArray[i]["name"].GetString();
        for (int x = 0; x < sizeof(SKILL_LIST) / sizeof(SKILL_LIST[0]); x++)
        {
            // Checks to see if item name is any of the skills in skillList.h
            if (itemName == SKILL_LIST[x])
            {
                std::transform(itemName.begin(), itemName.end(), itemName.begin(), ::tolower);
                skillMods.insert(std::pair<std::string, int>((itemName), itemArray[i]["system"]["mod"]["value"].GetInt()));
            }
        }
    }

    for (int x = 0; x < sizeof(SKILL_LIST) / sizeof(SKILL_LIST[0]); x++)
    {
        std::string skill = SKILL_LIST[x];
        std::transform(skill.begin(), skill.end(), skill.begin(), ::tolower);
        skillMods.insert(std::pair<std::string, int>(skill, 0));
    }

    return skillMods;
}
