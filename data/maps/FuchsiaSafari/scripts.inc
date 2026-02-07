.set LOCALID_RANGER, 1
.set LOCALID_RHYPERIOR, 2

FuchsiaSafari_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, FuchsiaSafari_EventScript_OnFrame
	.byte 0

FuchsiaSafari_EventScript_OnFrame::
	map_script_2 VAR_TEMP_1, 0, FuchsiaSafari_EventScript_StartRangerEvent
	.2byte 0

FuchsiaSafari_EventScript_StartRangerEvent::
	setvar VAR_TEMP_1, 1
	lock
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	msgbox FuchsiaSafari_Text_OverHere, MSGBOX_DEFAULT
	closemessage
	turnobject LOCALID_RANGER, DIR_NORTH
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaSafari_Movement_GoUp
	waitmovement 0
	msgbox FuchsiaSafari_Text_TakeThis, MSGBOX_DEFAULT
	closemessage
	playfanfare MUS_OBTAIN_ITEM
	message FuchsiaSafari_Text_ReceivedStyler
	waitmessage
	waitfanfare
	msgbox FuchsiaSafari_Text_GoDoIt, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkUp4
	waitmovement 0
	applymovement LOCALID_RANGER, Common_Movement_WalkUp2
	waitmovement 0
	setvar VAR_0x8004, LOCALID_RHYPERIOR
	setvar VAR_0x8005, 0
	special StartRangerCapture
	waitstate
	goto_if_eq VAR_RESULT, TRUE, FuchsiaSafari_EventScript_Complete
	goto_if_eq VAR_RESULT, FALSE, FuchsiaSafari_EventScript_Restart
	release
	end

FuchsiaSafari_Text_OverHere:
	.string "Hey, over here.$"

FuchsiaSafari_Movement_GoUp:
	walk_left
	walk_up
	walk_up
	walk_right
	face_down
	step_end

FuchsiaSafari_Text_TakeThis:
	.string "Here, you will need this.$"

FuchsiaSafari_Text_ReceivedStyler:
	.string "Received Capture Styler!$"

FuchsiaSafari_Text_GoDoIt:
	.string "The Rhyperior is up there.\p"
	.string "You can do this, I believe.\p"
	.string "To complete a capture, hold the A\n"
	.string "button and loop around the pokemon.\p"
	.string "You can also hold B to move faster.\p"
	.string "Please help it!$"

FuchsiaSafari_EventScript_Complete::
	release
	end

FuchsiaSafari_EventScript_Restart::
	release
	end
