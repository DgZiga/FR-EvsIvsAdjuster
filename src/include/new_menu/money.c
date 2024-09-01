#ifndef MONEY_C
#define MONEY_C

#include "money.h"


POKEAGB_EXTERN u32 decrypt_player_money(u32 in); //defined in BPRE.ld
POKEAGB_EXTERN void encrypt_player_money(u32 in, u32 money); //defined in BPRE.ld

u32 get_player_money(){ //stolen from 0813F6D0
    return decrypt_player_money((*(u32 *)0x03005008)+0x290);
}

void set_player_money(u32 val){ //stolen from 0809FDD8
    return encrypt_player_money((*(u32 *)0x03005008)+0x290, val);
}

void fmt_money(u32 price, bool isNeg){
    fmt_int_10(evs_menu_state->str_buff, price, 0, sizeof(evs_menu_state->str_buff));
    if(isNeg){
        for(s8 i=sizeof(evs_menu_state->str_buff); i>=0; i--){
            if(i!=0){
                evs_menu_state->str_buff[i]=evs_menu_state->str_buff[i-1];
            }else{
                evs_menu_state->str_buff[0] = 0xAE;
            }
        }
    }
    for(u8 i=0; i<sizeof(evs_menu_state->str_buff); i++){
        if(evs_menu_state->str_buff[i]==0xFF){
            evs_menu_state->str_buff[i]=0xB7; //pokesign
            evs_menu_state->str_buff[i+1]=0xFF;
            break;
        }
    }
}


void calc_price(bool gfx_upd){
    s32 delta = 0;
    for(u8 i=0; i<6; i++){
        if(evs_menu_state->curr_evs[i] > evs_menu_state->start_evs[i]){
            delta += evs_menu_state->curr_evs[i] - evs_menu_state->start_evs[i];
        } else {
            delta -= evs_menu_state->start_evs[i] - evs_menu_state->curr_evs[i] ;
        }
    }
    bool isNeg = false;
    if(delta < 0){
        isNeg = true;
        delta = delta * -1;
    }
    u32 price = (delta >> 2) * PRICE_PER_4_EV;
    evs_menu_state->curr_price = price;
    evs_menu_state->curr_price_is_neg = isNeg;
    if(gfx_upd){
        fmt_money(price, isNeg);
        rboxid_clean (1, true);
        rboxid_print (1, 3, 1, 1, &text_color, 0, evs_menu_state->str_buff);
        rboxid_update(1, 3);
        rboxid_tilemap_update(1);
    }

}

#endif