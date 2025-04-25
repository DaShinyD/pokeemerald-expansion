.set LOCALID_NURSE, 1
.set LOCALID_SHOP, 2
.set LOCALID_GUARD, 3
.set LOCALID_GUARD_2, 4

Viridian_PokemonLeague_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Viridian_PokemonLeague_OnTransition
	.byte 0

Viridian_PokemonLeague_OnTransition:
	setrespawn HEAL_LOCATION_VIRIDIAN_CITY
	end

Viridian_PokemonLeague_EventScript_Nurse::
	lock
	msgbox Viridian_PokemonLeague_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox Viridian_PokemonLeague_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, Viridian_PokemonLeague_Movement_Turn
	release
	end

Viridian_PokemonLeague_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

Viridian_PokemonLeague_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

Viridian_PokemonLeague_Movement_Turn:
	face_down
	step_end

Viridian_PokemonLeague_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart Viridian_PokemonLeague_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
Viridian_PokemonLeague_Pokemart:
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_MAX_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_REVIVE
	.2byte ITEM_MAX_REPEL
	.2byte ITEM_MAX_ELIXIR
	.2byte ITEM_MAX_ETHER
	.2byte ITEM_PP_MAX
	.2byte ITEM_NONE
	release
	end

Viridian_PokemonLeague_EventScript_BadgeCheck::
	lock
	applymovement LOCALID_GUARD, Viridian_PokemonLeague_Movement_LookAtPlayer
	waitmovement 0
	goto_if_set FLAG_INDIGO_BEAT, Viridian_PokemonLeague_EventScript_Nope
	msgbox Viridian_PokemonLeague_Text_LemmeCheck, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GUARD, Viridian_PokemonLeague_Movement_LookBack
	waitmovement 0
	removeitem ITEM_BOULDER_BADGE
	removeitem ITEM_CASCADE_BADGE
	removeitem ITEM_THUNDER_BADGE
	removeitem ITEM_RAINBOW_BADGE
	removeitem ITEM_SOUL_BADGE
	removeitem ITEM_MARSH_BADGE
	removeitem ITEM_VOLCANO_BADGE
	removeitem ITEM_EARTH_BADGE
	release
	end

Viridian_PokemonLeague_Movement_LookAtPlayer:
	face_right
	step_end

Viridian_PokemonLeague_Text_LemmeCheck:
	.string "Hold on, let me check your badges.\p"
	.string "...\p"
	.string "Alright, you have all eight.\p"
	.string "However, these are colorless, so\n"
	.string "you may not earn the title.\p"
	.string "Be warned, a new rule change has\n"
	.string "been implemented.\p"
	.string "Due to a back and forth between\n"
	.string "two people, our champion is\l"
	.string "actually champions.\p"
	.string "With an s.\p"
	.string "It will be a double battle.\p"
	.string "Best of luck.$"

Viridian_PokemonLeague_Movement_LookBack:
	face_down
	step_end

Viridian_PokemonLeague_EventScript_Nope::
	msgbox Viridian_PokemonLeague_Text_AlreadyBeat, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	release
	end

Viridian_PokemonLeague_Text_AlreadyBeat:
	.string "You've already won your challenge.\p"
	.string "You can only come back if you\n"
	.string "properly take on the gym challenge.$"

