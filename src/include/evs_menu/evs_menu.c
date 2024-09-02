#ifndef NEW_MENU_C
#define NEW_MENU_C
#include "evs_menu.h"

//char_base is multiplied by 0x4000 and added to 0x06000000. 
//map_base is multiplied by 0x800 and added to 0x06000000.
//size: 0 is 256x256, 1 is 512x256
//priority: 0-3, 0 is higher
//palette: 0 (16) or 1(256)
void evs_ivs_menu_entrypoint(){ new_gui_main(IMPLEMENTED_INTERFACES[0]);}

struct BgConfig new_menu_bg_config[4] = {
    {.padding=0, .b_padding=0, .priority=0, .palette=0, .size=0, .map_base=6 , .character_base=0, .bgid=0, }, 
    {.padding=0, .b_padding=0, .priority=1, .palette=0, .size=0, .map_base=14, .character_base=1, .bgid=1, }, 
    {.padding=0, .b_padding=0, .priority=2, .palette=0, .size=0, .map_base=22, .character_base=2, .bgid=2, }, 
    {.padding=0, .b_padding=0, .priority=3, .palette=0, .size=0, .map_base=30, .character_base=3, .bgid=3, } };
struct TextboxTemplate txtboxes[];
const u16 text_pal[] = {
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(66, 66, 66),
	rgb5(222, 222, 198), rgb5(208, 208, 208), rgb5(76, 154, 38),
	rgb5(206, 66, 0), rgb5(168, 75, 76), rgb5(224, 114, 75),
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
s16 evs_cursor_positions[6][2] = {
    {100,42 },
    {100,56 },
    {100,70 },
    {100,84 },
    {100,98 },
    {100,112},
};
//first index is stat index, second index is 0=x, 1=y
s16 confirm_cursor_positions[2][2] = {
    {63 ,141},
    {136,141},
};

void upd_cursor_pos(){
    if(evs_menu_state->curr_mode == EVS){
        objects[evs_menu_state->cursor_oam_id].pos1.x = evs_cursor_positions[evs_menu_state->curr_stat_pos][0];
        objects[evs_menu_state->cursor_oam_id].pos1.y = evs_cursor_positions[evs_menu_state->curr_stat_pos][1];
    } else {
        objects[evs_menu_state->cursor_oam_id].pos1.x = confirm_cursor_positions[evs_menu_state->curr_stat_pos][0];
        objects[evs_menu_state->cursor_oam_id].pos1.y = confirm_cursor_positions[evs_menu_state->curr_stat_pos][1];
    }
}

extern pchar instructions[]; //defined in main.s
void display_instructions(){
    rboxid_clean (2, true);
    rboxid_print (2, 3, 1, 1, &text_color, 0, instructions);
}

extern pchar confirm_str[]; //defined in main.s
void display_confirm_str(){
    rboxid_clean (2, true);
    rboxid_print (2, 3, 58, 6, &text_color, 0, confirm_str);
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
    if(evs_menu_state->curr_mode != EVS){
        return;
    }
    if(evs_menu_state->curr_stat_pos != 0){
        evs_menu_state->curr_stat_pos--;
    }else{
        evs_menu_state->curr_stat_pos=5;
    }
    upd_cursor_pos();
    audio_play(SOUND_GENERIC_CLINK);
}

void on_down(){
    if(evs_menu_state->curr_mode != EVS){
        return;
    }
    if(evs_menu_state->curr_stat_pos != 5){
        evs_menu_state->curr_stat_pos++;
    }else{
        evs_menu_state->curr_stat_pos=0;
    }
    upd_cursor_pos();
    audio_play(SOUND_GENERIC_CLINK);
}


void on_left(){
    if(evs_menu_state->curr_mode == EVS){
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
    } else {
        if (evs_menu_state->curr_stat_pos == 0){
            evs_menu_state->curr_stat_pos = 1;
        } else {
            evs_menu_state->curr_stat_pos = 0;
        }
        upd_cursor_pos();
        audio_play(SOUND_GENERIC_CLINK);
    }
}

void on_right(){
    if(evs_menu_state->curr_mode == EVS){
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
    } else {
        if (evs_menu_state->curr_stat_pos == 0){
            evs_menu_state->curr_stat_pos = 1;
        } else {
            evs_menu_state->curr_stat_pos = 0;
        }
        upd_cursor_pos();
        audio_play(SOUND_GENERIC_CLINK);
    }
}

void exit_unsaved(){
    audio_play(SOUND_GENERIC_CLINK);
    super.multi_purpose_state_tracker=0;
    set_callback1(gui_exit);
}

void back_from_confirm(){
    audio_play(SOUND_GENERIC_CLINK);
    evs_menu_state->curr_mode = EVS;
    evs_menu_state->curr_stat_pos = 0;
    upd_cursor_pos();
    display_instructions();
}

void on_a(){
    if(evs_menu_state->curr_mode == CONFIRM){
        if(evs_menu_state->curr_stat_pos == 0){
            exit();
        } else {
            back_from_confirm();
        }
    } else {
        audio_play(SOUND_GENERIC_CLINK);
        evs_menu_state->curr_mode = CONFIRM;
        evs_menu_state->curr_stat_pos = 0;
        display_confirm_str();
        upd_cursor_pos();
    }
}

void on_b(){
    if(evs_menu_state->curr_mode == CONFIRM){
        back_from_confirm();
    } else {
        exit_unsaved();
    }
}

#define BASE_1_X 111
#define BASE_2_X BASE_1_X + 32 


void on_l(){
    if(evs_menu_state->curr_mode != EVS){
        return;
    }
    evs_menu_state->curr_evs[evs_menu_state->curr_stat_pos] = 0;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][0]].pos1.x = BASE_1_X;
    objects[evs_menu_state->evs_bars_oam_ids[evs_menu_state->curr_stat_pos][1]].pos1.x = BASE_2_X;
    audio_play(SOUND_GENERIC_CLINK);
    calc_price(true);
}

