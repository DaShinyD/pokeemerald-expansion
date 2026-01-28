.set LOCALID_COP_1, 2
.set LOCALID_CHAUVIN, 3
.set LOCALID_COP_2, 4
.set LOCALID_FLOYD, 5
.set LOCALID_FLOYD_CAR, 6
.set LOCALID_ITEM, 7
.set LOCALID_BYSTANDER_1, 8
.set LOCALID_BYSTANDER_2, 9
.set LOCALID_BYSTANDER_3, 10
.set LOCALID_BYSTANDER_4, 11
.set LOCALID_BYSTANDER_5, 12
.set LOCALID_BYSTANDER_6, 13
.set LOCALID_BYSTANDER_7, 14
.set LOCALID_BYSTANDER_8, 15
.set LOCALID_YUGI, 24

SaffronCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SaffronCity_OnTransition
	.byte 0

SaffronCity_OnTransition:
	setflag FLAG_VISITED_SAFFRON
	end

@ Normal Stuff

SaffronCity_EventScript_Scientist::
	msgbox SaffronCity_Text_Scientist, MSGBOX_NPC
	end

SaffronCity_Text_Scientist:
	.string "I'm trying to get a job at the Silph\n"
	.string "Company.\p"
	.string "But they say my jars aren't scientific.$"

SaffronCity_EventScript_GymBlocker::
	msgbox SaffronCity_Text_NoSabrina, MSGBOX_NPC
	end

SaffronCity_Text_NoSabrina:
	.string "Sabrina is currently at the battle\n"
	.string "club.$"

SaffronCity_EventScript_DojoBlocker::
	msgbox SaffronCity_Text_DojoMan, MSGBOX_NPC
	end

SaffronCity_Text_DojoMan:
	.string "Our dojo master left for Hoenn with\n"
	.string "his brother a while ago.\p"
	.string "I think they established a new dojo.$"

SaffronCity_EventScript_Psychic1::
	msgbox SaffronCity_Text_Psychic1, MSGBOX_NPC
	end

SaffronCity_Text_Psychic1:
	.string "My mind's eye tells me not to have\n"
	.string "a battle with you.$"

SaffronCity_EventScript_Psychic2::
	msgbox SaffronCity_Text_Psychic2, MSGBOX_NPC
	end

SaffronCity_Text_Psychic2:
	.string "I see, you beat Sabrina...$"

SaffronCity_EventScript_Psychic3::
	msgbox SaffronCity_Text_Psychic3, MSGBOX_NPC
	end

SaffronCity_Text_Psychic3:
	.string "The psychic energy in Saffron is\n"
	.string "off the charts.$"

SaffronCity_EventScript_Psychic4::
	msgbox SaffronCity_Text_Psychic4, MSGBOX_NPC
	end

SaffronCity_Text_Psychic4:
	.string "...\p"
	.string "Did you hear my message?\p"
	.string "Guess not...$"

SaffronCity_EventScript_Locked::
	msgbox SaffronCity_Text_Locked, MSGBOX_SIGN
	end

SaffronCity_Text_Locked:
	.string "It's locked.$"

@ Floyd Stuff

SaffronCity_EventScript_FloydLeftL::
	lock
	removeobject LOCALID_ITEM
	removeobject LOCALID_FLOYD
	special DrawWholeMapView
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_Movement_CameraPanLeftL
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_GetHim, MSGBOX_DEFAULT
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_COP_1, SaffronCity_Movement_PullOut
	applymovement LOCALID_COP_2, SaffronCity_Movement_PullOut
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_PullOut
	applymovement LOCALID_FLOYD, SaffronCity_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_StopResist, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox SaffronCity_Text_Dindu, MSGBOX_DEFAULT
	removefieldmugshot
	msgbox SaffronCity_Text_Bystanders, MSGBOX_DEFAULT
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_ChauvinLookBack
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_ChauvinWarn, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_BYSTANDER_1, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_2, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_3, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_4, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_5, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_6, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_7, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_8, SaffronCity_Movement_BystandersRunR
	waitmovement 0
	closemessage
	removeobject LOCALID_BYSTANDER_1
	removeobject LOCALID_BYSTANDER_2
	removeobject LOCALID_BYSTANDER_3
	removeobject LOCALID_BYSTANDER_4
	removeobject LOCALID_BYSTANDER_5
	removeobject LOCALID_BYSTANDER_6
	removeobject LOCALID_BYSTANDER_7
	removeobject LOCALID_BYSTANDER_8
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, SaffronCity_Movement_FentFly
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FLOYD
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD_CAR
	setflag FLAG_FLOYD_COPS
	setvar VAR_FLOYD, 0
	clearflag FLAG_FLOYD
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_MovementCameraPanBackLeftL
	waitmovement 0
	special RemoveCameraObject
	release
	end
	
