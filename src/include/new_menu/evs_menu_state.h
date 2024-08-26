#ifndef EVS_MENU_STATE_H
#define EVS_MENU_STATE_H

#include <pokeagb/pokeagb.h>
#include "../../config.h"

struct EvsMenuState{
    u8 curr_stat_pos; //ranging from 0 to 5, where 0 is atk and 5 is speed
    u8 curr_selected_pkmn;

    u32 start_evs[6]; //hp, atk, def, spatk, spdef, speed
    u32 curr_evs [6]; //hp, atk, def, spatk, spdef, speed

    pchar str_buff[10];

    u32 curr_price;
    bool curr_price_is_neg;

};

struct EvsMenuState *evs_menu_state = (struct EvsMenuState *)EVS_MENU_STATE_START;

#endif