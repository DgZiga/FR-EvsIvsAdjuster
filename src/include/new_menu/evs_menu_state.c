#ifndef EVS_MENU_STATE_C
#define EVS_MENU_STATE_C

#include "evs_menu_state.h"

void init_evs_menu_state(){
    evs_menu_state->curr_stat_pos=0;
    evs_menu_state->curr_selected_pkmn=0;
    evs_menu_state->curr_price=0;
    evs_menu_state->curr_price_is_neg=false;
    memset(evs_menu_state->player_money_str_buff, 0, sizeof(evs_menu_state->player_money_str_buff));
    memset(evs_menu_state->str_buff, 0, sizeof(evs_menu_state->str_buff));
    memset(evs_menu_state->concat_str_buff, 0, sizeof(evs_menu_state->concat_str_buff));
}

#endif