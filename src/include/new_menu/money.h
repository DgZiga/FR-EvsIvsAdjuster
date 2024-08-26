#ifndef MONEY_H
#define MONEY_H

#include <pokeagb/pokeagb.h>
#include "evs_menu_state.h"

void calc_price();
void set_player_money(u32 val);
u32 get_player_money();
void fmt_money(u32 price, bool isNeg);

#endif