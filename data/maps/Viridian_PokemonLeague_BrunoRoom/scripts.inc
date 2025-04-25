Viridian_PokemonLeague_BrunoRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Viridian_PokemonLeague_BrunoRoom_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Viridian_PokemonLeague_BrunoRoom_OnWarp
	.byte 0

Viridian_PokemonLeague_BrunoRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_BrunoRoom_EventScript_PlayerTurnNorth
	.2byte 0

Viridian_PokemonLeague_BrunoRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

Viridian_PokemonLeague_BrunoRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_BrunoRoom_EventScript_WalkInCloseDoor
	.2byte 0

Viridian_PokemonLeague_BrunoRoom_EventScript_WalkInCloseDoor::
	lockall
	call PokemonLeague_EliteFour_EventScript_WalkInCloseDoor
	setvar VAR_TEMP_1, 1
	releaseall
	end

Viridian_PokemonLeague_BrunoRoom_EventScript_Bruno::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_BRUNO, Viridian_PokemonLeague_BrunoRoom_EventScript_BrunoAlreadyBeat
	msgbox Viridian_PokemonLeague_BrunoRoom_Text_Bruno, MSGBOX_DEFAULT
	goto Viridian_PokemonLeague_BrunoRoom_EventScript_BrunoBattle
	end

Viridian_PokemonLeague_BrunoRoom_Text_Bruno:
	.string "So you've beaten Lorelei.\p"
	.string "However, I won't prove to be so easy.\p"
	.string "Come at me then!$"

Viridian_PokemonLeague_BrunoRoom_EventScript_BrunoBattle::
	trainerbattle_no_intro TRAINER_BRUNO, Viridian_PokemonLeague_BrunoRoom_Text_BrunoDefeat
	goto Viridian_PokemonLeague_BrunoRoom_EventScript_BrunoAfter
	end

Viridian_PokemonLeague_BrunoRoom_Text_BrunoDefeat:
	.string "Hmm.$"

Viridian_PokemonLeague_BrunoRoom_EventScript_BrunoAfter::
	call PokemonLeague_EliteFour_SetAdvanceToNextRoomMetatiles
	msgbox Viridian_PokemonLeague_BrunoRoom_Text_BrunoAfter, MSGBOX_DEFAULT
	setflag FLAG_BEAT_BRUNO
	release
	end

Viridian_PokemonLeague_BrunoRoom_Text_BrunoAfter:
	.string "Alright, you got me.\p"
	.string "Next up is Agatha.\p"
	.string "I warn you, that old coot is no easy\n"
	.string "opponent just because of her age.$"

Viridian_PokemonLeague_BrunoRoom_EventScript_BrunoAlreadyBeat::
	msgbox Viridian_PokemonLeague_BrunoRoom_Text_BrunoAfter, MSGBOX_DEFAULT
	release
	end

