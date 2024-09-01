#ifndef ENGINE_MAIN_H
#define ENGINE_MAIN_H

#include <pokeagb/pokeagb.h>
#include "../interface_definition.h"
int new_gui_main(const struct InterfaceDefinition* def);
POKEAGB_EXTERN void run_eventually_start(u8 task_id);


#include "../include/agb_debug/debug.c"
#include "../interface_definition.h"
#include "setup.h"
#include "gui.h"


#define CURSOR_TILES_TAG 0x6F
#define CURSOR_PALS_TAG 0x08
struct InterfaceDefinition** interface_def_addr = (struct InterfaceDefinition**)(&(party_opponent[0]).base.nick[8]);


#endif