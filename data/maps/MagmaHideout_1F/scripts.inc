.set LOCALID_ROXANNE, 4
.set LOCALID_FLANNERY, 5
.set LOCALID_NORMAN, 6
.set LOCALID_BRAWLY, 7
.set LOCALID_DASH, 8
.set LOCALID_RYE, 9

MagmaHideout_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, MagmaHideout_1F_OnTransition
	.byte 0

MagmaHideout_1F_OnTransition:
	setvar VAR_JAGGED_PASS_ASH_WEATHER, 0
	setflag FLAG_TEMP_HIDE_FOLLOWER
	end

MagmaHideout_1F_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_1, MagmaHideout_1F_Text_Grunt1Intro, MagmaHideout_1F_Text_Grunt1Defeat
	msgbox MagmaHideout_1F_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_MAGMA_HIDEOUT_2, MagmaHideout_1F_Text_Grunt2Intro, MagmaHideout_1F_Text_Grunt2Defeat
	msgbox MagmaHideout_1F_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

MagmaHideout_1F_Text_Grunt1Intro:
	.string "When TEAM MAGMA has roll call, we get\n"
	.string "important guarding assignments in\l"
	.string "the order that we line up.\p"
	.string "That's why I'm stuck off in this corner.\n"
	.string "I'm always late to roll call!$"

MagmaHideout_1F_Text_Grunt1Defeat:
	.string "I'm always late for training sessions,\n"
	.string "too!\p"
	.string "I hate to say it, but I'm wimpy…$"

MagmaHideout_1F_Text_Grunt1PostBattle:
	.string "Okay, I'll try to put a little more\n"
	.string "effort into things from now on…$"

MagmaHideout_1F_Text_Grunt2Intro:
	.string "Our leader told us to dig into\n"
	.string "MT. CHIMNEY, so we dug and dug.\p"
	.string "And in the course of digging, we came\n"
	.string "across something that blew our minds!\p"
	.string "What did we find?\p"
	.string "Fuhahaha!\n"
	.string "I'll tell you if you beat me!$"

MagmaHideout_1F_Text_Grunt2Defeat:
	.string "Arrgh!\n"
	.string "Taken down!$"

MagmaHideout_1F_Text_Grunt2PostBattle:
	.string "I won't tell you after all.\n"
	.string "You'll find out when you get there!\p"
	.string "It'd be better if you saved surprises\n"
	.string "to the end, don't you think?$"




@ My stuff

MagmaHideout_1F_EventScript_CallRivals::
	lock
	msgbox MagmaHideout_1F_Text_ShouldCall, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	addobject LOCALID_RYE
	addobject LOCALID_DASH
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceDown
	msgbox MagmaHideout_1F_Text_RingRingHello, MSGBOX_AUTOCLOSE
	fadescreen FADE_FROM_BLACK
	msgbox MagmaHideout_1F_Text_HeyThereFromRivals, MSGBOX_DEFAULT
	applymovement LOCALID_RYE, MagmaHideout_1F_Movement_RyePaceForCall
	waitmovement 0
	msgbox MagmaHideout_1F_Text_OkDoneRyeCall, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	addobject LOCALID_BRAWLY
	addobject LOCALID_ROXANNE
	addobject LOCALID_FLANNERY
	addobject LOCALID_NORMAN
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_RYE, MagmaHideout_1F_Movement_RyeAllHereLook
	waitmovement 0
	msgbox MagmaHideout_1F_Text_RyeAllHere, MSGBOX_DEFAULT
	applymovement LOCALID_NORMAN, MagmaHideout_1F_Movement_NormanApproachEx
	waitmovement 0
	msgbox MagmaHideout_1F_Text_NormanExcuseMe, MSGBOX_AUTOCLOSE
	applymovement LOCALID_RYE, MagmaHideout_1F_Movement_RyeDashMoveOutOfWay
	applymovement LOCALID_DASH, MagmaHideout_1F_Movement_RyeDashMoveOutOfWay
	waitmovement 0
	applymovement LOCALID_NORMAN, MagmaHideout_1F_Movement_NormanApproachFinish
	waitmovement 0
	msgbox MagmaHideout_1F_Text_DaddySayHi, MSGBOX_DEFAULT
	applymovement LOCALID_DASH, MagmaHideout_1F_Movement_DashLineUp
	applymovement LOCALID_RYE, MagmaHideout_1F_Movement_RyeLineUp
	waitmovement 0
	applymovement LOCALID_FLANNERY, MagmaHideout_1F_Movement_FlanneryLineUp
	applymovement LOCALID_ROXANNE, MagmaHideout_1F_Movement_RoxanneLineUp
	applymovement LOCALID_BRAWLY, MagmaHideout_1F_Movement_BrawlyLineUp
	waitmovement 0
	msgbox MagmaHideout_1F_Text_AllGoodToGo, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, MagmaHideout_1F_Movement_LeadTheGroup
	applymovement LOCALID_NORMAN, MagmaHideout_1F_Movement_NormanFollow
	applymovement LOCALID_DASH, MagmaHideout_1F_Movement_DashFollow
	applymovement LOCALID_RYE, MagmaHideout_1F_Movement_RyeFollow
	applymovement LOCALID_FLANNERY, MagmaHideout_1F_Movement_FlanneryFollow
	applymovement LOCALID_ROXANNE, MagmaHideout_1F_Movement_RoxanneFollow
	applymovement LOCALID_BRAWLY, MagmaHideout_1F_Movement_BrawlyFollow
	waitmovement 0
	msgbox MagmaHideout_1F_Text_DadWeWillGoAhead, MSGBOX_DEFAULT
	applymovement LOCALID_NORMAN, MagmaHideout_1F_Movement_NormanGoAhead
	applymovement LOCALID_DASH, MagmaHideout_1F_Movement_DashGoAhead
	applymovement LOCALID_RYE, MagmaHideout_1F_Movement_RyeGoAhead
	applymovement LOCALID_FLANNERY, MagmaHideout_1F_Movement_FlanneryGoAhead
	applymovement LOCALID_ROXANNE, MagmaHideout_1F_Movement_RoxanneGoAhead
	applymovement LOCALID_BRAWLY, MagmaHideout_1F_Movement_BrawlyGoAhead
	waitmovement 0
	removeobject LOCALID_NORMAN
	removeobject LOCALID_DASH
	removeobject LOCALID_RYE
	removeobject LOCALID_FLANNERY
	removeobject LOCALID_ROXANNE
	removeobject LOCALID_BRAWLY
	setvar VAR_M_HIDEOUT_HELP, 1
	release
	end

