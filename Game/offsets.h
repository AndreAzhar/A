/*

	Game offsets, top ones need to be updated every
	update generally, and for big updates other ones
	may need to be changed too.

*/
#pragma once

// RainbowSix.exe ->
#define address_game_manager  0x53A3168
#define address_v_table 0x3a73350

// RainbowSix.exe -> game_manager -> 
#define offset_entity_count 0xD8
#define offset_entity_list 0xC8

// RainbowSix.exe -> game_manager -> entity_list ->
#define offset_entity_index 0x8
#define ADDRESS_FREEZEMANAGER 0x538c590 //48 8B 05 ? ? ? ? 0F B6 48 61 // 0x5c





