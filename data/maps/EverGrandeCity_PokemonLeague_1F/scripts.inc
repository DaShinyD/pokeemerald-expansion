.set LOCALID_NURSE, 1
.set LOCALID_GUARD_1, 3
.set LOCALID_GUARD_2, 4
.set LOCALID_DASH, 5
.set LOCALID_RYE, 6

EverGrandeCity_PokemonLeague_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_PokemonLeague_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

EverGrandeCity_PokemonLeague_1F_OnTransition:
	setrespawn HEAL_LOCATION_EVER_GRANDE_CITY_POKEMON_LEAGUE
	setflag FLAG_LANDMARK_POKEMON_LEAGUE
	call_if_unset FLAG_ENTERED_ELITE_FOUR, EverGrandeCity_PokemonLeague_1F_EventScript_GuardsBlockDoor
	end

EverGrandeCity_PokemonLeague_1F_EventScript_GuardsBlockDoor::
	setobjectxyperm LOCALID_GUARD_1, 9, 2
	setobjectxyperm LOCALID_GUARD_2, 10, 2
	return

EverGrandeCity_PokemonLeague_1F_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

EverGrandeCity_PokemonLeague_1F_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart EverGrandeCity_PokemonLeague_1F_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
EverGrandeCity_PokemonLeague_1F_Pokemart:
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

@ The door guards only check for FLAG_BADGE06_GET because Winonas badge is the only one that can be skipped
@ Its assumed the player has the remaining badges
EverGrandeCity_PokemonLeague_1F_EventScript_DoorGuard::
	lockall
	goto_if_set FLAG_ENTERED_ELITE_FOUR, EverGrandeCity_PokemonLeague_1F_EventScript_GoForth
	goto_if_unset FLAG_LEAGUE_RYE, EverGrandeCity_PokemonLeague_1F_EventScript_FightRivals
	goto_if_unset FLAG_LEAGUE_DASH, EverGrandeCity_PokemonLeague_1F_EventScript_FightRivals
	getplayerxy VAR_TEMP_0, VAR_TEMP_1
	call_if_ge VAR_TEMP_0, 11, EverGrandeCity_PokemonLeague_1F_EventScript_PlayerMoveToFrontFromRight
	call_if_le VAR_TEMP_0, 8, EverGrandeCity_PokemonLeague_1F_EventScript_PlayerMoveToFrontFromLeft
	message EverGrandeCity_PokemonLeague_1F_Text_MustHaveAllGymBadges
	waitmessage
	delay 120
	goto_if_unset FLAG_BADGE06_GET, EverGrandeCity_PokemonLeague_1F_EventScript_NotAllBadges
	closemessage
	applymovement LOCALID_GUARD_1, EverGrandeCity_PokemonLeague_1F_Movement_LeftGuardOutOfWay
	applymovement LOCALID_GUARD_2, EverGrandeCity_PokemonLeague_1F_Movement_RightGuardOutOfWay
	waitmovement 0
	delay 10
	playfanfare MUS_OBTAIN_BADGE
	message EverGrandeCity_PokemonLeague_1F_Text_GoForth
	waitmessage
	waitfanfare
	closemessage
	copyobjectxytoperm LOCALID_GUARD_1
	copyobjectxytoperm LOCALID_GUARD_2
	setflag FLAG_ENTERED_ELITE_FOUR
	releaseall
	end

EverGrandeCity_PokemonLeague_1F_EventScript_PlayerMoveToFrontFromRight::
	applymovement OBJ_EVENT_ID_PLAYER, EverGrandeCity_PokemonLeague_1F_Movement_MoveToFrontFromRight
	waitmovement 0
	return

EverGrandeCity_PokemonLeague_1F_EventScript_PlayerMoveToFrontFromLeft::
	applymovement OBJ_EVENT_ID_PLAYER, EverGrandeCity_PokemonLeague_1F_Movement_MoveToFrontFromLeft
	waitmovement 0
	return

EverGrandeCity_PokemonLeague_1F_EventScript_NotAllBadges::
	playse SE_FAILURE
	msgbox EverGrandeCity_PokemonLeague_1F_Text_HaventObtainedAllBadges, MSGBOX_DEFAULT
	releaseall
	end

EverGrandeCity_PokemonLeague_1F_EventScript_GoForth::
	applymovement VAR_LAST_TALKED, Common_Movement_FacePlayer
	waitmovement 0
	msgbox EverGrandeCity_PokemonLeague_1F_Text_GoForth, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

EverGrandeCity_PokemonLeague_1F_Movement_MoveToFrontFromRight:
	walk_down
	walk_left
	walk_in_place_faster_up
	step_end

EverGrandeCity_PokemonLeague_1F_Movement_MoveToFrontFromLeft:
	walk_down
	walk_right
	walk_in_place_faster_up
	step_end

EverGrandeCity_PokemonLeague_1F_Movement_LeftGuardOutOfWay:
	walk_left
	walk_in_place_faster_down
	step_end

EverGrandeCity_PokemonLeague_1F_Movement_RightGuardOutOfWay:
	walk_right
	walk_in_place_faster_down
	step_end

EverGrandeCity_PokemonLeague_1F_Text_MustHaveAllGymBadges:
	.string "Beyond this point, only those TRAINERS\n"
	.string "who have collected all the GYM BADGES\l"
	.string "are permitted to enter.\p"
	.string "TRAINER, let us confirm that you have\n"
	.string "all the GYM BADGES.$"

EverGrandeCity_PokemonLeague_1F_Text_HaventObtainedAllBadges:
	.string "You haven't obtained all the BADGES.\p"
	.string "If you're bound for the POKéMON\n"
	.string "LEAGUE, you must return with them.$"

