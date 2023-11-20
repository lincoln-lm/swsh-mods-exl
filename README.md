# swsh-mods-exl
### Configurable [exlaunch](https://github.com/shadowninja108/exlaunch)-based mods for Pokemon Sword and Shield


## Mods

### Overworld Shiny Indicatiors ``[overworld_shiny]``

Mod that makes shiny pokemon visible in the overworld with a sound indicator & aura effect

#### Example
![]()

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
- ``sound``
    - Controls the "command" that is run upon shiny spawns
    - Leave empty (``""``) for no command
    - Sound effects are commands and that is the intended use of this setting
    - string (``""``, ``"Play_Camp_Cooking_Explosion"``, etc.)
- ``repurpose_aura``
    - Controls whether or not the brilliant aura effect is used as a shiny indicator
    - Brilliant auras will only appear around shiny pokemon and not regular brilliant spawns
    - Brilliant spawns still happen as normal but will not have the indicator
    - boolean (``true``, ``false``)
- ``boosted_percentage``
    - Controls the percentage for the modified overworld shiny odds
    - Set to 0 for regular shiny odds
    - This setting is intended to be used to ensure shiny models are functioning properly, not as a standalone shiny odds boost cheat
    - integer [0, 100]

#### Notes
- Hooks the check that determines whether or not a PokemonModel should display shininess to always return true (normally only true for following pokemon)
    - If the PokemonModel is shiny -> call SendCommand to trigger the sound effect
- Hooks the brilliant aura check (only for displaying the effect) for both EncountObjects and FishingPoints to check the shiny flag instead of the brilliant flag
    - FishingPoints do not use PokemonModels so the sound effect for them is triggered here
- Hooks the instruction that sets the shiny flag to call an external PRNG to determine shininess based on percentage (``randU64() % 100 < boosted_rate``)

### Underworld ``[underworld]``

Mod that makes [symbol encounters](https://bulbapedia.bulbagarden.net/wiki/Symbol_encounter) pull from the encounter tables of [hidden encounters](https://bulbapedia.bulbagarden.net/wiki/Hidden_encounter), allowing "underworld"-exclusive pokemon to spawn and be visible in the overworld

#### Example
![]()

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
<!-- TODO -->
<!-- - ``disable_brilliants``
    - Controls whether or not brilliant spawns are possible for hidden encounters
    - This option exists to preserve legality of the pokemon generated with this mod active as hidden encounters cannot naturally be brilliant
    - boolean (``true``, ``false``) -->

#### Notes
- Hooks function responsible for generating SymbolEncounts to write the proper hidden table to the encounter table parameter
    - In the vanilla game, each EncountSpawner is assigned a hash and the symbol encounter table is stored within the spawner object upon initialization
    - The hook needs to actively pull the proper encounter table based on player location every new symbol spawn (hidden encounts naturally do this)


### Live Randomizer ``[randomizer]``

Mod that live randomizes the species and form of all overworld spawns rather than statically randomizing the encounter tables they pull from

#### Example
![]()

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)

#### Notes
- Hooks the functions responsible for setting species & form for symbol, hidden, and gimmick encounters to overwrite the chosen species and form
    - A random species (randU64() % 898 + 1) and form (randU64() % form_count) are generated until one is found that exists in game

### Camera Tweaks ``[camera_tweaks]``

Mod that overwrites the default camera constants to allow tweaking of camera speed, pitch range, and field of view

#### Example
![]()

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
- ``adjustment_speed``
    - Controls the speed of the camera
    - Value is in degrees per time unit
    - float [``0.0``, ``360.0``)
- ``min_pitch``
    - Controls the minimum pitch of the camera
    - Value is in degrees
    - float [``-180.0``, ``180.0``)
- ``max_pitch``
    - Controls the maximum pitch of the camera
    - Value is in degrees
    - float [``-180.0``, ``180.0``)
- ``min_distance``
    - Controls the distance of the camera when zoomed in
    - float [``0.0``, ``...``)
- ``max_distance``
    - Controls the distance of the camera when zoomed out
    - float [``0.0``, ``...``)

#### Notes
- Hooks the camera constructor to overwrite the applicable constants