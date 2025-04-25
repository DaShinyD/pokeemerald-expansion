Viridian_PokemonLeague_LoreleiRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Viridian_PokemonLeague_LoreleiRoom_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Viridian_PokemonLeague_LoreleiRoom_OnWarp
	.byte 0

Viridian_PokemonLeague_LoreleiRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_LoreleiRoom_EventScript_PlayerTurnNorth
	.2byte 0

Viridian_PokemonLeague_LoreleiRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

Viridian_PokemonLeague_LoreleiRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_LoreleiRoom_EventScript_WalkInCloseDoor
	.2byte 0

Viridian_PokemonLeague_LoreleiRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_TEMP_1, 1
	releaseall
	end

Viridian_PokemonLeague_LoreleiRoom_EventScript_Lorelei::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_LORELEI, Viridian_PokemonLeague_LoreleiRoom_EventScript_LoreleiAlreadyBeat
	msgbox Viridian_PokemonLeague_LoreleiRoom_Text_Lorelei, MSGBOX_DEFAULT
	goto Viridian_PokemonLeague_LoreleiRoom_EventScript_LoreleiBattle
	end

Viridian_PokemonLeague_LoreleiRoom_Text_Lorelei:
	.string "Ah, I've heard of you.\p"
	.string "The new Hoenn champion.\p"
	.string "I'm Lorelei, the icy beauty.\p"
	.string "Seeing as your here, you must want to\n"
	.string "see how are League is.\p"
	.string "Let me show you, but be warned, it'll\n"
	.string "be a chilly reception!$"

Viridian_PokemonLeague_LoreleiRoom_EventScript_LoreleiBattle::
	trainerbattle_no_intro TRAINER_LORELEI, Viridian_PokemonLeague_LoreleiRoom_Text_LoreleiDefeat
	goto Viridian_PokemonLeague_LoreleiRoom_EventScript_LoreleiAfter
	end

Viridian_PokemonLeague_LoreleiRoom_Text_LoreleiDefeat:
	.string "Frozen solid.$"

Viridian_PokemonLeague_LoreleiRoom_EventScript_LoreleiAfter::
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox Viridian_PokemonLeague_LoreleiRoom_Text_LoreleiAfter, MSGBOX_DEFAULT
	setflag FLAG_BEAT_LORELEI
	release
	end

Viridian_PokemonLeague_LoreleiRoom_Text_LoreleiAfter:
	.string "I may have fallen, but the monsters\n"
	.string "at the end will definitely stop you.\p"
	.string "Go on.$"

Viridian_PokemonLeague_LoreleiRoom_EventScript_LoreleiAlreadyBeat::
	msgbox Viridian_PokemonLeague_LoreleiRoom_Text_LoreleiAfter, MSGBOX_DEFAULT
	release
	end

