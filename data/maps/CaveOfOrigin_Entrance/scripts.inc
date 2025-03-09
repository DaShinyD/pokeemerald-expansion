.set LOCALID_RIVAL, 2
.set LOCALID_DASH, 1
.set LOCALID_RYE, 4
.set LOCALID_WALLY, 3

CaveOfOrigin_Entrance_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CaveOfOrigin_Entrance_OnResume
	.byte 0

CaveOfOrigin_Entrance_OnResume:
	setescapewarp MAP_SOOTOPOLIS_CITY, 31, 17
	end

CaveOfOrigin_Entrance_EventScript_RivalsAppear::
	lock
	call Common_EventScript_SetupRivalGfxId
	addobject LOCALID_RYE
	addobject LOCALID_RIVAL
	addobject LOCALID_WALLY
	addobject LOCALID_DASH
	msgbox CaveOfOrigin_Entrance_Text_RivalsCallOut, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_RYE, CaveOfOrigin_Entrance_Movement_ApproachPlayer
	applymovement LOCALID_DASH, CaveOfOrigin_Entrance_Movement_ApproachPlayer
	applymovement LOCALID_RIVAL, CaveOfOrigin_Entrance_Movement_ApproachPlayer
	applymovement LOCALID_WALLY, CaveOfOrigin_Entrance_Movement_ApproachPlayer
	waitmovement 0
	msgbox CaveOfOrigin_Entrance_Text_Rivals, MSGBOX_DEFAULT
	waitmessage
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 3  @ shake delay
	special ShakeCamera
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	applymovement LOCALID_RYE, Common_Movement_ExclamationMark
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	applymovement LOCALID_RIVAL, Common_Movement_ExclamationMark
	applymovement LOCALID_WALLY, Common_Movement_ExclamationMark
	waitstate
	waitse
	waitmovement 0
	msgbox CaveOfOrigin_Entrance_Text_RivalsLeave, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, CaveOfOrigin_Entrance_Movement_LeavePlayer
	applymovement LOCALID_DASH, CaveOfOrigin_Entrance_Movement_LeavePlayer
	applymovement LOCALID_RIVAL, CaveOfOrigin_Entrance_Movement_LeavePlayer
	applymovement LOCALID_WALLY, CaveOfOrigin_Entrance_Movement_LeavePlayer
	waitmovement 0
	removeobject LOCALID_DASH
	removeobject LOCALID_RIVAL
	removeobject LOCALID_RYE
	removeobject LOCALID_WALLY
	setvar VAR_RIVALS_CAVE_OF_ORIGIN, 1
	release
	end

CaveOfOrigin_Entrance_Text_RivalsCallOut:
	.string "All: {PLAYER}!$"

CaveOfOrigin_Entrance_Movement_ApproachPlayer:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

CaveOfOrigin_Entrance_Text_Rivals:
	.string "Dash: Glad to see you're okay,\n"
	.string "{PLAYER}.\p"
	.string "We heard you beat Archie.\p"
	.string "But now both Kyogre and Groudon\n"
	.string "are causing mayhem!\p"
	.string "Rye: Did you find out any way to\n"
	.string "stop all of this?\p"
	.string "...\p"
	.string "Sky Pillar, huh? Rayquaza can\n"
	.string "stop them?\p"
	.string "Sounds like a plan.\p"
	.string "{RIVAL}: You can do this.\p"
	.string "We'll hold down the fort here and\n"
	.string "keep people safe.\p"
	.string "Wally: You can count on us.$"

CaveOfOrigin_Entrance_Text_RivalsLeave:
	.string "Wally: You'd better get going now!\p"
	.string "We don't have long.\p"
	.string "Dash: We'll go now, good luck.$"

CaveOfOrigin_Entrance_Movement_LeavePlayer:
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	step_end