EverGrandeCity_PokemonLeague_1F_Text_GoForth:
	.string "TRAINER! Believe in yourself and your\n"
	.string "POKéMON, and go forth!$"




EverGrandeCity_PokemonLeague_1F_EventScript_RivalsFight::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, EverGrandeCity_PokemonLeague_1F_MovementApproachRivals
	waitmovement 0
	msgbox EverGrandeCity_PokemonLeague_1F_Text_HeyPlayer, MSGBOX_DEFAULT
	setvar VAR_LEAGUE_RIVALS, 1
	release
	end

EverGrandeCity_PokemonLeague_1F_MovementApproachRivals:
	walk_up
	walk_up
	walk_up
	step_end

EverGrandeCity_PokemonLeague_1F_Text_HeyPlayer:
	.string "Dash: You made it!\p"
	.string "We're sorry about disappearing\n"
	.string "after Sootopolis.\p"
	.string "After you solved the crisis, we\n"
	.string "felt dissatisfied with our power.\p"
	.string "You handled Team Aqua by yourself\n"
	.string "and even got Rayquaza.\p"
	.string "So we went to train.\n"
	.string "That led us here.\p"
	.string "Now, we finally feel like we can\n"
	.string "help you if anything happens.\p"
	.string "Rye and I want to test our new\n"
	.string "strength here.\p"
	.string "Rye: That's right.\p"
	.string "And before we go to the Elite 4,\n"
	.string "we want to battle you.\p"
	.string "Don't worry, we'll let you go heal.\n"
	.string "So get ready and come talk to us.\p"
	.string "We already got permission from\n"
	.string "the staff.\p"
	.string "We'll wait here.$"

EverGrandeCity_PokemonLeague_1F_EventScript_Dash::
	lock
	faceplayer
	msgbox EverGrandeCity_PokemonLeague_1F_Text_DashBeforeBattle, MSGBOX_DEFAULT
	goto EverGrandeCity_PokemonLeague_1F_EventScript_DashBattle
	end

EverGrandeCity_PokemonLeague_1F_Text_DashBeforeBattle:
	.string "Let's do this, {PLAYER}.$"

EverGrandeCity_PokemonLeague_1F_EventScript_DashBattle::
	trainerbattle_no_intro TRAINER_DASH_EG, EverGrandeCity_PokemonLeague_1F_Text_DashDefeat
	goto EverGrandeCity_PokemonLeague_1F_EventScript_DashLeave
	end

EverGrandeCity_PokemonLeague_1F_Text_DashDefeat:
	.string "As strong as ever.$"

EverGrandeCity_PokemonLeague_1F_EventScript_DashLeave::
	msgbox EverGrandeCity_PokemonLeague_1F_Text_DashNextTime, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_DASH
	setflag FLAG_LEAGUE_DASH
	fadescreen FADE_FROM_BLACK
	release
	end

EverGrandeCity_PokemonLeague_1F_Text_DashNextTime:
	.string "I still can't beat you.\p"
	.string "...\p"
	.string "But that's okay, I can see I'm\n"
	.string "closer now than ever.\p"
	.string "I'm going to prepare a bit more\n"
	.string "before challenging the League.\p"
	.string "Next time we see each other may be\n"
	.string "at the Champion's Club.\p"
	.string "Later, {PLAYER}.$"

EverGrandeCity_PokemonLeague_1F_EventScript_Rye::
	lock
	faceplayer
	msgbox EverGrandeCity_PokemonLeague_1F_Text_RyeBeforeBattle, MSGBOX_DEFAULT
	goto EverGrandeCity_PokemonLeague_1F_EventScript_RyeBattle
	end

EverGrandeCity_PokemonLeague_1F_Text_RyeBeforeBattle:
	.string "Come at me, {PLAYER}.$"

EverGrandeCity_PokemonLeague_1F_EventScript_RyeBattle::
	trainerbattle_no_intro TRAINER_RYE_EG, EverGrandeCity_PokemonLeague_1F_Text_RyeDefeat
	goto EverGrandeCity_PokemonLeague_1F_EventScript_RyeLeave
	end

EverGrandeCity_PokemonLeague_1F_Text_RyeDefeat:
	.string "Another loss.$"

EverGrandeCity_PokemonLeague_1F_EventScript_RyeLeave::
	msgbox EverGrandeCity_PokemonLeague_1F_Text_RyeNextTime, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_RYE
	setflag FLAG_LEAGUE_RYE
	fadescreen FADE_FROM_BLACK
	release
	end

EverGrandeCity_PokemonLeague_1F_Text_RyeNextTime:
	.string "Man, no one but you two beat me\n"
	.string "so much.\p"
	.string "It doesn't feel bad, though.\p"
	.string "I could tell I can be useful now.\p"
	.string "If anything ever happens again,\n"
	.string "I'll be there.\p"
	.string "We can fight together again, as\n"
	.string "champions of the Hoenn Region.\p"
	.string "It's goodbye for the moment,\n"
	.string "{PLAYER}.$"

EverGrandeCity_PokemonLeague_1F_EventScript_FightRivals::
	msgbox EverGrandeCity_PokemonLeague_1F_Text_YouShouldFightRivals, MSGBOX_DEFAULT
	closemessage
	release
	end

EverGrandeCity_PokemonLeague_1F_Text_YouShouldFightRivals:
	.string "You should face those rivals of\n"
	.string "yours before entering.\p"
	.string "It seems like they really want to\n"
	.string "battle you.\p"
	.string "You're only young once.$"

