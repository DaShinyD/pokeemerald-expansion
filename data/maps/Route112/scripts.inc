.set LOCALID_GRUNT_1, 1
.set LOCALID_GRUNT_2, 6
.set LOCALID_DASH, 15
.set LOCALID_RYE, 16

Route112_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route112_OnTransition
	.byte 0

Route112_OnTransition:
	clearflag FLAG_FORCE_MIRAGE_TOWER_VISIBLE
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	end

Route112_EventScript_Rivals::
	lockall
	applymovement LOCALID_DASH, Route112_Movement_RivalsApproach112
	applymovement LOCALID_RYE, Route112_Movement_RivalsApproach112
	waitmovement 0
	msgbox Route112_Text_CamAndRyeHello112, MSGBOX_DEFAULT
	waitmessage
	goto Route112_EventScript_CamBattle112
	releaseall
	end

Route112_EventScript_CamBattle112::
	trainerbattle_no_intro TRAINER_DASH_ROUTE_112, Route112_Text_DashDefeat112
	goto Route112_EventScript_RyeBattleText112
	releaseall
	end

Route112_EventScript_RyeBattleText112::
	msgbox Route112_Text_RyeTurn112, MSGBOX_DEFAULT
	waitmessage
	call Common_EventScript_OutOfCenterPartyHeal
	goto Route112_EventScript_RyeBattle112
	releaseall
	end

Route112_EventScript_RyeBattle112::
	trainerbattle_no_intro TRAINER_RYE_ROUTE_112, Route112_Text_RyeDefeat112
	goto Route112_EventScript_RivalsLeave112
	releaseall
	end

Route112_EventScript_RivalsLeave112::
	msgbox Route112_Text_RivalsBye112, MSGBOX_DEFAULT
	waitmessage
	applymovement LOCALID_DASH, Route112_Movement_RivalsLeave112
	applymovement LOCALID_RYE, Route112_Movement_RivalsLeave112
	waitmovement 0
	setflag FLAG_HIDE_RIVALS_112
	setvar VAR_RIVALS_112, 1
	removeobject LOCALID_DASH
	removeobject LOCALID_RYE
	releaseall
	end

Route112_Movement_RivalsApproach112:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route112_Text_CamAndRyeHello112:
	.string "Dash: Hey there {PLAYER}!\p"
	.string "Great job in Slateport. You\n"
	.string "showed those Aqua guys who the\l"
	.string "boss is.\p"
	.string "Rye: Listen, up ahead there\n"
	.string "are guys calling themselves\l"
	.string "Team Magma.\p"
	.string "We don't know who they are\n"
	.string "but it can't be anything good.\p"
	.string "Dash: I think something big\n"
	.string "is going on here.\p"
	.string "We will have to keep an eye\n"
	.string "out for anything suspicious.\p"
	.string "Rye: If anything happens, just\n"
	.string "call us. We will come help.\p"
	.string "Here are our numbers.\p"
	.string "Received Dash and Rye's\n"
	.string "numbers!\p"
	.string "Dash: Anyway, show me how much\n"
	.string "stronger you are!$"

Route112_Text_DashDefeat112:
	.string "You're too good!$"

Route112_Text_RyeTurn112:
	.string "You may have beat Dash, but\n"
	.string "now it's my turn.\p"
	.string "I'll heal your pokemon\n"
	.string "for you.$"

Route112_Text_RyeDefeat112:
	.string "Not me too!$"

Route112_Text_RivalsBye112:
	.string "Rye: Ah man, you got us both.\p"
	.string "Dash: Good, my rival should be\n"
	.string "at least this strong.\p"
	.string "Rye: Time for us to get going.\p"
	.string "Dash: Don't hesitate to give\n"
	.string "us a call if anything happens.\p"
	.string "Dash and Rye: See ya!$"

Route112_Movement_RivalsLeave112:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Route112_EventScript_MagmaGrunts::
	lockall
	delay 40
	applymovement LOCALID_GRUNT_1, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 20
	msgbox Route112_Text_LeaderGoingToAwakenThing, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	delay 40
	applymovement LOCALID_GRUNT_2, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 20
	msgbox Route112_Text_YeahWeNeedMeteorite, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	delay 40
	applymovement LOCALID_GRUNT_1, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	delay 20
	msgbox Route112_Text_OhThatsWhyCrewWentToFallarbor, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT_1, Common_Movement_FaceOriginalDirection
	waitmovement 0
	delay 40
	applymovement LOCALID_GRUNT_2, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	delay 20
	msgbox Route112_Text_CantLetAnyonePassUntilTheyreBack, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT_2, Common_Movement_FaceOriginalDirection
	waitmovement 0
	releaseall
	end

