#ifndef NEW_MENU_C
#define NEW_MENU_C
#include "new_menu.h"

//char_base is multiplied by 0x4000 and added to 0x06000000. 
//map_base is multiplied by 0x800 and added to 0x06000000.
//size: 0 is 256x256, 1 is 512x256
//priority: 0-3, 0 is higher
//palette: 0 (16) or 1(256)
struct BgConfig new_menu_bg_config[4] = {
    {.padding=0, .b_padding=0, .priority=0, .palette=0, .size=0, .map_base=6 , .character_base=0, .bgid=0, }, 
    {.padding=0, .b_padding=0, .priority=1, .palette=0, .size=0, .map_base=14, .character_base=1, .bgid=1, }, 
    {.padding=0, .b_padding=0, .priority=2, .palette=0, .size=0, .map_base=22, .character_base=2, .bgid=2, }, 
    {.padding=0, .b_padding=0, .priority=3, .palette=0, .size=0, .map_base=30, .character_base=3, .bgid=3, } };
struct TextboxTemplate txtboxes[];
const u16 text_pal[] = {
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(120, 120, 120),
	rgb5(0, 0, 0), rgb5(208, 208, 208), rgb5(76, 154, 38),
	rgb5(102, 194, 66), rgb5(168, 75, 76), rgb5(224, 114, 75),
	rgb5(180, 124, 41), rgb5(241, 188, 60), rgb5(255, 0, 255),
    rgb5(255, 0, 255), rgb5(255, 0, 255), rgb5(255, 133, 200), rgb5(64, 200, 248)};
void do_nothing();
void exit();
const u8* new_menu_get_bg_pal();
const u16* new_menu_get_bg_map();
const u8* new_menu_get_bg_tiles();

const u16* bg2_get_bg_map(){
    return __bg2Map;
}
const u8* bg2_get_bg_tiles(){
    return __bg2Tiles;
}
const u8* bg2_get_bg_pal(){
    return __bg2Pal;
}

const u16* bg0_get_bg_map(){
    return __bg0Map;
}
const u8* bg0_get_bg_tiles(){
    return __bg0Tiles;
}
const u8* bg0_get_bg_pal(){
    return __bg0Pal;
}
//first index is stat index, second index is 0=x, 1=y
s16 cursor_positions[6][2] = {
    {100,48 },
    {100,62 },
    {100,76 },
    {100,90 },
    {100,104},
    {100,118},
};

void upd_cursor_pos(){
    objects[evs_menu_state->cursor_oam_id].pos1.x = cursor_positions[evs_menu_state->curr_stat_pos][0];
    objects[evs_menu_state->cursor_oam_id].pos1.y = cursor_positions[evs_menu_state->curr_stat_pos][1];
}
//Total EVs of a pkmn cant be more than 510. Returns true if more EVs can be added, false otherwise
bool check_total_ev(){
    u32 sum = 0;
    for(u8 i=0; i<6; i++){
        sum += evs_menu_state->curr_evs[i];
    }
    return sum < 508;
}
void on_up(){
    if(evs_menu_state->curr_stat_pos != 0){
        evs_menu_state->curr_stat_pos--;
    }else{
        evs_menu_state->curr_stat_pos=5;
    }
    upd_cursor_pos();
    audio_play(SOUND_GENERIC_CLINK);
}
void on_down(){
    if(evs_menu_state->curr_stat_pos != 5){
        evs_menu_state->curr_stat_pos++;
    }else{
        evs_menu_state->curr_stat_pos=0;
    }
    upd_cursor_pos();
    audio_play(SOUND_GENERIC_CLINK);
}


void on_left(){
    u32 curr_set_ev = evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos];
    if(curr_set_ev == 0){
        audio_play(SOUND_CANT_OPEN_HELP_MENU);
        return;
    }
    evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos]-=4;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][0]].pos1.x--;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][1]].pos1.x--;
    audio_play(SOUND_GENERIC_CLINK);
    calc_price(true);
}

void on_right(){
    u32 curr_set_ev = evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos];
    //if price is negative (the player has credit) money is always enough, otherwise project price and compare to player money
    bool money_is_enough = evs_menu_state->curr_price_is_neg ? true : get_player_money() >= evs_menu_state->curr_price + PRICE_PER_4_EV; 
    if(curr_set_ev >= 252 || !money_is_enough || !check_total_ev()){
        audio_play(SOUND_CANT_OPEN_HELP_MENU);
        return;
    }
    
    evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos]+=4;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][0]].pos1.x++;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][1]].pos1.x++;
    audio_play(SOUND_GENERIC_CLINK);
    calc_price(true);
}

