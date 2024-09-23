.set LOCALID_DASH, 10

Route102_MapScripts::
	.byte 0

Route102_EventScript_LittleBoy::
	msgbox Route102_Text_ImNotVeryTall, MSGBOX_NPC
	end

Route102_EventScript_RouteSignOldale::
	msgbox Route102_Text_RouteSignOldale, MSGBOX_SIGN
	end

Route102_EventScript_RouteSignPetalburg::
	msgbox Route102_Text_RouteSignPetalburg, MSGBOX_SIGN
	end

Route102_EventScript_Boy::
	msgbox Route102_Text_CatchWholeBunchOfPokemon, MSGBOX_NPC
	end

Route102_EventScript_Calvin::
	trainerbattle_single TRAINER_CALVIN_1, Route102_Text_CalvinIntro, Route102_Text_CalvinDefeated, Route102_EventScript_CalvinRegisterMatchCallAfterBattle
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route102_EventScript_CalvinRematch
	setvar VAR_0x8004, TRAINER_CALVIN_1
	specialvar VAR_RESULT, IsTrainerRegistered
	goto_if_eq VAR_RESULT, FALSE, Route102_EventScript_CalvinTryRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegisterMatchCallAfterBattle::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegisterMatchCall
	release
	end

Route102_EventScript_CalvinRegisterMatchCall::
	msgbox Route102_Text_CalvinRegisterShort, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinTryRegister::
	goto_if_set FLAG_HAS_MATCH_CALL, Route102_EventScript_CalvinRegister
	msgbox Route102_Text_CalvinPostBattle, MSGBOX_DEFAULT
	release
	end

Route102_EventScript_CalvinRegister::
	msgbox Route102_Text_CalvinRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_CALVIN_1
	release
	end

Route102_EventScript_CalvinRematch::
	trainerbattle_rematch TRAINER_CALVIN_1, Route102_Text_CalvinRematchIntro, Route102_Text_CalvinRematchDefeated
	msgbox Route102_Text_CalvinRematchPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Rick::
	trainerbattle_single TRAINER_RICK, Route102_Text_RickIntro, Route102_Text_RickDefeated
	msgbox Route102_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Tiana::
	trainerbattle_single TRAINER_TIANA, Route102_Text_TianaIntro, Route102_Text_TianaDefeated
	msgbox Route102_Text_TianaPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_EventScript_Allen::
	trainerbattle_single TRAINER_ALLEN, Route102_Text_AllenIntro, Route102_Text_AllenDefeated
	msgbox Route102_Text_AllenPostBattle, MSGBOX_AUTOCLOSE
	end

Route102_Text_WatchMeCatchPokemon:
	.string "WALLY: {PLAYER}…\n"
	.string "POKéMON hide in tall grass like this,\l"
	.string "don't they?\p"
	.string "Please watch me and see if I can\n"
	.string "catch one properly.\p"
	.string "…Whoa!$"

Route102_Text_WallyIDidIt:
	.string "WALLY: I did it… It's my…\n"
	.string "My POKéMON!$"

Route102_Text_LetsGoBack:
	.string "{PLAYER}, thank you!\n"
	.string "Let's go back to the GYM!$"

Route102_Text_ImNotVeryTall:
	.string "I'm…not very tall, so I sink right\n"
	.string "into tall grass.\p"
	.string "The grass goes up my nose and…\n"
	.string "Fwafwafwafwafwa…\p"
	.string "Fwatchoo!$"

Route102_Text_CatchWholeBunchOfPokemon:
	.string "I'm going to catch a whole bunch of\n"
	.string "POKéMON!$"

Route102_Text_RouteSignOldale:
	.string "ROUTE 102\n"
	.string "{RIGHT_ARROW} OLDALE TOWN$"

Route102_Text_RouteSignPetalburg:
	.string "ROUTE 102\n"
	.string "{LEFT_ARROW} PETALBURG CITY$"

@begin my new code

Route102_EventScript_DashTrigger1::
	lock
	playse SE_PIN
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	applymovement LOCALID_DASH, Route102_Movement_DashNoticesPlayer
	waitmovement 0
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	applymovement LOCALID_DASH, Route102_Movement_DashApproachesPlayer1
	waitmovement 0
	msgbox Route102_Text_DashGreet, MSGBOX_DEFAULT
	waitmessage
	goto Route102_EventScript_DashBattle1
	end

Route102_EventScript_DashBattle1::
	trainerbattle_no_intro TRAINER_DASH_102, Route102_Text_DashDefeat
	goto Route102_EventScript_DashLeave1
	end

