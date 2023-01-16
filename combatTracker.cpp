#include "combatTracker.h"

combatTracker::combatTracker()
{
    short int playerNum = 0;
    short int enemyNum = 0;
    /* PLAYERS ENTRY */
    cout << "How many players?\n";
    cin >> playerNum;
    cout << endl;
    numCombatants += playerNum;
    for (short int x = 0; x < playerNum; x++)
    {
        Player *aPlayer = new Player; // NEED TO DELETE IN DESTRUCTOR
        Combative *combatant = aPlayer;
        combatants.push_back(*combatant);

        if (x < playerNum - 1)
        {
            cout << "Next player\n\n";
        }
    }

    /* ENEMIES ENTRY */
    cout << "How many enemies?\n";
    cin >> enemyNum;
    cout << endl;
    numCombatants += enemyNum;

    for (short int x = 0; x < enemyNum; x++)
    {
        Enemy *anEnemy = new Enemy; // NEED TO DELETE IN DESTRUCTOR
        Combative *combatant = anEnemy;
        combatants.push_back(*combatant);

        if (x < enemyNum - 1)
        {
            cout << "Next enemy\n\n";
        }
    }

    beginInitiative(combatants);
}

void combatTracker::printInitOrder(list<Combative> combatants)
{
    cout << "Initiative Order\n\n";

    for (list<Combative>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        cout << itr->getName() << ": " << itr->getInit() << endl;
    }

    cout << endl;
}

void combatTracker::printConditions()
{
    cout << "Death and Dying:\n1.Doomed\n2.Dying\n3.Unconscious\n4.Wounded\n\n";
    cout << "Degrees of Detection:\n5.Hidden\n6.Observed\n7.Undetected\n8.Unnoticed\n\n";
    cout << "Lowered Abilities:\n9.Clumsy\n10.Drained\n11.Enfeebled\n12.Stupefied\n\n";
    cout << "Senses:\n13.Blinded\n14.Concealed\n15.Dazzled\n16.Deafened\n17.Invisible\n\n";
    cout << "Other:\n18.Confused\n19.Controlled\n20.Encumbered\n21.Fascinated\n22.Fatigued\n";
    cout << "23.Flatfooted\n24.Fleeing\n25.Frightened\n26.Grabbed\n27.Immobilized\n28.Paralyzed\n";
    cout << "29.Petrified\n30.Prone\n31.Quickened\n32.Restrained\n33.Sickened\n34.Slowed\n";
    cout << "35.Stunned\n\n";
    cout << "Attitudes:\n36.Friendly\n37.Helpful\n38.Hostile\n39.Indifferent\n40.Unfriendly\n\n";
}

void combatTracker::printPDamage()
{
    cout << "Death and Dying:\n1.Doomed\n2.Dying\n3.Unconscious\n4.Wounded\n\n";
    cout << "Degrees of Detection:\n5.Hidden\n6.Observed\n7.Undetected\n8.Unnoticed\n\n";
    cout << "Lowered Abilities:\n9.Clumsy\n10.Drained\n11.Enfeebled\n12.Stupefied\n\n";
    cout << "Senses:\n13.Blinded\n14.Concealed\n15.Dazzled\n16.Deafened\n17.Invisible\n\n";
    cout << "Other:\n18.Confused\n19.Controlled\n20.Encumbered\n21.Fascinated\n22.Fatigued\n";
    cout << "23.Flatfooted\n24.Fleeing\n25.Frightened\n26.Grabbed\n27.Immobilized\n28.Paralyzed\n";
    cout << "29.Petrified\n30.Prone\n31.Quickened\n32.Restrained\n33.Sickened\n34.Slowed\n";
    cout << "35.Stunned\n\n";
    cout << "Attitudes:\n36.Friendly\n37.Helpful\n38.Hostile\n39.Indifferent\n40.Unfriendly\n\n";
}

void combatTracker::setPersistentTracker(list<Combative> &combatants)
{
    cout << "Select combatant to add persistent damage to.\n\n";
    int count = 0;
    int selection = 0;
    map<int, Combative> selected;

    for (list<Combative>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        cout << count << ": " << itr->getName() << endl;
        selected.insert({count, *itr});
        count++;
    }

    cin >> selection;
    Combative *chosen = &(selected.at(selection));
    cout << "Pick a persistent damage(s) to apply\n\n";

    printPDamage();

    chosen->setCondition();
    string chosenName = chosen->getName();

    for (list<Combative>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        if (chosenName == (itr->getName()))
        {
            list<Combative>::iterator it1 = combatants.erase(itr);
            combatants.insert(it1, *chosen);
            break;
        }
    }
}