void on_a(){
    //calc price. Do not allow the player to leave with negative price.
    u32 player_money = get_player_money();
    if(!evs_menu_state->curr_price_is_neg || evs_menu_state->curr_price > player_money){
        audio_play(SOUND_CANT_OPEN_HELP_MENU);
        return;
    }

    if(evs_menu_state->curr_price_is_neg){
        player_money += evs_menu_state->curr_price;
    } else {
        player_money -= evs_menu_state->curr_price;
    }
    set_player_money(player_money);

    audio_play(SOUND_GENERIC_CLINK);
    pokemon_setattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_HP_EV   , &evs_menu_state->curr_evs [0]);
    pokemon_setattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_ATK_EV  , &evs_menu_state->curr_evs [1]);
    pokemon_setattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_DEF_EV  , &evs_menu_state->curr_evs [2]);
    pokemon_setattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPATK_EV, &evs_menu_state->curr_evs [3]);
    pokemon_setattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPDEF_EV, &evs_menu_state->curr_evs [4]);
    pokemon_setattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPD_EV  , &evs_menu_state->curr_evs [5]);
    exit();
}


#define BASE_1_X 111
#define BASE_2_X BASE_1_X + 32 


void on_l(){
    evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos] = 0;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][0]].pos1.x = BASE_1_X;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][1]].pos1.x = BASE_2_X;
    audio_play(SOUND_GENERIC_CLINK);
    calc_price(true);
}

void on_r(){
    for(u8 i=0; i<63; i++){
        //if price is negative (the player has credit) money is always enough, otherwise project price and compare to player money
        bool money_is_enough = evs_menu_state->curr_price_is_neg ? true : get_player_money() >= evs_menu_state->curr_price + PRICE_PER_4_EV; 
        if(evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos] >= 252 || !money_is_enough || !check_total_ev()){
            break;
        }
        evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos]+=4;
        objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][0]].pos1.x++;
        objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][1]].pos1.x++;
        calc_price(false);
    }
    
    audio_play(SOUND_GENERIC_CLINK);
    calc_price(true);
}

extern pchar stat_names[]; //defined in main.s
extern pchar price_tag[]; //defined in main.s
extern pchar default_price[]; //defined in main.s
void on_load(){
    //init menu state
    init_evs_menu_state();

    //Show rboxes
    u32 player_money = get_player_money();
    fmt_money(player_money, false);
    rboxid_clean (1, true);
    rboxid_print (1, 3, 1, 1, &text_color, 0, evs_menu_state->str_buff);
    rboxid_update(1, 3);
    rboxid_tilemap_update(1);
    //Current price
    fmt_money(evs_menu_state->curr_price, false);
    rboxid_clean (2, true);
    rboxid_print (2, 3, 1, 1, &text_color, 0, evs_menu_state->str_buff);
    rboxid_update(2, 3);
    rboxid_tilemap_update(2);
    //pkmn name pkmn_name_buffer
    rboxid_clean (3, true);
    rboxid_print (3, 3, 1, 1, &text_color, 0, party_player[evs_menu_state->curr_selected_pkmn].base.nick);
    rboxid_update(3, 3);
    rboxid_tilemap_update(3);
    
    u32 hp_ev    = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_HP_EV   , 0) >> 2 << 2;
    u32 atk_ev   = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_ATK_EV  , 0) >> 2 << 2;
    u32 def_ev   = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_DEF_EV  , 0) >> 2 << 2;
    u32 spatk_ev = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPATK_EV, 0) >> 2 << 2;
    u32 spdef_ev = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPDEF_EV, 0) >> 2 << 2;
    u32 speed_ev = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPD_EV  , 0) >> 2 << 2;

    evs_menu_state->start_evs[0]=hp_ev;
    evs_menu_state->curr_evs [0]=hp_ev;

    evs_menu_state->start_evs[1]=atk_ev;
    evs_menu_state->curr_evs [1]=atk_ev;

    evs_menu_state->start_evs[2]=def_ev;
    evs_menu_state->curr_evs [2]=def_ev;

    evs_menu_state->start_evs[3]=spatk_ev;
    evs_menu_state->curr_evs [3]=spatk_ev;

    evs_menu_state->start_evs[4]=spdef_ev;
    evs_menu_state->curr_evs [4]=spdef_ev;

    evs_menu_state->start_evs[5]=speed_ev;
    evs_menu_state->curr_evs [5]=speed_ev;

    //Load pokemon sprite
    u32 species = pokemon_getattr(&party_player[evs_menu_state->curr_selected_pkmn], REQUEST_SPECIES, 0);
    struct SpriteTiles pkmn_sprite_tiles = pokemon_graphics_front[species];
    struct SpritePalette pkmn_sprite_pal = pokemon_palette_normal[species];
    display_compressed_sprite_compressed_pal(64, 64, 45, 90, pkmn_sprite_tiles.tag, (void *)pkmn_sprite_tiles.data, pkmn_sprite_pal.tag, (void *)pkmn_sprite_pal.data, 0, 0);	

    //Load cursor sprite
    u8 cursor_oam_id = display_compressed_sprite(16, 16, cursor_positions[0][0], cursor_positions[0][1], CURSOR_TILES_TAG, (void *)CURSOR_TILE_ADDR, CURSOR_PALS_TAG, (void *)0x08463308, 1, 0);	
    evs_menu_state->cursor_oam_id = cursor_oam_id;
    upd_cursor_pos();

    //Load EVS bar sprites
    u16 tileTag = 0xD760;
    u8 y_off = 48;
    const u8 y_delta = 14;

    for(u8 i=0; i<6; i++){
        evs_menu_state->evs_bars_oam_ids[i][0]=display_sprite(32, 16, BASE_1_X + (evs_menu_state->start_evs[i]>>2) , y_off, tileTag, (void *)s_greenblockTiles, 0xDAC0, (void *)s_greenblockPal, 0, 0, 3);
        tileTag++;
        evs_menu_state->evs_bars_oam_ids[i][1]=display_sprite(32, 16, BASE_2_X + (evs_menu_state->start_evs[i]>>2), y_off, tileTag, (void *)s_greenblockTiles, 0xDAC0, (void *)s_greenblockPal, 0, 0, 3);
        tileTag++;
        y_off+=y_delta;
    }
}


