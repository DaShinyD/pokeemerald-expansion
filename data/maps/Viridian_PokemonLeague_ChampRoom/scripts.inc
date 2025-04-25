.set LOCALID_GREEN, 1
.set LOCALID_RED, 2

Viridian_PokemonLeague_ChampRoom_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Viridian_PokemonLeague_ChampRoom_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, Viridian_PokemonLeague_ChampRoom_OnWarp
	.byte 0

Viridian_PokemonLeague_ChampRoom_OnWarp:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_ChampRoom_EventScript_PlayerTurnNorth
	.2byte 0

Viridian_PokemonLeague_ChampRoom_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

Viridian_PokemonLeague_ChampRoom_OnFrame:
	map_script_2 VAR_TEMP_1, 0, Viridian_PokemonLeague_ChampRoom_EventScript_WalkInStartEvent
	.2byte 0

Viridian_PokemonLeague_ChampRoom_EventScript_WalkInStartEvent::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Viridian_PokemonLeague_ChampRoom_Movement_WalkIn
	waitmovement 0
	setvar VAR_TEMP_1, 1
	applymovement LOCALID_GREEN, Common_Movement_WalkInPlaceDown
	msgbox Viridian_PokemonLeague_ChampRoom_Text_RedAndGreen, MSGBOX_DEFAULT
	goto Viridian_PokemonLeague_ChampRoom_EventScript_ChampDoubleBattle
	releaseall
	end

Viridian_PokemonLeague_ChampRoom_Movement_WalkIn:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Viridian_PokemonLeague_ChampRoom_Text_RedAndGreen:
	.string "Welcome!\p"
	.string "The guy with the Red hat is...Red.\p"
	.string "I'm Green.\p"
	.string "Green: We are the champions of the\n"
	.string "Indigo League.\p"
	.string "Weird right?\p"
	.string "We both keep winning against each\n"
	.string "other, so eventually they told us\l"
	.string "to just share it.\p"
	.string "Both of us have traveled far and\n"
	.string "wide.\p"
	.string "However.\p"
	.string "It seems you haven't yet unlocked\n"
	.string "{COLOR RED}that {COLOR DARK_GRAY}yet.\p"
	.string "We won't take you on with our full\n"
	.string "power.\p"
	.string "Perhaps this battle will allow you\n"
	.string "to grasp it.\p"
	.string "Haha, sorry, I'm rambling.\p"
	.string "He isn't much of a talker either.\p"
	.string "Now, you face the power of Kanto!$"

Viridian_PokemonLeague_ChampRoom_EventScript_ChampDoubleBattle::
	trainerbattle_two_trainers TRAINER_RED, Viridian_PokemonLeague_ChampRoom_Text_RedDefeat, TRAINER_LEAF, Viridian_PokemonLeague_ChampRoom_Text_GreenDefeat
	goto Viridian_PokemonLeague_ChampRoom_EventScript_After
	end

Viridian_PokemonLeague_ChampRoom_Text_RedDefeat:
	.string "...$"

Viridian_PokemonLeague_ChampRoom_Text_GreenDefeat:
	.string "Oh? I see it...$"

Viridian_PokemonLeague_ChampRoom_EventScript_After::
	setflag FLAG_INDIGO_BEAT
	setflag FLAG_LEVEL_CAP_TEN
	msgbox Viridian_PokemonLeague_ChampRoom_Text_GreenAfter, MSGBOX_DEFAULT
	applymovement LOCALID_GREEN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_GREEN, Common_Movement_FaceUp
	waitmovement 0
	msgbox Viridian_PokemonLeague_ChampRoom_Text_GreenCall, MSGBOX_DEFAULT
	applymovement LOCALID_GREEN, Common_Movement_FaceDown
	waitmovement 0
	msgbox Viridian_PokemonLeague_ChampRoom_Text_GreenExplain, MSGBOX_DEFAULT
	setvar VAR_MOSSDEEP_ROCKET, 1
	clearflag FLAG_HIDE_SHIP_GUY
	warp MAP_VERMILLION_CITY, 23, 33
	release
	end

Viridian_PokemonLeague_ChampRoom_Text_GreenAfter:
	.string "Green: I saw it!\p"
	.string "You're Pokemon, they can now grow\n"
	.string "stronger.\p"
	.string "Listen, when the time comes, we\n"
	.string "will get a Hoenn Ferry Pass.\p"
	.string "We'll wait for you near the\n"
	.string "Champion Club.\p"
	.string "There, you can face us at our full\n"
	.string "strength, individually.\p"
	.string "Ring ring.\p"
	.string "Oh!$"

Viridian_PokemonLeague_ChampRoom_Text_GreenCall:
	.string "Hello?\p"
	.string "Uh huh. Uh huh.\p"
	.string "...\p"
	.string "WHAT?!?!\p"
	.string "He's back?\p"
	.string "He just doesn't learn.\p"
	.string "I understand.\p"
	.string "Actually, I have someone I can send.\p"
	.string "Will do. Buh bye.$"

Viridian_PokemonLeague_ChampRoom_Text_GreenExplain:
	.string "Hey, uh, what was your name?\p"
	.string "{PLAYER}, got it.\p"
	.string "Listen, something is going down.\p"
	.string "You need to get back to Hoenn.\p"
	.string "It isn't happening there, but you\n"
	.string "should go to the Mossdeep Space\l"
	.string "Center, pronto.\p"
	.string "They'll explain more there.\p"
	.string "Sorry to cut things short, but\n"
	.string "it was a great battle.\p"
	.string "My Alakazam is gonna teleport\n"
	.string "you back to the boat.\p"
	.string "Sorry again, I promise everything\n"
	.string "will make sense back at Mossdeep!$"

