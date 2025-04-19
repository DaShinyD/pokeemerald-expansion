.set LOCALID_CHAMPBLOCK, 1
.set LOCALID_STRATA_GRUNT, 7
.set LOCALID_OAKBLOCK, 6

EverGrandeCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, EverGrandeCity_OnTransition
	.byte 0

EverGrandeCity_OnTransition:
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	end

EverGrandeCity_EventScript_VictoryRoadSign::
	msgbox EverGrandeCity_Text_EnteringVictoryRoad, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_CitySign::
	msgbox EverGrandeCity_Text_CitySign, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_PokemonLeagueSign::
	msgbox EverGrandeCity_Text_EnteringPokemonLeague, MSGBOX_SIGN
	end

EverGrandeCity_EventScript_SetVisitedEverGrande::
	setflag FLAG_VISITED_EVER_GRANDE_CITY
	setvar VAR_TEMP_1, 1
	end

EverGrandeCity_Text_EnteringVictoryRoad:
	.string "ENTERING VICTORY ROAD$"

EverGrandeCity_Text_EnteringPokemonLeague:
	.string "ENTERING POKéMON LEAGUE\n"
	.string "CENTER GATE$"

EverGrandeCity_Text_CitySign:
	.string "EVER GRANDE CITY\p"
	.string "“The paradise of flowers, the sea,\n"
	.string "and POKéMON.”$"



EverGrandeCity_EventScript_ChampClubBlocker::
	lock
	faceplayer
	goto_if_set FLAG_BASE_CHAMP_SET, EverGrandeCity_EventScript_MoveOutOfWay
	msgbox EverGrandeCity_Text_NeedToBeChamp, MSGBOX_DEFAULT
	release
	end

EverGrandeCity_Text_NeedToBeChamp:
	.string "Hey there champ in the making.\p"
	.string "Sorry, but you need to be a\n"
	.string "current or previous champion to\l"
	.string "enter.\p"
	.string "Seeing as how you're at the Elite\n"
	.string "four, maybe you can join soon!$"

EverGrandeCity_EventScript_MoveOutOfWay:
	msgbox EverGrandeCity_Text_OhYoureAChampion, MSGBOX_DEFAULT
	applymovement LOCALID_CHAMPBLOCK, EverGrandeCity_Movement_Leave
	waitmovement 0
	removeobject LOCALID_CHAMPBLOCK
	setflag FLAG_CHAMP_BLOCK
	release
	end

EverGrandeCity_Text_OhYoureAChampion:
	.string "If it isn't the new Hoenn champ!\p"
	.string "A real tough trainer like you is\n"
	.string "absolutely welcome to join our\l"
	.string "Champion Club.\p"
	.string "Here, you can train and battle\n"
	.string "with previous champions and\l"
	.string "trainers seen as masters.\p"
	.string "You can even find Gym Leaders\n"
	.string "from other regions!\p"
	.string "Maybe you can prove you're the\n"
	.string "number 1 of all time.\p"
	.string "Let me move out of your way.$"

EverGrandeCity_Movement_Leave:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

EverGrandeCity_EventScript_StrataGrunt::
	lock
	faceplayer
	msgbox EverGrandeCity_Text_StrataGrunt, MSGBOX_DEFAULT
	closemessage
	goto EverGrandeCity_EventScript_StrataGruntFight
	end

EverGrandeCity_Text_StrataGrunt:
	.string "You managed to stir some trouble at\n"
	.string "the Power Plant huh?\p"
	.string "You won't need to worry anymore\n"
	.string "once I'm done with you.$"

EverGrandeCity_EventScript_StrataGruntFight::
	trainerbattle_no_intro TRAINER_STRATA_GRUNT_2, EverGrandeCity_Text_StrataGruntDefeat
	goto EverGrandeCity_EventScript_StrataGruntLeave
	end

EverGrandeCity_Text_StrataGruntDefeat:
	.string "Tssk.$"

EverGrandeCity_EventScript_StrataGruntLeave::
	msgbox EverGrandeCity_Text_StrataGruntBye, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STRATA_GRUNT
	setflag FLAG_STRATA_HIDE_2
	clearflag FLAG_STRATA_HIDE_3
	fadescreen FADE_FROM_BLACK
	release
	end

EverGrandeCity_Text_StrataGruntBye:
	.string "You're just wasting your time\n"
	.string "here anyway.\p"
	.string "Straits will bring power to\n"
	.string "all, equally.\p"
	.string "Try and stop us.$"

EverGrandeCity_EventScript_OakBlock::
	lock
	applymovement LOCALID_OAKBLOCK, Common_Movement_FacePlayer
	waitmovement 0
	msgbox EverGrandeCity_Text_NoGo, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	release
	end

EverGrandeCity_Text_NoGo:
	.string "Sorry, but you can't go this way\n"
	.string "unless you have incredible merits.\p"
	.string "Try coming after achieving great\n"
	.string "feats.$"

