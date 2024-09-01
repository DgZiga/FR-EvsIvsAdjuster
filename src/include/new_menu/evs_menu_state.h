#ifndef EVS_MENU_STATE_H
#define EVS_MENU_STATE_H

#include <pokeagb/pokeagb.h>
#include "../../config.h"

enum EvsMenuStateMode {
    EVS, CONFIRM
};

struct EvsMenuState{
    u8 curr_stat_pos; //in EVS mode: ranging from 0 to 5, where 0 is atk and 5 is speed. in CONFIRM mode: 0 is confirm, 1 is back
    u8 curr_selected_pkmn;

    u32 start_evs[6]; //hp, atk, def, spatk, spdef, speed
    u32 curr_evs [6]; //hp, atk, def, spatk, spdef, speed

    pchar str_buff[10];
    pchar player_money_str_buff[10];
    pchar concat_str_buff[21];

    u32 curr_price;
    bool curr_price_is_neg;

    u8 evs_bars_oam_ids[6][2]; //first index is stat, second is first or second bar
    u8 cursor_oam_id;

    enum EvsMenuStateMode curr_mode; 
};

struct EvsMenuState *evs_menu_state = (struct EvsMenuState *)EVS_MENU_STATE_START;

void init_evs_menu_state();

#endif