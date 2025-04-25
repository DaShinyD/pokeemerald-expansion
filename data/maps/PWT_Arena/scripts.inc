.set LOCALID_RYE, 12
.set LOCALID_DASH, 13
.set LOCALID_SKY, 14
.set LOCALID_JAKE, 15

PWT_Arena_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, PWT_Arena_OnFrame
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, PWT_Arena_OnWarp
	.byte 0

PWT_Arena_OnWarp:
	map_script_2 VAR_TEMP_1, 0, PWT_Arena_EventScript_PlayerTurnEast
	.2byte 0

PWT_Arena_EventScript_PlayerTurnEast::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_EAST
	end

PWT_Arena_OnFrame:
	map_script_2 VAR_PWT_TYPE, 2, PWT_Arena_EventScript_BeginTourney
	.2byte 0


@ For different tournaments, set a goto_if_set FLAG_NAME_HERE after the line msgbox PWT_Arena_Text_AlrightLetsGetStarted, MSGBOX_AUTOCLOSE. In descending order (FLAG_5, 4, 3, etc)

@ Tourney 1

PWT_Arena_EventScript_BeginTourney::
	lockall
	setvar VAR_TEMP_1, 1
	setvar VAR_PWT_TYPE, 0
	msgbox PWT_Arena_Text_PleaseGo, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, PWT_Arena_Movement_TakeSpot
	waitmovement 0
	msgbox PWT_Arena_Text_LadiesGenlemen, MSGBOX_AUTOCLOSE
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, PWT_Arena_Movement_Announcer
	waitmovement 0
	msgbox PWT_Arena_Text_AnnouncerStuff, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_CAMERA, PWT_Arena_Movement_PanToAudience
	waitmovement 0
	setvar VAR_0x8004, 2  @ vertical pan
	setvar VAR_0x8005, 2  @ horizontal pan
	setvar VAR_0x8006, 5  @ num shakes
	setvar VAR_0x8007, 3  @ shake delay
	special ShakeCamera
	waitstate
	msgbox PWT_Arena_Text_Wooo, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_CAMERA, PWT_Arena_Movement_PanBackToAnnouncer
	waitmovement 0
	msgbox PWT_Arena_Text_AlrightLetsGetStarted, MSGBOX_AUTOCLOSE
	addobject LOCALID_RYE
	applymovement LOCALID_RYE, PWT_Arena_Movement_OppEnter
	waitmovement 0
	msgbox PWT_Arena_Text_FirstBattle, MSGBOX_AUTOCLOSE
	goto PWT_Arena_EventScript_Tourney1FirstBattle
	end

PWT_Arena_Text_PleaseGo:
	.string "Attendant: Please take your place\n"
	.string "on the left in the arena.$"

PWT_Arena_Movement_TakeSpot:
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	face_right
	step_end

PWT_Arena_Text_LadiesGenlemen:
	.string "Laaaadieeeees and Gentlemeeeen.\p"
	.string "If I may have your attention.$"

PWT_Arena_Movement_Announcer:
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_right
	walk_faster_up
	walk_faster_up
	step_end

PWT_Arena_Text_AnnouncerStuff:
	.string "Today we welcome you to the first\n"
	.string "ever Hoenn Pokemon World Tournament.\p"
	.string "It is my great honor to be your\n"
	.string "announcer today.\p"
	.string "Joining us today is our very own\n"
	.string "Hoenn Pokemon League Champion,\l"
	.string "{PLAYER}!\p"
	.string "Hooh boy, the crowd is absolutely\n"
	.string "loving this.\p"
	.string "Just take a look.$"

PWT_Arena_Movement_PanToAudience:
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	walk_faster_up
	step_end

PWT_Arena_Text_Wooo:
	.string "Crowd: WOOOOOOOO!!\p"
	.string "GO {PLAYER} GO!\p"
	.string "GO {PLAYER} GO!$"

PWT_Arena_Movement_PanBackToAnnouncer:
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	walk_faster_down
	step_end

PWT_Arena_Text_AlrightLetsGetStarted:
	.string "The arena here is shaking from all\n"
	.string "the excitement the fans are bringing.\p"
	.string "Let's not disappoint them!\p"
	.string "Without further ado, bring in the\n"
	.string "second competitor.$"

PWT_Arena_Movement_OppEnter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	face_left
	step_end

PWT_Arena_Text_FirstBattle:
	.string "That's right, it's Rye!\p"
	.string "Crowd: WOOOO!\p"
	.string "Rye is another ferocious trainer\n"
	.string "forged in the fire of Hoenn.\p"
	.string "While {PLAYER} is your champ,\n"
	.string "Rye has also beaten the League.\p"
	.string "...\p"
	.string "Folks, it is now time.\p"
	.string "Let us commence the first. ever.\n"
	.string "HOENN WORLD TOURNEY!\p"
	.string "Crowd: WOOOOOO!\p"
	.string "Announcer: BEGIN!\p"
	.string "Rye: Let's make it awesome!$"

PWT_Arena_EventScript_Tourney1FirstBattle:
	trainerbattle_no_intro TRAINER_RYE_GRANITE_CAVE, PWT_Arena_Text_RyeDefeat
	goto PWT_Arena_EventScript_TourneyContinued
	release
	end

PWT_Arena_Text_RyeDefeat:
	.string "Amazing.$"

