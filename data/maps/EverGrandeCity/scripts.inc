.set LOCALID_CHAMPBLOCK, 1

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
