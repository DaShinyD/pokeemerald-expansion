.set LOCALID_NURSE, 1

LavenderCenter_MapScripts::
	.byte 0

LavenderCenter_EventScript_Nurse::
	lock
	msgbox LavenderCenter_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox LavenderCenter_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, LavenderCenter_Movement_Turn
	release
	end

LavenderCenter_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

LavenderCenter_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

LavenderCenter_Movement_Turn:
	face_down
	step_end

