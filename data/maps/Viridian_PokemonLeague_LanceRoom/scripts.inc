Viridian_PokemonLeague_LanceRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Viridian_PokemonLeague_LanceRoom_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Viridian_PokemonLeague_LanceRoom_OnWarp
	.byte 0

Viridian_PokemonLeague_LanceRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_LanceRoom_EventScript_PlayerTurnNorth
	.2byte 0

Viridian_PokemonLeague_LanceRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

Viridian_PokemonLeague_LanceRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_LanceRoom_EventScript_WalkInCloseDoor
	.2byte 0

Viridian_PokemonLeague_LanceRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_TEMP_1, 1
	releaseall
	end

Viridian_PokemonLeague_LanceRoom_EventScript_Lance::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_LANCE, Viridian_PokemonLeague_LanceRoom_EventScript_LanceAlreadyBeat
	msgbox Viridian_PokemonLeague_LanceRoom_Text_Lance, MSGBOX_DEFAULT
	goto Viridian_PokemonLeague_LanceRoom_EventScript_LanceBattle
	end

Viridian_PokemonLeague_LanceRoom_Text_Lance:
	.string "Hahaha.\p"
	.string "I am Lance, the dragon master.\p"
	.string "I once commanded both the Kanto and\n"
	.string "the Johto Leagues.\p"
	.string "Now I merely serve as a guard to the\n"
	.string "sort of power rarely seen.\p"
	.string "Show me yours.$"

Viridian_PokemonLeague_LanceRoom_EventScript_LanceBattle::
	trainerbattle_no_intro TRAINER_LANCE, Viridian_PokemonLeague_LanceRoom_Text_LanceDefeat
	goto Viridian_PokemonLeague_LanceRoom_EventScript_LanceAfter
	end

Viridian_PokemonLeague_LanceRoom_Text_LanceDefeat:
	.string "Haaah.$"

Viridian_PokemonLeague_LanceRoom_EventScript_LanceAfter::
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox Viridian_PokemonLeague_LanceRoom_Text_LanceAfter, MSGBOX_DEFAULT
	setflag FLAG_BEAT_LANCE
	release
	end

Viridian_PokemonLeague_LanceRoom_Text_LanceAfter:
	.string "Go.\p"
	.string "I have nothing more to say.\p"
	.string "You must experience what is ahead\n"
	.string "for yourself.$"

Viridian_PokemonLeague_LanceRoom_EventScript_LanceAlreadyBeat::
	msgbox Viridian_PokemonLeague_LanceRoom_Text_LanceAfter, MSGBOX_DEFAULT
	release
	end

