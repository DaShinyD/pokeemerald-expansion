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

