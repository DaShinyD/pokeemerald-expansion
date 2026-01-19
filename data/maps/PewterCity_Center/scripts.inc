.set LOCALID_NURSE, 1

PewterCity_Center_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_Center_OnTransition
	.byte 0

PewterCity_Center_OnTransition:
	setrespawn HEAL_LOCATION_PEWTER_CITY
	end

PewterCity_Center_EventScript_Nurse::
	lock
	msgbox PewterCity_Center_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox PewterCity_Center_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, PewterCity_Center_Movement_Turn
	release
	end

PewterCity_Center_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

PewterCity_Center_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

PewterCity_Center_Movement_Turn:
	face_down
	step_end