SaffronCity_EventScript_FloydLeftM::
	lock
	removeobject LOCALID_ITEM
	removeobject LOCALID_FLOYD
	special DrawWholeMapView
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_Movement_CameraPanLeftM
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_GetHim, MSGBOX_DEFAULT
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_COP_1, SaffronCity_Movement_PullOut
	applymovement LOCALID_COP_2, SaffronCity_Movement_PullOut
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_PullOut
	applymovement LOCALID_FLOYD, SaffronCity_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_StopResist, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox SaffronCity_Text_Dindu, MSGBOX_DEFAULT
	removefieldmugshot
	msgbox SaffronCity_Text_Bystanders, MSGBOX_DEFAULT
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_ChauvinLookBack
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_ChauvinWarn, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_BYSTANDER_1, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_2, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_3, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_4, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_5, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_6, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_7, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_8, SaffronCity_Movement_BystandersRunR
	waitmovement 0
	closemessage
	removeobject LOCALID_BYSTANDER_1
	removeobject LOCALID_BYSTANDER_2
	removeobject LOCALID_BYSTANDER_3
	removeobject LOCALID_BYSTANDER_4
	removeobject LOCALID_BYSTANDER_5
	removeobject LOCALID_BYSTANDER_6
	removeobject LOCALID_BYSTANDER_7
	removeobject LOCALID_BYSTANDER_8
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, SaffronCity_Movement_FentFly
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FLOYD
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD_CAR
	setflag FLAG_FLOYD_COPS
	setvar VAR_FLOYD, 0
	clearflag FLAG_FLOYD
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_MovementCameraPanBackLeftM
	waitmovement 0
	special RemoveCameraObject
	release
	end

SaffronCity_EventScript_FloydLeftR::
	lock
	removeobject LOCALID_ITEM
	removeobject LOCALID_FLOYD
	special DrawWholeMapView
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_Movement_CameraPanLeftR
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_GetHim, MSGBOX_DEFAULT
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_COP_1, SaffronCity_Movement_PullOut
	applymovement LOCALID_COP_2, SaffronCity_Movement_PullOut
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_PullOut
	applymovement LOCALID_FLOYD, SaffronCity_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_StopResist, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox SaffronCity_Text_Dindu, MSGBOX_DEFAULT
	removefieldmugshot
	msgbox SaffronCity_Text_Bystanders, MSGBOX_DEFAULT
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_ChauvinLookBack
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_ChauvinWarn, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_BYSTANDER_1, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_2, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_3, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_4, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_5, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_6, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_7, SaffronCity_Movement_BystandersRunR
	applymovement LOCALID_BYSTANDER_8, SaffronCity_Movement_BystandersRunR
	waitmovement 0
	closemessage
	removeobject LOCALID_BYSTANDER_1
	removeobject LOCALID_BYSTANDER_2
	removeobject LOCALID_BYSTANDER_3
	removeobject LOCALID_BYSTANDER_4
	removeobject LOCALID_BYSTANDER_5
	removeobject LOCALID_BYSTANDER_6
	removeobject LOCALID_BYSTANDER_7
	removeobject LOCALID_BYSTANDER_8
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, SaffronCity_Movement_FentFly
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FLOYD
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD_CAR
	setflag FLAG_FLOYD_COPS
	setvar VAR_FLOYD, 0
	clearflag FLAG_FLOYD
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_MovementCameraPanBackLeftR
	waitmovement 0
	special RemoveCameraObject
	release
	end