Route102_EventScript_DashLeave1::
	msgbox Route102_Text_DashGoodbye, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DASH, Route102_Movement_DashLeaves1
	waitmovement 0
	removeobject LOCALID_DASH
	fadedefaultbgm
	setflag FLAG_HIDE_DASH102
	setvar VAR_DASH_ROUTE102, 1
	release
	end

Route102_EventScript_DashTrigger2::
	lock
	playse SE_PIN
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	applymovement LOCALID_DASH, Route102_Movement_DashNoticesPlayer
	waitmovement 0
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	applymovement LOCALID_DASH, Route102_Movement_DashApproachesPlayer2
	waitmovement 0
	msgbox Route102_Text_DashGreet, MSGBOX_DEFAULT
	waitmessage
	goto Route102_EventScript_DashBattle2
	end

Route102_EventScript_DashBattle2::
	trainerbattle_no_intro TRAINER_DASH_102, Route102_Text_DashDefeat
	goto Route102_EventScript_DashLeave2
	end

Route102_EventScript_DashLeave2::
	msgbox Route102_Text_DashGoodbye, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DASH, Route102_Movement_DashLeaves2
	waitmovement 0
	removeobject LOCALID_DASH
	fadedefaultbgm
	setflag FLAG_HIDE_DASH102
	setvar VAR_DASH_ROUTE102, 1
	release
	end

Route102_EventScript_DashTrigger3::
	lock
	playse SE_PIN
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	applymovement LOCALID_DASH, Route102_Movement_DashNoticesPlayer
	waitmovement 0
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	applymovement LOCALID_DASH, Route102_Movement_DashApproachesPlayer3
	waitmovement 0
	msgbox Route102_Text_DashGreet, MSGBOX_DEFAULT
	waitmessage
	goto Route102_EventScript_DashBattle3
	end

Route102_EventScript_DashBattle3::
	trainerbattle_no_intro TRAINER_DASH_102, Route102_Text_DashDefeat
	goto Route102_EventScript_DashLeave3
	end

Route102_EventScript_DashLeave3::
	msgbox Route102_Text_DashGoodbye, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DASH, Route102_Movement_DashLeaves3
	waitmovement 0
	removeobject LOCALID_DASH
	fadedefaultbgm
	setflag FLAG_HIDE_DASH102
	setvar VAR_DASH_ROUTE102, 1
	release
	end

Route102_EventScript_DashTrigger4::
	lock
	playse SE_PIN
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	applymovement LOCALID_DASH, Route102_Movement_DashNoticesPlayer
	waitmovement 0
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	applymovement LOCALID_DASH, Route102_Movement_DashApproachesPlayer4
	waitmovement 0
	msgbox Route102_Text_DashGreet, MSGBOX_DEFAULT
	waitmessage
	goto Route102_EventScript_DashBattle4
	end

Route102_EventScript_DashBattle4::
	trainerbattle_no_intro TRAINER_DASH_102, Route102_Text_DashDefeat
	goto Route102_EventScript_DashLeave4
	end

Route102_EventScript_DashLeave4::
	msgbox Route102_Text_DashGoodbye, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DASH, Route102_Movement_DashLeaves4
	waitmovement 0
	removeobject LOCALID_DASH
	fadedefaultbgm
	setflag FLAG_HIDE_DASH102
	setvar VAR_DASH_ROUTE102, 1
	release
	end

Route102_Text_DashGreet:
	.string "Yo! You look like a new\n"
	.string "trainer. My name is Dash.\p"
	.string "You are?\p"
	.string "{PLAYER}, huh? Nice to\n"
	.string "meet you.\p"
	.string "How would you like to be my\n"
	.string "rival?\p"
	.string "I'll show you my strength with\n"
	.string "a battle.$"

Route102_Text_DashIntro:
	.string "Go, Beedrill!$"

Route102_Text_DashDefeat:
	.string "Aw man.$"

Route102_Text_DashGoodbye:
	.string "That was a great battle. We\n"
	.string "definitely have to do it\l"
	.string "again.\p"
	.string "I'll be seeing you around.\p"
	.string "For now, I've got some\n"
	.string "gyms to beat.\p"
	.string "Bye!$"

Route102_Movement_DashNoticesPlayer:
	face_right
	step_end

Route102_Movement_DashApproachesPlayer1:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	face_right
	step_end

Route102_Movement_DashApproachesPlayer2:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	face_right
	step_end

Route102_Movement_DashApproachesPlayer3:
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Route102_Movement_DashApproachesPlayer4:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	face_right
	step_end

Route102_Movement_DashLeaves1:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route102_Movement_DashLeaves2:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route102_Movement_DashLeaves3:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route102_Movement_DashLeaves4:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

