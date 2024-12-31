# swsh-mods-exl
### Configurable [exlaunch](https://github.com/shadowninja108/exlaunch)-based mods for Pokemon Sword and Shield 
### v.1.3.2 (latest version) only !

## Installation (Console)
1. Download the [latest release](https://github.com/Lincoln-LM/swsh-mods-exl/releases/tag/release) for your game version
1. Unzip the release to your sd card
1. Edit ``sd:/config/swsh-mods-exl/config.toml`` to your liking

## Installation (Ryujinx)
1. Download the [latest release](https://github.com/Lincoln-LM/swsh-mods-exl/releases/tag/release) for your game version
    - Extract the ``atmosphere/contents/<title_id>`` folder and rename it to ``swsh-mods-exl`` as well as the ``config`` folder
1. Right click on the game in Ryujinx and select "Open Atmosphere Mods Directory"
1. Place the ``swsh-mods-exl`` folder in the opened directory
1. Navigate out of the ``sdcard/atmosphere/contents/<title_id>`` directory to the ``sdcard`` directory and place the ``config`` folder there

## Mods

### Overworld Shiny Indicatiors ``[overworld_shiny]``

Mod that makes shiny pokemon visible in the overworld with a sound indicator & particle effect

#### Example
<!-- TODO: new video with particle effects -->
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/f2a553aa-2f44-40c4-bf16-9b0dd924e1bf

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
- ``sound``
    - Controls the "command" that is run upon shiny spawns
    - Leave empty (``""``) for no command
    - Sound effects are commands and that is the intended use of this setting
    - string (``""``, ``"Play_Camp_Cooking_Explosion"``, etc.)
- ``shiny_ptcl``
    - Controls the particle effect that is played around a shiny
    - Leave empty (``""``) for no particle
    - Any .ptcl in romfs or custom layeredfs should work but needs to be configured to repeat
    - string (``""``, ``"bin/field/effect/particle/particle/ef_poke_symbol_aura.ptcl"``, etc.)
- ``show_aura_for_brilliants``
    - Controls whether or not regular brilliant spawns show their aura
    - boolean (``true``, ``false``)
- ``include_battle_sounds``
    - Controls whether or not to load the battle sound bank in the overworld (needed for battle shiny sound)
    - boolean (``true``, ``false``)
- ``play_sound_for_following``
    - Controls whether or not to play the shiny sound when the following pokemon respawns
    - boolean (``true``, ``false``)
- ``show_ptcl_for_following``
    - Controls whether or not to show the shiny particle for the following pokemon
    - boolean (``true``, ``false``)
- ``show_message_box``
    - Controls whether or not to show a message box on screen when a shiny spawns
    - boolean (``true``, ``false``)
- ``boosted_percentage``
    - Controls the percentage for the modified overworld shiny odds
    - Set to 0 for regular shiny odds
    - This setting is intended to be used to ensure shiny models are functioning properly, not as a standalone shiny odds boost cheat
    - integer [0, 100]

#### Notes
- Hooks the check that determines whether or not a PokemonModel should display shininess to always return true (normally only true for following pokemon)
    - If the PokemonModel is shiny -> call SendCommand to trigger the sound effect
- Hooks the functions responsible for adding & playing the particle effects to supply a custom .ptcl path
- Hooks the brilliant aura check (only for displaying the effect) for both EncountObjects and FishingPoints to check the shiny flag or the brilliant flag
    - FishingPoints do not use PokemonModels so the sound effect for them is triggered here
- Hooks the instruction that sets the shiny flag to call an external PRNG to determine shininess based on percentage (``randU64() % 100 < boosted_rate``)

### Underworld ``[underworld]``

Mod that makes [symbol encounters](https://bulbapedia.bulbagarden.net/wiki/Symbol_encounter) pull from the encounter tables of [hidden encounters](https://bulbapedia.bulbagarden.net/wiki/Hidden_encounter), allowing "underworld"-exclusive pokemon to spawn and be visible in the overworld

This is a reimplementation of [norainthefuture](https://twitter.com/norainthefuture)'s mod which accomplishes the swapping via exefs patching rather than manually swapping the data tables. The idea is entirely by her!

#### Example
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/06888db2-8cd5-409a-931d-5fa9fb14f62e

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
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/c598db1b-d42d-4f3f-b5ff-db5652c9dfc0


#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)

#### Notes
- Hooks the functions responsible for setting species & form for symbol, hidden, and gimmick encounters to overwrite the chosen species and form
    - A random species (randU64() % 898 + 1) and form (randU64() % form_count) are generated until one is found that exists in game

### Camera Tweaks ``[camera_tweaks]``

Mod that overwrites the default camera constants to allow tweaking of camera speed, pitch range, and field of view

Idea & recommendations by [norainthefuture](https://twitter.com/norainthefuture)

#### Example
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/e9f19b4a-16f8-4bcb-973e-67ccfa19e6aa

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

### Level Cap Removal ``[uncap_level]``

Mod that removes the level cap-based shiny lock & (optionally) catch lock

#### Example
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/067555d6-4efc-4f55-8fff-73e8ffed8d3b

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
- ``fully``
    - Controls whether or not all level caps are removed
    - Must be set to true for level capped pokemon to be catchable
    - boolean (``true``, ``false``)

#### Notes
- Hooks the functions that set level cap based on badge count
    - if ``fully`` is set to ``true``: always return 100, otherwise: only return 100 when shiny locking

### Freecam ``[freecam]``

Mod that allows detaching the camera from the player and moving it on its own

#### Example
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/87cf634b-e205-48ff-88a2-a09ee89119e1

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
- ``disable_terrain_culling``
    - Controls whether or not the game's automatic culling of far away terrain is disabled
    - boolean (``true``, ``false``)
- ``always_use_extended_camera``
    - Controls whether or not the extended camera (the wild area camera that you can adjust) is enabled by default in all areas
    - boolean (``true``, ``false``)

#### Notes
- Manual camera angle adjustment only possible on "Wide Roads" (wild area/ioa/ct) unless ``always_use_extended_camera`` is set
- Inputs only register from "Npad" controllers (only tested on pro controller)

### Controls
- R+A to detach/reattach the camera from the player
- In detached state:
    - D-Pad to move camera
    - L/R to decrease/increase movement speed
    - ZL/ZR to decrease/increase height

### Glimwood w/ Overworld Spawns ``[glimwood_overworld]``

Mod that overwrites the maximum symbol encounter spawn count for spawners in glimwood tangle

#### Example
https://github.com/Lincoln-LM/swsh-mods-exl/assets/73306575/ada85d46-1754-4750-b909-49cd5649466e

#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)
- ``maximum_spawns``
    - Maximum symbol count per spawner
    - int

#### Notes
- Overwrites the maximum symbol count field on any new EncountSpawner with a hash within the hard-coded glimwood list

### Synchro Mode ``[synchro_mode]``

Mod that allows you to pilot your following pokemon

Idea & recommendations by [norainthefuture](https://twitter.com/norainthefuture)

#### Example


#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)

#### Notes
- Inputs only register from "Npad" controllers (only tested on pro controller)
- Movement can be buggy at times
- If the following pokemon gets forcefully despawned you may teleport to (0,0)

### Controls
- L+R to toggle in and out of the state

### Extended Following ``[extended_following]``

Mod that allows following pokemon in the galar mainland wild area

#### Example


#### Config
- ``active``
    - Controls whether or not the mod is activated
    - boolean (``true``, ``false``)

<!-- #### Notes -->