SaffronCity_EventScript_FloydRightL::
	lock
	removeobject LOCALID_ITEM
	removeobject LOCALID_FLOYD
	special DrawWholeMapView
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_Movement_CameraPanRightL
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_GetHim, MSGBOX_DEFAULT
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_COP_1, SaffronCity_Movement_PullOut
	applymovement LOCALID_COP_2, SaffronCity_Movement_PullOut
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_PullOut
	applymovement LOCALID_FLOYD, SaffronCity_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_StopResist, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox SaffronCity_Text_Dindu, MSGBOX_DEFAULT
	removefieldmugshot
	msgbox SaffronCity_Text_Bystanders, MSGBOX_DEFAULT
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_ChauvinLookBack
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_ChauvinWarn, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_BYSTANDER_1, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_2, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_3, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_4, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_5, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_6, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_7, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_8, SaffronCity_Movement_BystandersRunL
	waitmovement 0
	closemessage
	removeobject LOCALID_BYSTANDER_1
	removeobject LOCALID_BYSTANDER_2
	removeobject LOCALID_BYSTANDER_3
	removeobject LOCALID_BYSTANDER_4
	removeobject LOCALID_BYSTANDER_5
	removeobject LOCALID_BYSTANDER_6
	removeobject LOCALID_BYSTANDER_7
	removeobject LOCALID_BYSTANDER_8
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, SaffronCity_Movement_FentFly
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FLOYD
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD_CAR
	setflag FLAG_FLOYD_COPS
	setvar VAR_FLOYD, 0
	clearflag FLAG_FLOYD
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_MovementCameraPanBackRightL
	waitmovement 0
	special RemoveCameraObject
	release
	end

SaffronCity_EventScript_FloydRightM::
	lock
	removeobject LOCALID_ITEM
	removeobject LOCALID_FLOYD
	special DrawWholeMapView
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_Movement_CameraPanRightM
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_GetHim, MSGBOX_DEFAULT
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_COP_1, SaffronCity_Movement_PullOut
	applymovement LOCALID_COP_2, SaffronCity_Movement_PullOut
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_PullOut
	applymovement LOCALID_FLOYD, SaffronCity_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_StopResist, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox SaffronCity_Text_Dindu, MSGBOX_DEFAULT
	removefieldmugshot
	msgbox SaffronCity_Text_Bystanders, MSGBOX_DEFAULT
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_ChauvinLookBack
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_ChauvinWarn, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_BYSTANDER_1, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_2, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_3, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_4, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_5, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_6, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_7, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_8, SaffronCity_Movement_BystandersRunL
	waitmovement 0
	closemessage
	removeobject LOCALID_BYSTANDER_1
	removeobject LOCALID_BYSTANDER_2
	removeobject LOCALID_BYSTANDER_3
	removeobject LOCALID_BYSTANDER_4
	removeobject LOCALID_BYSTANDER_5
	removeobject LOCALID_BYSTANDER_6
	removeobject LOCALID_BYSTANDER_7
	removeobject LOCALID_BYSTANDER_8
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, SaffronCity_Movement_FentFly
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FLOYD
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD_CAR
	setflag FLAG_FLOYD_COPS
	setvar VAR_FLOYD, 0
	clearflag FLAG_FLOYD
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_MovementCameraPanBackRightM
	waitmovement 0
	special RemoveCameraObject
	release
	end

SaffronCity_EventScript_FloydRightR::
	lock
	removeobject LOCALID_ITEM
	removeobject LOCALID_FLOYD
	special DrawWholeMapView
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_Movement_CameraPanRightR
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_GetHim, MSGBOX_DEFAULT
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_COP_1, SaffronCity_Movement_PullOut
	applymovement LOCALID_COP_2, SaffronCity_Movement_PullOut
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_PullOut
	applymovement LOCALID_FLOYD, SaffronCity_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_StopResist, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox SaffronCity_Text_Dindu, MSGBOX_DEFAULT
	removefieldmugshot
	msgbox SaffronCity_Text_Bystanders, MSGBOX_DEFAULT
	applymovement LOCALID_CHAUVIN, SaffronCity_Movement_ChauvinLookBack
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox SaffronCity_Text_ChauvinWarn, MSGBOX_DEFAULT
	removefieldmugshot
	applymovement LOCALID_BYSTANDER_1, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_2, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_3, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_4, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_5, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_6, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_7, SaffronCity_Movement_BystandersRunL
	applymovement LOCALID_BYSTANDER_8, SaffronCity_Movement_BystandersRunL
	waitmovement 0
	closemessage
	removeobject LOCALID_BYSTANDER_1
	removeobject LOCALID_BYSTANDER_2
	removeobject LOCALID_BYSTANDER_3
	removeobject LOCALID_BYSTANDER_4
	removeobject LOCALID_BYSTANDER_5
	removeobject LOCALID_BYSTANDER_6
	removeobject LOCALID_BYSTANDER_7
	removeobject LOCALID_BYSTANDER_8
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, SaffronCity_Movement_FentFly
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_FLOYD
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD_CAR
	setflag FLAG_FLOYD_COPS
	setvar VAR_FLOYD, 0
	clearflag FLAG_FLOYD
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_CAMERA, SaffronCity_MovementCameraPanBackRightR
	waitmovement 0
	special RemoveCameraObject
	release
	end

