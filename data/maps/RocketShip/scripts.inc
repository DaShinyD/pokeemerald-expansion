.set LOCALID_SHIP_GUY, 8

RocketShip_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RocketShip_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, RocketShip_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, RocketShip_OnFrame
	.byte 0

RocketShip_OnTransition:
	setrespawn HEAL_LOCATION_MOSSDEEP_ROCKET
	end

RocketShip_OnWarp:
	map_script_2 VAR_MOSSDEEP_ROCKET, 0, RocketShip_EventScript_PlayerTurnNorth
	.2byte 0

RocketShip_EventScript_PlayerTurnNorth::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	end

RocketShip_OnFrame:
	map_script_2 VAR_MOSSDEEP_ROCKET, 0, RocketShip_EventScript_StartEvent
	.2byte 0

RocketShip_EventScript_StartEvent::
	lock
	setvar VAR_MOSSDEEP_ROCKET, 2
	msgbox RocketShip_Text_GoGettem, MSGBOX_DEFAULT
	release
	end

RocketShip_Text_GoGettem:
	.string "We've arrived to the Asteroid.\p"
	.string "You have to stop them.\p"
	.string "When you leave, you can use the\n"
	.string "teleport pad to come back in.\p"
	.string "Good luck, save us!$"

RocketShip_EventScript_ShipGuy::
	lock
	faceplayer
	goto_if_unset FLAG_TEAM_ROCKET, RocketShip_EventScript_NotYet
	msgbox RocketShip_Text_YouDidIt, MSGBOX_DEFAULT
	applymovement LOCALID_SHIP_GUY, RocketShip_Movement_Jump
	waitmovement 0
	msgbox RocketShip_Text_LetsGoHome, MSGBOX_DEFAULT
	setflag FLAG_HIDE_SHIP_GUY
	setflag FLAG_CLEAR_KANTO
	warp MAP_MOSSDEEP_CITY_SPACE_CENTER_2F, 13, 4
	release
	end

RocketShip_Text_YouDidIt:
	.string "You did it!!$"

RocketShip_Movement_Jump:
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	step_end

RocketShip_Text_LetsGoHome:
	.string "You are truly an incredible person.\p"
	.string "You have saved not only the region,\n"
	.string "but the planet multiple times.\p"
	.string "An accomplished trainer such as\n"
	.string "you should definitely dominate\l"
	.string "the Kanto region.\p"
	.string "It should probably be more open\n"
	.string "now that you took care of this.p"
	.string "As for me, I have much more\n"
	.string "research to do.\p"
	.string "Now, let's get you home.$"

RocketShip_EventScript_NotYet::
	msgbox RocketShip_Text_NotDone, MSGBOX_DEFAULT
	release
	end

RocketShip_Text_NotDone:
	.string "Good luck, we're all counting on\n"
	.string "you.$"