void on_r(){
    if(evs_menu_state->curr_mode != EVS){
        return;
    }
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

void on_load(){
    //init menu state
    init_evs_menu_state();

    evs_menu_state->curr_selected_pkmn = var_8004;

    //Show rboxes
    u32 player_money = get_player_money();
    fmt_money(player_money, false);

    //remove end-of-string terminator: we will concat player money to newline and then price
    for(u8 i=0; i<sizeof(evs_menu_state->str_buff); i++){
        if(evs_menu_state->str_buff[i]==0xFF){
            evs_menu_state->str_buff[i]=0;
            break;
        }
    }
    memcpy(evs_menu_state->player_money_str_buff, evs_menu_state->str_buff, sizeof(evs_menu_state->str_buff));

    memcpy(evs_menu_state->concat_str_buff, evs_menu_state->player_money_str_buff, 10);
    evs_menu_state->concat_str_buff[10] = 0xFE; // \n
    fmt_money(evs_menu_state->curr_price, false);
    memcpy(&(evs_menu_state->concat_str_buff[11]), evs_menu_state->str_buff, 10);

    rboxid_clean (0, true);
    rboxid_print (0, 3, 1, 2, &text_color, 0, evs_menu_state->concat_str_buff);

    u8 *buffer = malloc(11);
    memcpy(buffer, party_player[evs_menu_state->curr_selected_pkmn].base.nick, 10);
    buffer[11] = 0xFF;
    //pkmn name pkmn_name_buffer
    rboxid_clean (1, true);
    rboxid_print (1, 3, 3, 0, &text_color, 0, buffer);
    rboxid_update(1, 3);
    rboxid_tilemap_update(1);
    free(buffer);
    //instructions
    display_instructions();
    
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
    display_compressed_sprite_compressed_pal(64, 64, 49, 85, pkmn_sprite_tiles.tag, (void *)pkmn_sprite_tiles.data, pkmn_sprite_pal.tag, (void *)pkmn_sprite_pal.data, 0, 0);	

    //Load cursor sprite
    u8 cursor_oam_id = display_compressed_sprite(16, 16, evs_cursor_positions[0][0], evs_cursor_positions[0][1], CURSOR_TILES_TAG, (void *)CURSOR_TILE_ADDR, CURSOR_PALS_TAG, (void *)0x08463308, 1, 0);	
    evs_menu_state->cursor_oam_id = cursor_oam_id;
    upd_cursor_pos();

    //Load EVS bar sprites
    u16 tileTag = 0xD760;
    u8 y_off = 42;
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
    .on_key_b=     on_b,
    .on_key_start= do_nothing,
    .on_key_select=do_nothing,
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
    //calc price. If price is negative give player the difference in money
    u32 player_money = get_player_money();
    if(!evs_menu_state->curr_price_is_neg && evs_menu_state->curr_price > player_money){ //if price is positive and is higher than player money, don't allow exit
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
    exit_unsaved();
}


//larghezza salta di 8px in 8 px
// lunghezza sempre di 8px in 8 px ma è come se ci aggiungessi sempre 1 (parte da 8 px)
//charbase is multiplied byu 0x20 and added to 0x06000000
struct TextboxTemplate txtboxes[] = {
    {
        //current money and price
        .bg_id = 0,
        .x = 21,
        .y = 0,
        .width = 8,
        .height = 4,
        .pal_id = 15,
        .charbase = 1,
    },
    {
        //pkmn name 
        .bg_id = 0,
        .x = 2,
        .y = 4,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 40,
    },
    {
        //instructions 
        .bg_id = 0,
        .x = 1,
        .y = 16,
        .width = 28,
        .height = 4,
        .pal_id = 15,
        .charbase = 70,
    },
    {
        .bg_id = 0xFF, // marks the end of the tb array 
    },

};


#endif