SaffronCity_Movement_CameraPanLeftL:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

SaffronCity_Movement_CameraPanLeftM:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

SaffronCity_Movement_CameraPanLeftR:
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

SaffronCity_Movement_CameraPanRightR:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

SaffronCity_Movement_CameraPanRightM:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

SaffronCity_Movement_CameraPanRightL:
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	walk_fast_up
	step_end

SaffronCity_Text_GetHim:
	.string "Get down criminal!\p"
	.string "Pull this thief out.$"

SaffronCity_Movement_PullOut:
	lock_facing_direction
	walk_slow_down
	unlock_facing_direction
	step_end

SaffronCity_Text_StopResist:
	.string "Stop resisting!$"

SaffronCity_Movement_JumpOnFloyd:
	jump_up
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	step_end

SaffronCity_Text_Dindu:
	.string "Stop stop!\p"
	.string "I dindu nuffin!\p"
	.string "I...can't...breeve...$"

SaffronCity_Text_Bystanders:
	.string "Bystanders: Omg! Stop! He's gonna die!$"

SaffronCity_Movement_ChauvinLookBack:
	face_down
	step_end

SaffronCity_Text_ChauvinWarn:
	.string "This guy is overdosing and resisting\n"
	.string "and they are worried about me...\p"
	.string "Typical LIBERAL Amer-I mean Kanto.\p"
	.string "Come on men, let's take him away.\p"
	.string "EVERYONE DISPERSE NOW!$"

SaffronCity_Movement_BystandersRunR:
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	step_end

SaffronCity_Movement_BystandersRunL:
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	step_end

SaffronCity_Movement_FentFly:
	walk_faster_down
	walk_faster_down
	walk_faster_down
	step_end

SaffronCity_MovementCameraPanBackLeftL:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

SaffronCity_MovementCameraPanBackLeftM:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

SaffronCity_MovementCameraPanBackLeftR:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	walk_fast_left
	step_end

SaffronCity_MovementCameraPanBackRightL:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SaffronCity_MovementCameraPanBackRightM:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

SaffronCity_MovementCameraPanBackRightR:
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_down
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	walk_fast_right
	step_end

@ TP Man Stuff

SaffronCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox SaffronCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_SAFFRON,   SaffronCity_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  SaffronCity_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    SaffronCity_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  SaffronCity_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    SaffronCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  SaffronCity_EventScript_WarpLavender
	case TPMAN_SELECTION_VERMILION, SaffronCity_EventScript_WarpVermilion
	case TPMAN_SELECTION_CELADON,   SaffronCity_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   SaffronCity_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  SaffronCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      SaffronCity_EventScript_Exit
	case MULTI_B_PRESSED,           SaffronCity_EventScript_Rude
	release
	end

SaffronCity_EventScript_YoureHere::
	msgbox SaffronCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

SaffronCity_EventScript_WarpCerulean::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

SaffronCity_EventScript_WarpPewter::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

SaffronCity_EventScript_WarpViridian::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

SaffronCity_EventScript_WarpPallet::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

SaffronCity_EventScript_WarpLavender::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

SaffronCity_EventScript_WarpVermilion::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

SaffronCity_EventScript_WarpCeladon::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

SaffronCity_EventScript_WarpFuchsia::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

