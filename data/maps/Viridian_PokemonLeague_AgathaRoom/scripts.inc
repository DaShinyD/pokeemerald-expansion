Viridian_PokemonLeague_AgathaRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Viridian_PokemonLeague_AgathaRoom_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Viridian_PokemonLeague_AgathaRoom_OnWarp
	.byte 0

Viridian_PokemonLeague_AgathaRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_AgathaRoom_EventScript_PlayerTurnNorth
	.2byte 0

Viridian_PokemonLeague_AgathaRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

Viridian_PokemonLeague_AgathaRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_AgathaRoom_EventScript_WalkInCloseDoor
	.2byte 0

Viridian_PokemonLeague_AgathaRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_TEMP_1, 1
	releaseall
	end

Viridian_PokemonLeague_AgathaRoom_EventScript_Agatha::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_AGATHA, Viridian_PokemonLeague_AgathaRoom_EventScript_AgathaAlreadyBeat
	msgbox Viridian_PokemonLeague_AgathaRoom_Text_Agatha, MSGBOX_DEFAULT
	goto Viridian_PokemonLeague_AgathaRoom_EventScript_AgathaBattle
	end

Viridian_PokemonLeague_AgathaRoom_Text_Agatha:
	.string "Keh heh heh.\p"
	.string "It's been a while since we have had\n"
	.string "a real challenger.\p"
	.string "Everyone is too afraid because of them.\p"
	.string "Whatever, that is that and this is this.\p"
	.string "See if you can take me.$"

Viridian_PokemonLeague_AgathaRoom_EventScript_AgathaBattle::
	trainerbattle_no_intro TRAINER_AGATHA, Viridian_PokemonLeague_AgathaRoom_Text_AgathaDefeat
	goto Viridian_PokemonLeague_AgathaRoom_EventScript_AgathaAfter
	end

Viridian_PokemonLeague_AgathaRoom_Text_AgathaDefeat:
	.string "Thrilling.$"

Viridian_PokemonLeague_AgathaRoom_EventScript_AgathaAfter::
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox Viridian_PokemonLeague_AgathaRoom_Text_AgathaAfter, MSGBOX_DEFAULT
	setflag FLAG_BEAT_AGATHA
	release
	end

Viridian_PokemonLeague_AgathaRoom_Text_AgathaAfter:
	.string "Maybe it's time I resign soon.\p"
	.string "The children these days are much\n"
	.string "too strong it seems.\p"
	.string "Get out of here.$"

Viridian_PokemonLeague_AgathaRoom_EventScript_AgathaAlreadyBeat::
	msgbox Viridian_PokemonLeague_AgathaRoom_Text_AgathaAfter, MSGBOX_DEFAULT
	release
	end

