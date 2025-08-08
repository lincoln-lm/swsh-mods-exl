# swsh-challenge-pack

### Configurable IronMon-like challenge mods for Pok&eacute;mon Sword and Shield

### v.1.3.2 (latest version) only !

## Features
- In-game configuration menu for the various features
- RNG seed option for synchronizing runs
- Permadeath (when a Pok&eacute;mon faints it is deleted)
    - Toggleable
- Nuzlocke route restriction (no Pok&eacute;mon will spawn after one is caught or defeated on a route)
    - Toggleable
- 1.5x level modifier for trainer & wild encounters
    - Toggleable
    - Optionally increase the capture level cap by 1.5x as well
- Shop filter (shops will only sell Pok&eacute; balls)
    - Toggleable
- Randomized evolutions
    - Toggleable
    - Optionally match at least 1 type with previous stage
    - Optionally make every Pok&eacute;mon evolve each level
- Randomized learnsets
    - Toggleable
- Randomized field items
    - Toggleable
- Randomized gift tms
    - Toggleable
- Randomized personal info
    - Toggleable
    - Includes:
        - Base stats (in the style of UPR ZX)
        - Abilities
        - Wild held items
- Randomized trainer teams
    - Toggleable
- Randomized wild encounters
    - Toggleable
    - Optionally "live" randomizes which generates a random species/form each spawn instead of selecting from a randomly generated encounter table
- Randomized scripted encounters (box legends, regis, motostoke gym)
    - Toggleable
- Randomized gift encounters
    - Toggleable
- Pok&eacute;mon model randomization (any non-encounter Pok&eacute;mon you see throughout the world)
    - Toggleable
    - Things like starters and scripted encounters will match the species they are randomized to be
    - Optionally hide the starters (displays as Pikachu)

## Known Issues
- Gift TM randomization is not yet implemented for all TMs
- Matching model randomization is not yet implemented for all gifts/scripted encounters
- Raids aren't randomized and don't abide by the nuzlocke restriction
- Berry trees don't abide by the nuzlocke restriction

## Installation (Console)
1. Download the [latest release](https://github.com/lincoln-lm/swsh-challenge-pack/releases/tag/release) for your game version
1. Unzip the release to your sd card
    - The ``atmosphere`` folder should combine with the existing one on the root of the sd card

## Installation (Eden, recommended emulator)
1. Download the [latest release](https://github.com/Lincoln-LM/swsh-challenge-pack/releases/tag/release) for your game version
    - Extract the ``atmosphere/contents/<title_id>`` folder and rename it to ``swsh-challenge-pack``
1. Right click on the game in eden and select "Open Mod Data Location"
1. Place the ``swsh-challenge-pack`` folder in the opened directory

## Installation (Ryujinx)
1. Download the [latest release](https://github.com/Lincoln-LM/swsh-challenge-pack/releases/tag/release) for your game version
    - Extract the ``atmosphere/contents/<title_id>`` folder and rename it to ``swsh-challenge-pack``
1. Right click on the game in Ryujinx and select "Open Atmosphere Mods Directory"
1. Place the ``swsh-challenge-pack`` folder in the opened directory

## Credits
- [IronMon Rules](https://gist.github.com/valiant-code/adb18d248fa0fae7da6b639e2ee8f9c1) for inspiration
- [pkNX](https://github.com/kwsch/pkNX/tree/master) for some game data formats
- [UPR ZX](https://github.com/Ajarmar/universal-Pok&eacute;mon-randomizer-zx) for inspiration & some implementation details
- [exlaunch](https://github.com/shadowninja108/exlaunch) for the hooking framework
