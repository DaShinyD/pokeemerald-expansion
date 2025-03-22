.set LOCALID_WALLACE, 1
.set LOCALID_GRUNT1, 2
.set LOCALID_STRAITS, 3
.set LOCALID_GRUNT2, 4
.set LOCALID_GRUNT3, 5

SkyPillar_Outside_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SkyPillar_Outside_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, SkyPillar_Outside_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, SkyPillar_Outside_OnFrame
	.byte 0

SkyPillar_Outside_OnTransition:
	call_if_eq VAR_SOOTOPOLIS_CITY_STATE, 3, SkyPillar_Outside_EventScript_HideMapNamePopup
	call_if_ge VAR_SOOTOPOLIS_CITY_STATE, 4, SkyPillar_Outside_EventScript_CheckSetAbnormalWeather
	end

SkyPillar_Outside_EventScript_HideMapNamePopup::
	setflag FLAG_HIDE_MAP_NAME_POPUP
	return

SkyPillar_Outside_EventScript_CheckSetAbnormalWeather::
	call_if_set FLAG_SYS_WEATHER_CTRL, Common_EventScript_SetAbnormalWeather
	return

SkyPillar_Outside_OnLoad:
	call_if_set FLAG_WALLACE_GOES_TO_SKY_PILLAR, SkyPillar_Outside_EventScript_OpenDoor
	end

SkyPillar_Outside_EventScript_OpenDoor::
	setmetatile 14, 4, METATILE_Pacifidlog_SkyPillar_DoorOpen_Top, FALSE
	setmetatile 14, 5, METATILE_Pacifidlog_SkyPillar_DoorOpen_Bottom, FALSE
	return

SkyPillar_Outside_OnFrame:
	map_script_2 VAR_SOOTOPOLIS_CITY_STATE, 3, SkyPillar_Outside_EventScript_WallaceScene
	.2byte 0

SkyPillar_Outside_EventScript_WallaceScene::
	lockall
	applymovement LOCALID_WALLACE, SkyPillar_Outside_Movement_WallaceApproachPlayer
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox SkyPillar_Outside_Text_OpenedDoorToSkyPillar, MSGBOX_DEFAULT
	closemessage
	delay 30
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 3  @ shake delay
	special ShakeCamera
	waitstate
	delay 40
	msgbox SkyPillar_Outside_Text_EarthquakeNotMomentToWaste, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, SkyPillar_Outside_Movement_PlayerClimbSkyPillar
	applymovement LOCALID_WALLACE, SkyPillar_Outside_Movement_WallaceClimbSkyPillar
	waitmovement 0
	setvar VAR_0x8004, 1   @ vertical pan
	setvar VAR_0x8005, 3   @ horizontal pan
	setvar VAR_0x8006, 20  @ num shakes
	setvar VAR_0x8007, 5   @ shake delay
	special ShakeCamera
	waitstate
	delay 20
	applymovement LOCALID_WALLACE, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 10
	applymovement LOCALID_WALLACE, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 20
	applymovement LOCALID_WALLACE, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 30
	msgbox SkyPillar_Outside_Text_SituationGettingWorse, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_SYS_WEATHER_CTRL
	setweather WEATHER_ABNORMAL
	doweather
	special WaitWeather
	waitstate
	delay 30
	msgbox SkyPillar_Outside_Text_GotToGoBackForSootopolis, MSGBOX_DEFAULT
	closemessage
	playse SE_EXIT
	fadescreenswapbuffers FADE_TO_BLACK
	clearflag FLAG_HIDE_MAP_NAME_POPUP
	setvar VAR_SOOTOPOLIS_CITY_STATE, 4
	removeobject LOCALID_WALLACE
	clearflag FLAG_HIDE_SOOTOPOLIS_CITY_WALLACE
	fadescreenswapbuffers FADE_FROM_BLACK
	releaseall
	end

SkyPillar_Outside_Movement_WallaceApproachPlayer:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	step_end

SkyPillar_Outside_Movement_WallaceClimbSkyPillar:
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

SkyPillar_Outside_Movement_PlayerClimbSkyPillar:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

SkyPillar_Outside_EventScript_Wallace::
	end

SkyPillar_Outside_EventScript_ClosedDoor::
	msgbox SkyPillar_Outside_Text_DoorIsClosed, MSGBOX_SIGN
	end

SkyPillar_Outside_Text_DoorIsClosed:
	.string "The door is closed.$"

SkyPillar_Outside_Text_OpenedDoorToSkyPillar:
	.string "WALLACE: Oh, my, I'm terribly sorry!\p"
	.string "In my haste, I didn't notice that\n"
	.string "I'd left you behind!\p"
	.string "I've opened the locked door of\n"
	.string "the SKY PILLAR.\p"
	.string "{PLAYER}{KUN}, let's be on our way!$"

