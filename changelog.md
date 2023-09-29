## 1.0.0

### System Improvements
-   Added rudimentary persistent damage tracker
-   Allowed for the altering and removal of conditions and persistent damage

### Bug Fixes

-   Fixed bug that incremented the round counter when conditions or persistent damage was modified, removed, or applied
-   Enemy combatants go before players now on a tied initiative roll as per the Pathfinder Second Edition rules

## 0.9.0

### System Improvements

-   JSON compatibility with [Pathbuilder 2e](https://pathbuilder2e.com/) and the [FoundryVTT Pathfinder 2e](https://github.com/foundryvtt/pf2e) system's creature JSONs [readme.md](./readme.md) on how to integrate them into the program
-   Conditions and their associated values if needed are now tracked
-   Initiative order can now be changed mid initiative to allow for the Delay action
-   Initiative can now be automatically rolled or manually entered
-   Initiative now allows for skill rolls with all skills excluding lores as per the Pathfinder Second Edition Ruleset
-   User can now move to a previous combatant or round as well as forward
-   All input is now validated and the program will no longer close or segfault due to invalid input