const struct InterfaceDefinition NEW_MENU_DEFINITION = {
    .gui_bg_config =new_menu_bg_config, 
    .gui_text_pal  =text_pal,
    .get_bg_0_map        =bg0_get_bg_map, 
    .get_bg_0_tilesets   =bg0_get_bg_tiles,
    .get_bg_0_pal        =bg0_get_bg_pal, 
    .get_bg_1_map        =new_menu_get_bg_map, 
    .get_bg_1_tilesets   =new_menu_get_bg_tiles,
    .get_bg_1_pal        =new_menu_get_bg_pal, 
    .get_bg_2_map        =bg2_get_bg_map, 
    .get_bg_2_tilesets   =bg2_get_bg_tiles,
    .get_bg_2_pal        =bg2_get_bg_pal, 
    .get_bg_3_map        =new_menu_get_bg_map, 
    .get_bg_3_tilesets   =new_menu_get_bg_tiles,
    .get_bg_3_pal        =new_menu_get_bg_pal, 
    .textboxes     =txtboxes,
    .on_load       =on_load,
    .on_key_a=     on_a,
    .on_key_b=     exit,
    .on_key_start= do_nothing,
    .on_key_select=on_a,
    .on_key_l=     on_l,
    .on_key_r=     on_r,
    .on_key_up=    on_up,
    .on_key_down=  on_down,
    .on_key_left=  on_left,
    .on_key_right= on_right,
};

const u16* new_menu_get_bg_map(){
    return __new_menu_guiMap;
}
const u8* new_menu_get_bg_tiles(){
    return __new_menu_guiTiles;
}
const u8* new_menu_get_bg_pal(){
    return __new_menu_guiPal;
}


void do_nothing(){
    return;
}

void exit(){
    audio_play(SOUND_GENERIC_CLINK);
    super.multi_purpose_state_tracker=0;
    set_callback1(gui_exit);
}


//larghezza salta di 8px in 8 px
// lunghezza sempre di 8px in 8 px ma è come se ci aggiungessi sempre 1 (parte da 8 px)
struct TextboxTemplate txtboxes[] = {
    {
        /*stats names */
        .bg_id = 0,
        .x = 2,
        .y = 5,
        .width = 25,
        .height = 11,
        .pal_id = 15,
        .charbase = 1,
    },
    {
        //current money 
        .bg_id = 0,
        .x = 21,
        .y = 0,
        .width = 8,
        .height = 2,
        .pal_id = 15,
        .charbase = 71,
    },
    {
        //current price 
        .bg_id = 0,
        .x = 21,
        .y = 3,
        .width = 8,
        .height = 2,
        .pal_id = 15,
        .charbase = 141,
    },
    {
        //pkmn name 
        .bg_id = 0,
        .x = 2,
        .y = 5,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 211,
    },
    {
        .bg_id = 0xFF, // marks the end of the tb array 
    },

};


#endif