PWT_Arena_EventScript_TourneyContinued::
	special SpawnCameraObject
	addobject LOCALID_RYE
	msgbox PWT_Arena_Text_Close, MSGBOX_AUTOCLOSE
	applymovement LOCALID_RYE, PWT_Arena_Movement_OppLeave
	waitmovement 0
	removeobject LOCALID_RYE
	addobject LOCALID_JAKE
	applymovement LOCALID_JAKE, PWT_Arena_Movement_OppEnter
	waitmovement 0
	msgbox PWT_Arena_Text_Round2, MSGBOX_AUTOCLOSE
	special HealPlayerParty
	goto PWT_Arena_EventScript_Tourney1SecondBattle
	end

PWT_Arena_Text_Close:
	.string "Announcer: What a fantastic match!\p"
	.string "Our contender Rye will take his\n"
	.string "leave today, but don't count him\l"
	.string "out for good!$"

PWT_Arena_Movement_OppLeave:
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

PWT_Arena_Text_Round2:
	.string "Please welcome our next 2nd round\n"
	.string "contestant, Jake!!\p"
	.string "Crowd: Alright!!\p"
	.string "A champion versus a Master Trainer.\p"
	.string "My blood is boiling with excitement.\p"
	.string "Trainers, LETS GET STARTED.\p"
	.string "Jake: Champ or Master, let's see\n"
	.string "which wins.$"

PWT_Arena_EventScript_Tourney1SecondBattle::
	trainerbattle_no_intro TRAINER_JAKE, PWT_Arena_Text_JakeDefeat
	goto PWT_Arena_EventScript_TourneyContinued2
	end

PWT_Arena_Text_JakeDefeat:
	.string "Umbreon, no!$"

PWT_Arena_EventScript_TourneyContinued2::
	special SpawnCameraObject
	addobject LOCALID_JAKE
	msgbox PWT_Arena_Text_Another, MSGBOX_AUTOCLOSE
	applymovement LOCALID_JAKE, PWT_Arena_Movement_OppLeave
	waitmovement 0
	removeobject LOCALID_JAKE
	addobject LOCALID_SKY
	applymovement LOCALID_SKY, PWT_Arena_Movement_OppEnter
	waitmovement 0
	msgbox PWT_Arena_Text_Round3, MSGBOX_AUTOCLOSE
	special HealPlayerParty
	goto PWT_Arena_EventScript_Tourney1ThirdBattle
	end

PWT_Arena_Text_Another:
	.string "And another one falls to the Hoenn\n"
	.string "Champion!\p"
	.string "Jake will be taking his leave.$"

PWT_Arena_Text_Round3:
	.string "Another Master Trainer enters, Sky!\p"
	.string "Crowd: Yea, Sky!\p"
	.string "These 2 trainers will be fighting\n"
	.string "for the right to the final match.\p"
	.string "Let's see who's will is stronger.\p"
	.string "Trainers, enter battle!\p"
	.string "Sky: Hmph, time to lose.$"

PWT_Arena_EventScript_Tourney1ThirdBattle:
	trainerbattle_no_intro TRAINER_SKY, PWT_Arena_Text_SkyDefeat
	goto PWT_Arena_EventScript_TourneyContinued3
	end

PWT_Arena_Text_SkyDefeat:
	.string "Bugged, bugged I tell you!$"

PWT_Arena_EventScript_TourneyContinued3::
	special SpawnCameraObject
	addobject LOCALID_SKY
	msgbox PWT_Arena_Text_Finals, MSGBOX_AUTOCLOSE
	applymovement LOCALID_SKY, PWT_Arena_Movement_OppLeave
	waitmovement 0
	removeobject LOCALID_SKY
	addobject LOCALID_DASH
	applymovement LOCALID_DASH, PWT_Arena_Movement_OppEnter
	waitmovement 0
	msgbox PWT_Arena_Text_LastBattle, MSGBOX_AUTOCLOSE
	special HealPlayerParty
	goto PWT_Arena_EventScript_Tourney1Final
	end

PWT_Arena_Text_Finals:
	.string "{PLAYER} continues the streak!\p"
	.string "Master Trainer Sky has been removed\n"
	.string "from this competition.\p"
	.string "Congratulations to Sky for the\n"
	.string "top 4 finish.$"

PWT_Arena_Text_LastBattle:
	.string "Now, for what everyone has been\n"
	.string "waiting for.\p"
	.string "The final match, the grand ending.\p"
	.string "{PLAYER} vs Dash!\p"
	.string "Crowd: YEAAAA. WOOOOO.\p"
	.string "These 2, along with Rye, traveled\n"
	.string "this region together.\p"
	.string "Rivals, growing together.\p"
	.string "What a fitting end to our very\n"
	.string "first World Tournament.\p"
	.string "You can't ask for better folks.\p"
	.string "Let me remove myself, so that these\n"
	.string "2 unbelievable trainers may end\l"
	.string "this amazing showing.\p"
	.string "IT'S TIME FOR THE FINALE.\p"
	.string "Crowd: ALRIGHT YEA WOOOO!\p"
	.string "Dash: No matter what, let's give\n"
	.string "them a good show.$"

PWT_Arena_EventScript_Tourney1Final::
	trainerbattle_no_intro TRAINER_DASH_102, PWT_Arena_Text_DashDefeat
	goto PWT_Arena_EventScript_Tourney1End
	end

PWT_Arena_Text_DashDefeat:
	.string "What an incredible battle.$"

PWT_Arena_EventScript_Tourney1End::
	release
	end