SkyPillar_Outside_Text_EarthquakeNotMomentToWaste:
	.string "WALLACE: It's an earthquake!\p"
	.string "There's not a moment to waste!\n"
	.string "We've got to hurry!$"

SkyPillar_Outside_Text_SituationGettingWorse:
	.string "WALLACE: Hmm…\n"
	.string "The situation is getting worse…$"

SkyPillar_Outside_Text_GotToGoBackForSootopolis:
	.string "WALLACE: This isn't good…\p"
	.string "The weather distortion is spreading\n"
	.string "even here…\p"
	.string "{PLAYER}{KUN}.\p"
	.string "RAYQUAZA should be farther up\n"
	.string "from here.\p"
	.string "I'm worried about SOOTOPOLIS.\n"
	.string "I've got to go back.\p"
	.string "Everything is in your hands now.\n"
	.string "Don't fail us!$"



SkyPillar_Outside_EventScript_StrataGrunt::
	lock
	faceplayer
	goto_if_unset FLAG_START_STRATA, SkyPillar_Outside_EventScript_BuzzOff
	goto_if_set FLAG_STRATA_1, SkyPillar_Outside_EventScript_NoPass
	msgbox SkyPillar_Outside_Text_AhTheHero, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT1, Common_Movement_ExclamationMark
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 40
	applymovement LOCALID_GRUNT1, Common_Movement_FaceDown
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	waitmovement 0
	addobject LOCALID_STRAITS
	addobject LOCALID_GRUNT2
	addobject LOCALID_GRUNT3
	applymovement LOCALID_STRAITS, SkyPillar_Outside_Movement_StraitsApproach
	applymovement LOCALID_GRUNT2, SkyPillar_Outside_Movement_Grunt2Approach
	applymovement LOCALID_GRUNT3, SkyPillar_Outside_Movement_Grunt3Approach
	waitmovement 0
	msgbox SkyPillar_Outside_Text_StraitsSpeech, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT3, SkyPillar_Outside_Movement_Grunt3Leave
	applymovement LOCALID_GRUNT2, SkyPillar_Outside_Movement_Grunt2Leave
	applymovement LOCALID_STRAITS, SkyPillar_Outside_Movement_StraitsLeave
	waitmovement 0
	removeobject LOCALID_GRUNT2
	removeobject LOCALID_GRUNT3
	removeobject LOCALID_GRUNT3
	setflag FLAG_STRATA_1
	release
	end

SkyPillar_Outside_Text_AhTheHero:
	.string "If it isn't the hero of Hoenn.\n"
	.string "Keh heh heh.\p"
	.string "We, Team Strata, expected you might\n"
	.string "come back here.\p"
	.string "Sorry, but Rayquaza is preoccupied.\p"
	.string "We will show everyone those fools\n"
	.string "in Team Aqua and Magma were too\l"
	.string "small for their own good.\p"
	.string "???: That's enough, grunt.$"

SkyPillar_Outside_Movement_StraitsApproach:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	face_up
	step_end

SkyPillar_Outside_Movement_Grunt2Approach:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	face_up
	step_end

SkyPillar_Outside_Movement_Grunt3Approach:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	face_up
	step_end

SkyPillar_Outside_Text_StraitsSpeech:
	.string "???: Hero of Hoenn, I saw you before.\p"
	.string "???: My name is Straits.\p"
	.string "Straits: My team will save the region\n"
	.string "by harnessing the power of Pokemon.\p"
	.string "There is so much possibility that we\n"
	.string "refuse because it is wrong according\l"
	.string "to the strongest.\p"
	.string "No longer.\p"
	.string "The accepted norms will be broken by\n"
	.string "me and Rayquaza.\p"
	.string "Starting with mega Pokemon.\p"
	.string "I'm confident you cannot defy me.\p"
	.string "Try and find us if you can.$"

SkyPillar_Outside_Movement_Grunt3Leave:
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

SkyPillar_Outside_Movement_Grunt2Leave:
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

SkyPillar_Outside_Movement_StraitsLeave:
	delay_16
	delay_16
	walk_down
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

SkyPillar_Outside_EventScript_BuzzOff::
	msgbox SkyPillar_Outside_Text_BuzzOff, MSGBOX_DEFAULT
	closemessage
	release
	end

SkyPillar_Outside_Text_BuzzOff:
	.string "Buzz off.$"

SkyPillar_Outside_EventScript_NoPass::
	msgbox SkyPillar_Outside_Text_Nope, MSGBOX_DEFAULT
	closemessage
	release
	end

SkyPillar_Outside_Text_Nope:
	.string "Don't even think about it.$"