@ Messages

MagmaHideout_1F_Text_ShouldCall:
	.string "I should call Dash and Rye\n"
	.string "this time.$"

MagmaHideout_1F_Text_RingRingHello:
	.string "Ring...\n"
	.string "Ring...\l"
	.string "Ring...\p"
	.string "Dash: Hello?\n"
	.string "Ah, {PLAYER}.\p"
	.string "Wait...if you're calling, what's\n"
	.string "going on?\p"
	.string "...\n"
	.string "...\p"
	.string "Team Magma's Hideout...\p"
	.string "Okay, got it, we'll be there\n"
	.string "soon.$"

MagmaHideout_1F_Text_HeyThereFromRivals:
	.string "Dash: Hey, we got here as soon\n"
	.string "as we could.\p"
	.string "Rye: So this is their hideout.\p"
	.string "I wonder if Team Aqua has one?\p"
	.string "Dash: For now let's focus on\n"
	.string "the hideout we are in.\p"
	.string "Rye: We're gonna need more than\n"
	.string "just us for this. Let me make\l"
	.string "a call.$"

MagmaHideout_1F_Text_OkDoneRyeCall:
	.string "Rye: Okay, it's done.\p"
	.string "They should be here soon.\p"
	.string "Let's wait a bit.$"

MagmaHideout_1F_Text_RyeAllHere:
	.string "Looks like everyone is here.$"

MagmaHideout_1F_Text_NormanExcuseMe:
	.string "Norman: Excuse me.$"

MagmaHideout_1F_Text_DaddySayHi:
	.string "Dad: Hey kid, you're growing\n"
	.string "up quick, huh?\p"
	.string "It hasn't been long since you\n"
	.string "beat me, but I can tell you're\l"
	.string "already stronger.\p"
	.string "Anyway, this is the base of\n"
	.string "these Team Magma criminals\l"
	.string "I've heard about on the news.\p"
	.string "I wonder what their goals could\n"
	.string "even be...\p"
	.string "Anyway, how about we show these\n"
	.string "guys that they can't just run\l"
	.string "around our region.\p"
	.string "Everyone, LINE UP.$"

MagmaHideout_1F_Text_AllGoodToGo:
	.string "Dad: Everyone is ready.\p"
	.string "Lead the way, champ.$"

MagmaHideout_1F_Text_DadWeWillGoAhead:
	.string "{PLAYER}, we will take the lead\n"
	.string "from here.\p"
	.string "As the strongest of us, focus\n"
	.string "on getting to the end.\p"
	.string "Good luck, champ.$"

@ My movements

MagmaHideout_1F_Movement_RyePaceForCall:
	walk_down
	walk_up
	walk_down
	walk_up
	step_end

MagmaHideout_1F_Movement_RyeAllHereLook:
	face_down
	delay_8
	face_up
	step_end

MagmaHideout_1F_Movement_NormanApproachEx:
	walk_up
	walk_up
	step_end

MagmaHideout_1F_Movement_RyeDashMoveOutOfWay:
	walk_left
	face_right
	step_end

MagmaHideout_1F_Movement_NormanApproachFinish:
	walk_up
	step_end

MagmaHideout_1F_Movement_DashLineUp:
	walk_down
	walk_right
	face_up
	step_end

MagmaHideout_1F_Movement_RyeLineUp:
	walk_right
	walk_down
	walk_down
	walk_right
	face_up
	step_end

MagmaHideout_1F_Movement_FlanneryLineUp:
	walk_right
	face_up
	step_end

MagmaHideout_1F_Movement_RoxanneLineUp:
	walk_right
	walk_down
	walk_right
	face_up
	step_end

MagmaHideout_1F_Movement_BrawlyLineUp:
	walk_right
	walk_right
	walk_down
	walk_down
	walk_right
	face_up
	step_end

MagmaHideout_1F_Movement_LeadTheGroup:
	walk_up
	walk_up
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
	face_right
	step_end

MagmaHideout_1F_Movement_NormanFollow:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MagmaHideout_1F_Movement_DashFollow:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MagmaHideout_1F_Movement_RyeFollow:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

MagmaHideout_1F_Movement_FlanneryFollow:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	step_end

MagmaHideout_1F_Movement_RoxanneFollow:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	step_end

MagmaHideout_1F_Movement_BrawlyFollow:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

MagmaHideout_1F_Movement_NormanGoAhead:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MagmaHideout_1F_Movement_DashGoAhead:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MagmaHideout_1F_Movement_RyeGoAhead:
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MagmaHideout_1F_Movement_FlanneryGoAhead:
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MagmaHideout_1F_Movement_RoxanneGoAhead:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

MagmaHideout_1F_Movement_BrawlyGoAhead:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

