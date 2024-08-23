#ifndef EVS_MENU_STATE_H
#define EVS_MENU_STATE_H

#include <pokeagb/pokeagb.h>
#include "../../config.h"

struct EvsMenuState{
    u8 curr_stat_pos; //ranging from 0 to 5, where 0 is atk and 5 is speed
};

struct EvsMenuState *evs_menu_state = (struct EvsMenuState *)EVS_MENU_STATE_START;

#endif