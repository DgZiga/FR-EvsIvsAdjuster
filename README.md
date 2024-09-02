# EvsAdjusterMenu
Adds a menu that enables EVs adjustments

## How to insert
This tool requires:
- python (tested with 3.8.3)
- [grit](https://www.coranac.com/projects/grit/) 0.8.6+
- [devkitARM](https://devkitpro.org/wiki/devkitARM)

Once you have all the necessary software installed and ready to go, just navigate to the root of this project and copy/paste your ROM there. Rename the file to `BPRE0.gba`.  
Open the terminal, navigate to the root of the project, then run `python scripts/build.py 0xFFFFFFFF`, where `0xFFFFFFFF` is the free space in your ROM, and the script should insert everything, creating two new files:
- `multi.gba`: the updated ROM
- `symbols.txt`: a text file containing all the addresses known to the compiler. Conserve this file to know where everything in your ROM was inserted

## How to use
Once inserted in the rom, compile a script that uses `special 0x9F` to select a pokemon, check that the selected pokemon index is != 6 (index 6 = cancel button), and `callasm` the `evs_ivs_menu_entrypoint` symbol recovered from `symbols.txt`.  
Example script:
```
lock
special 0x9F
waitstate
#check var 0x8004 is != 6
callasm evs_ivs_menu_entrypoint|1
waitstate
release
end
```

## Notes
Please note that the routine will not auto-adjust the current stat value of the pokémon: a level up (or an external routine) is needed for that.

## Internal workings
The menu leverages the [Menu Engine](https://github.com/DgZiga/FR-MenuEngine) with some additional changes, such as updating all rboxes in the vblank handler.