Route112_EventScript_MtChimneyCableCarSign::
	msgbox Route112_Text_MtChimneyCableCarSign, MSGBOX_SIGN
	end

Route112_EventScript_MtChimneySign::
	msgbox Route112_Text_MtChimneySign, MSGBOX_SIGN
	end

Route112_EventScript_RouteSignLavaridge::
	msgbox Route112_Text_RouteSignLavaridge, MSGBOX_SIGN
	end

Route112_EventScript_Hiker::
	msgbox Route112_Text_NotEasyToGetBackToLavaridge, MSGBOX_NPC
	end

Route112_EventScript_Brice::
	trainerbattle_single TRAINER_BRICE, Route112_Text_BriceIntro, Route112_Text_BriceDefeat
	msgbox Route112_Text_BricePostBattle, MSGBOX_AUTOCLOSE
	end

Route112_EventScript_Trent::
	trainerbattle_single TRAINER_TRENT_1, Route112_Text_TrentIntro, Route112_Text_TrentDefeat, Route112_EventScript_RegisterTrent
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route112_EventScript_RematchTrent
	msgbox Route112_Text_TrentPostBattle, MSGBOX_DEFAULT
	release
	end

Route112_EventScript_RegisterTrent::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route112_Text_TrentRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_TRENT_1
	release
	end

Route112_EventScript_RematchTrent::
	trainerbattle_rematch TRAINER_TRENT_1, Route112_Text_TrentRematchIntro, Route112_Text_TrentRematchDefeat
	msgbox Route112_Text_TrentRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route112_EventScript_Larry::
	trainerbattle_single TRAINER_LARRY, Route112_Text_LarryIntro, Route112_Text_LarryDefeat
	msgbox Route112_Text_LarryPostBattle, MSGBOX_AUTOCLOSE
	end

Route112_EventScript_Carol::
	trainerbattle_single TRAINER_CAROL, Route112_Text_CarolIntro, Route112_Text_CarolDefeat
	msgbox Route112_Text_CarolPostBattle, MSGBOX_AUTOCLOSE
	end

Route112_EventScript_Bryant::
	trainerbattle_single TRAINER_BRYANT, Route112_Text_BryantIntro, Route112_Text_BryantDefeat
	msgbox Route112_Text_BryantPostBattle, MSGBOX_AUTOCLOSE
	end

Route112_EventScript_Shayla::
	trainerbattle_single TRAINER_SHAYLA, Route112_Text_ShaylaIntro, Route112_Text_ShaylaDefeat
	msgbox Route112_Text_ShaylaPostBattle, MSGBOX_AUTOCLOSE
	end

Route112_Text_LeaderGoingToAwakenThing:
	.string "Hey, man, is our leader really going\n"
	.string "to awaken that thing?$"

Route112_Text_YeahWeNeedMeteorite:
	.string "Sounds like it, yeah. But I heard\n"
	.string "we need a METEORITE to do it.$"

Route112_Text_OhThatsWhyCrewWentToFallarbor:
	.string "Oh, I get it now. That's why the rest\n"
	.string "of the crew went out to FALLARBOR.$"

Route112_Text_CantLetAnyonePassUntilTheyreBack:
	.string "You got it. And until they come back,\n"
	.string "we're not to let anyone pass, right.$"

Route112_Text_NotEasyToGetBackToLavaridge:
	.string "Eh, I'd like to get to MAUVILLE, but if\n"
	.string "I went down these ledges, it'd be no\l"
	.string "easy matter to get back to LAVARIDGE.$"

Route112_Text_MtChimneyCableCarSign:
	.string "MT. CHIMNEY CABLE CAR\n"
	.string "“A short walk {UP_ARROW} way!”$"

Route112_Text_MtChimneySign:
	.string "MT. CHIMNEY\p"
	.string "“For LAVARIDGE TOWN or the summit,\n"
	.string "please take the CABLE CAR.”$"

Route112_Text_RouteSignLavaridge:
	.string "ROUTE 112\n"
	.string "{LEFT_ARROW} LAVARIDGE TOWN$"