void combatTracker::setConditionTracker(list<Combative> &combatants)
{
    cout << "Select combatant to add condition to.\n\n";
    int count = 0;
    int selection = 0;
    map<int, Combative> selected;

    for (list<Combative>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        cout << count << ": " << itr->getName() << endl;
        selected.insert({count, *itr});
        count++;
    }

    cin >> selection;
    Combative *chosen = &(selected.at(selection));
    cout << "Pick a condition(s) to apply\n\n";

    printConditions();

    chosen->setCondition();
    string chosenName = chosen->getName();

    for (list<Combative>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
    {
        if (chosenName == (itr->getName()))
        {
            list<Combative>::iterator it1 = combatants.erase(itr);
            combatants.insert(it1, *chosen);
            break;
        }
    }
}



bool initOrder(Combative a, Combative b)
{
    return (a.getInit()) > (b.getInit());
}

void combatTracker::beginInitiative(list<Combative> combatants)
{
    //list<Combative> initOrder = combatOrder(combatants);
    combatants.sort(initOrder);
    printInitOrder(combatants);

    char userInput = ' ';
    bool startOfTurn = true;

    // ROUNDS
    for (short int z = 1;; z++)
    {
        cout << "ROUND " << z << "\n\n";
        // TURNS
        for (list<Combative>::iterator itr = combatants.begin(); itr != combatants.end(); ++itr)
        {
            startOfTurn = true;
            // UPDATE STUNNED AT BEGINNING OF TURN
            itr->updateConditions(startOfTurn);

            // PREVIOUS COMBATANT
            for (list<Combative>::iterator itr2 = combatants.begin(); itr2 != itr; ++itr2)
            {
                itr2->printCombatantInfo();
            }

            // CURRENT COMBATANT
            cout << "Current Turn\n\n";
            itr->printCombatantInfo();
            cout << "n to end turn, c to add a condition to a combatant,\n";
            cout << "p to add persistent damage,\n";
            cout << "r to remove condition or persistent damage,\n";
            cout << "v to change value of condition or persistent damage\n\n";

            while (1)
            {
                cin >> userInput;
                if (userInput == 'n')
                {
                    break;
                }
                else if (userInput == 'c')
                {
                    setConditionTracker(combatants);
                    // PREVIOUS COMBATANT
                    for (list<Combative>::iterator itr2 = combatants.begin(); itr2 != itr; ++itr2)
                    {
                        itr2->printCombatantInfo();
                    }
                    // CURRENT COMBATANT
                    cout << "Current Turn\n\n";
                    itr->printCombatantInfo();
                    cout << "n to end turn, c to add a condition to a combatant,\n";
                    cout << "p to add persistent damage,\n";
                    cout << "r to remove condition or persistent damage,\n";
                    cout << "v to change value of condition or persistent damage\n\n";
                }
                else if (userInput == 'p')
                {
                    setPersistentTracker(combatants);
                    // PREVIOUS COMBATANT
                    for (list<Combative>::iterator itr2 = combatants.begin(); itr2 != itr; ++itr2)
                    {
                        itr2->printCombatantInfo();
                    }
                    // CURRENT COMBATANT
                    cout << "Current Turn\n\n";
                    itr->printCombatantInfo();
                    cout << "n to end turn, c to add a condition to a combatant,\n";
                    cout << "p to add persistent damage,\n";
                    cout << "r to remove condition or persistent damage,\n";
                    cout << "v to change value of condition or persistent damage\n\n";
                }
                else if(userInput == 'r')
                {
                    cout << "UNIMPLEMENTED\n";
                }
                else if(userInput == 'v')
                {
                    cout << "UNIMPLEMENTED\n";
                }
                else
                {
                    cerr << "Invalid command\n";
                }
            }
            startOfTurn = false;

            // UPDATE FRIGHTENED AT END OF TURN
            itr->updateConditions(startOfTurn);
        }
    }
}

combatTracker::~combatTracker()
{
    /*for (int x = 0; x < playerMap.size(); x++)
    {
        delete playerMap
    }*/
}
