#ifndef NEW_MENU_H
#define NEW_MENU_H
#include <pokeagb/pokeagb.h>
#include "../interface_definition.h"
#include "built_graphics/0_bg0.h"
#include "built_graphics/1_new_menu_gui.h"
#include "built_graphics/2_bg2.h"
#include "built_graphics/s_greenblock.h"
#define rgb5(r, g, b) (u16)((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))

extern const struct InterfaceDefinition NEW_MENU_DEFINITION;
#include "../../engine/gui.h"
#include "../../engine/sprite_util.h"
#include "../../config.h"
#include "evs_menu_state.h"

#define CURSOR_TILES_TAG 0x6F
#define CURSOR_PALS_TAG 0x08


#endif