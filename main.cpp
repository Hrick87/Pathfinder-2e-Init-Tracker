#include <iostream>
#include "combatTracker.h"
#include "initialize.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace std;

// TODO: Auto enemy name, Print Following Combatants
// BUGS:

int main()
{

    cout << "Initiating combat tracker.\n";
    initialize init;
    combatTracker beginCombat(init.getPlayers(), init.getEnemies());

    return 0;
}