SaffronCity_EventScript_WarpCinnabar::
	msgbox SaffronCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

SaffronCity_EventScript_Exit::
	msgbox SaffronCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

SaffronCity_EventScript_Rude::
	msgbox SaffronCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

SaffronCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

SaffronCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

SaffronCity_Text_GuessNot:
	.string "Guess not.$"

SaffronCity_Text_Rude:
	.string "Rude, there's an exit button.$"

SaffronCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

@ Yugi Stuff

SaffronCity_EventScript_YugiT::
	lock
	addobject LOCALID_YUGI
	applymovement LOCALID_YUGI, SaffronCity_Movement_YugiApproachT
	waitmovement 0
	createfieldmugshot MUGSHOT_YUGI
	msgbox SaffronCity_Text_YugiStop, MSGBOX_DEFAULT
	closemessage
	removefieldmugshot
	goto SaffronCity_EventScript_YugiBattle
	release
	end

SaffronCity_EventScript_YugiM::
	lock
	addobject LOCALID_YUGI
	applymovement LOCALID_YUGI, SaffronCity_Movement_YugiApproachM
	waitmovement 0
	createfieldmugshot MUGSHOT_YUGI
	msgbox SaffronCity_Text_YugiStop, MSGBOX_DEFAULT
	closemessage
	removefieldmugshot
	goto SaffronCity_EventScript_YugiBattle
	release
	end

SaffronCity_EventScript_YugiB::
	lock
	addobject LOCALID_YUGI
	applymovement LOCALID_YUGI, SaffronCity_Movement_YugiApproachB
	waitmovement 0
	createfieldmugshot MUGSHOT_YUGI
	msgbox SaffronCity_Text_YugiStop, MSGBOX_DEFAULT
	closemessage
	removefieldmugshot
	goto SaffronCity_EventScript_YugiBattle
	release
	end

SaffronCity_Movement_YugiApproachT:
	walk_right
	walk_right
	walk_up
	face_right
	step_end

SaffronCity_Movement_YugiApproachM:
	walk_right
	walk_right
	step_end

SaffronCity_Movement_YugiApproachB:
	walk_right
	walk_right
	walk_down
	face_right
	step_end

SaffronCity_Text_YugiStop:
	.string "You there, what did you do with my\n"
	.string "Grandpa?$"

SaffronCity_EventScript_YugiBattle::
	trainerbattle_no_intro TRAINER_YUGI, SaffronCity_Text_What
	goto SaffronCity_EventScript_YugiLeave
	release
	end

SaffronCity_Text_What:
	.string "Hu-huh??$"

SaffronCity_EventScript_YugiLeave::
	createfieldmugshot MUGSHOT_YUGI
	msgbox SaffronCity_Text_WhatHappening, MSGBOX_DEFAULT
	closemessage
	removefieldmugshot
	setvar VAR_0x8000, 1
	callnative OpenFullscreenPopupBgFromScript
	waitstate
	msgbox SaffronCity_Text_Attention, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_YUGI, Common_Movement_ExclamationMark
	playse SE_PIN
	createfieldmugshot MUGSHOT_YUGI
	msgbox SaffronCity_Text_GottaGo, MSGBOX_DEFAULT
	closemessage
	removefieldmugshot
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_YUGI
	setflag FLAG_YUGI_SAFFRON
	setvar VAR_YUGI, 1
	fadescreen FADE_FROM_BLACK
	release
	end

SaffronCity_Text_WhatHappening:
	.string "You may have beaten me, but I\n"
	.string "won't stop looking for Grandpa!\p"
	.string "...\p"
	.string "Wait, you don't know who I am?\p"
	.string "Oh boy, I think I jumped the gun.\p"
	.string "I'm sorry.$"

SaffronCity_Text_Attention:
	.string "ATTENTION DUELISTS!\p"
	.string "My hair would like you to know that\n"
	.string "kidnapped Grandpas are over here.$"

SaffronCity_Text_GottaGo:
	.string "Gotta go! Sorry again.$"

SaffronCity_EventScript_NeedCard::
	msgbox SaffronCity_Text_NeedCard, MSGBOX_SIGN
	release
	end

SaffronCity_Text_NeedCard:
	.string "Key card required.$"

