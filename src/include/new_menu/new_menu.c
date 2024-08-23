#ifndef NEW_MENU_C
#define NEW_MENU_C
#include "new_menu.h"
struct BgConfig new_menu_bg_config[4];
struct TextboxTemplate txtboxes[];
const u16 text_pal[];
void do_nothing();
void exit();
const u8* new_menu_get_bg_pal();
const u16* new_menu_get_bg_map();
const u8* new_menu_get_bg_tiles();


const u16* bg2_get_bg_map(){
    return bg2Map;
}
const u8* bg2_get_bg_tiles(){
    return bg2Tiles;
}
const u8* bg2_get_bg_pal(){
    return bg2Pal;
}


const struct InterfaceDefinition NEW_MENU_DEFINITION = {
    .gui_bg_config =new_menu_bg_config, 
    .gui_text_pal  =text_pal,
    .get_bg_0_map        =new_menu_get_bg_map, 
    .get_bg_0_tilesets   =new_menu_get_bg_tiles,
    .get_bg_0_pal        =new_menu_get_bg_pal, 
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
    .on_load=      do_nothing,
    .on_key_a=     do_nothing,
    .on_key_b=     exit,
    .on_key_start= do_nothing,
    .on_key_select=do_nothing,
    .on_key_l=     do_nothing,
    .on_key_r=     do_nothing,
    .on_key_up=    do_nothing,
    .on_key_down=  do_nothing,
    .on_key_left=  do_nothing,
    .on_key_right= do_nothing,
};

const u16* new_menu_get_bg_map(){
    return new_menu_guiMap;
}
const u8* new_menu_get_bg_tiles(){
    return new_menu_guiTiles;
}
const u8* new_menu_get_bg_pal(){
    return new_menu_guiPal;
}

void do_nothing(){
    return;
}

void exit(){
    audio_play(SOUND_GENERIC_CLINK);
    super.multi_purpose_state_tracker=0;
    set_callback1(gui_exit);
}

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
        //"Price:" 
        .bg_id = 0,
        .x = 17,
        .y = 0,
        .width = 4,
        .height = 2,
        .pal_id = 15,
        .charbase = 71,
    },
    {
        //current price 
        .bg_id = 0,
        .x = 22,
        .y = 0,
        .width = 7,
        .height = 2,
        .pal_id = 15,
        .charbase = 141,
    },
    {
        //pkmn name 
        .bg_id = 0,
        .x = 19,
        .y = 3,
        .width = 9,
        .height = 2,
        .pal_id = 15,
        .charbase = 211,
    },
    {
        .bg_id = 0xFF, // marks the end of the tb array 
    },

};

const u16 text_pal[] = {
	rgb5(255, 0, 255), rgb5(248, 248, 248), rgb5(112, 112, 112),
	rgb5(0, 0, 0), rgb5(208, 208, 208), rgb5(76, 154, 38),
	rgb5(102, 194, 66), rgb5(168, 75, 76), rgb5(224, 114, 75),
	rgb5(180, 124, 41), rgb5(241, 188, 60), rgb5(255, 0, 255),
    rgb5(255, 0, 255), rgb5(255, 0, 255), rgb5(255, 133, 200), rgb5(64, 200, 248)};

#endif