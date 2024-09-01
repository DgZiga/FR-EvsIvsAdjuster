#ifndef CONFIG_H
#define CONFIG_H

#define EVS_MENU_STATE_START (&(party_opponent[0]).base.otname); //By default this is the opponent's party
#include "include/evs_menu/evs_menu.h"

#define OAM_TABLE_START 0x83A3BB0 		//Default BPRE
#define OAM_TABLE_SIZE 0x99 			//Default BPRE
#define OAM_PAL_TABLE_START 0x83A5158 	//Default BPRE
#define OAM_PAL_TABLE_SIZE 18 			//Default BPRE

const struct InterfaceDefinition* const IMPLEMENTED_INTERFACES[] = {&NEW_MENU_DEFINITION};

#define TOOL_COUNT sizeof(IMPLEMENTED_INTERFACES)/sizeof(IMPLEMENTED_INTERFACES[0])

#include "engine/engine_main.h"

#endif