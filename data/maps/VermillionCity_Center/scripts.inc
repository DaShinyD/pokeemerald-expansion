.set LOCALID_NURSE, 1

VermillionCity_Center_MapScripts::
	.byte 0

VermillionCity_Center_EventScript_Nurse::
	lock
	msgbox VermillionCity_Center_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox VermillionCity_Center_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, VermillionCity_Center_Movement_Turn
	release
	end

VermillionCity_Center_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

VermillionCity_Center_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

VermillionCity_Center_Movement_Turn:
	face_down
	step_end
