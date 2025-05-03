.set LOCALID_STRAITS, 1
.set LOCALID_RAYQUAZA, 2
.set LOCALID_GRUNT_1, 7
.set LOCALID_GRUNT_2, 8
.set LOCALID_GRUNT_3, 9
.set LOCALID_GRUNT_4, 10
.set LOCALID_DASH, 11
.set LOCALID_RYE, 12
.set LOCALID_ADMIN_1, 13
.set LOCALID_ADMIN_2, 14
.set LOCALID_WALLY, 15
.set LOCALID_DEOXYS, 17
.set LOCALID_PALKIA, 18
.set LOCALID_DIALGA, 19

Route136_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route136_OnLoad
	.byte 0

Route136_OnLoad:
	call_if_set FLAG_ROUTE136_CAVE, Route136_EventScript_PlaceCaveEntrance
	end

Route136_EventScript_Grunt1::
	trainerbattle_single TRAINER_STRATA_MOUNTAIN_1, Route136_Text_GruntIntro1, Route136_Text_GruntDefeat1
	msgbox Route136_Text_GruntAfter1, MSGBOX_AUTOCLOSE
	end

Route136_EventScript_Grunt2::
	trainerbattle_single TRAINER_STRATA_MOUNTAIN_2, Route136_Text_GruntIntro2, Route136_Text_GruntDefeat2
	msgbox Route136_Text_GruntAfter2, MSGBOX_AUTOCLOSE
	end

Route136_EventScript_Grunt3::
	trainerbattle_single TRAINER_STRATA_MOUNTAIN_3, Route136_Text_GruntIntro3, Route136_Text_GruntDefeat3
	msgbox Route136_Text_GruntAfter3, MSGBOX_AUTOCLOSE
	end

Route136_EventScript_Grunt4::
	trainerbattle_single TRAINER_STRATA_MOUNTAIN_4, Route136_Text_GruntIntro4, Route136_Text_GruntDefeat4
	msgbox Route136_Text_GruntAfter4, MSGBOX_AUTOCLOSE
	end

Route136_EventScript_Grunt5::
	trainerbattle_single TRAINER_STRATA_MOUNTAIN_5, Route136_Text_GruntIntro5, Route136_Text_GruntDefeat5
	msgbox Route136_Text_GruntAfter5, MSGBOX_AUTOCLOSE
	end

Route136_Text_GruntIntro1:
	.string "I've found a rat.$"

Route136_Text_GruntIntro2:
	.string "You won't pass me.$"

Route136_Text_GruntIntro3:
	.string "We will be victorious.$"

Route136_Text_GruntIntro4:
	.string "How did you make it here?$"

Route136_Text_GruntIntro5:
	.string "Good luck going any further.$"

Route136_Text_GruntDefeat1:
	.string "Rat's got hands.$"

Route136_Text_GruntDefeat2:
	.string "Passed up and hit by the baton.$"

Route136_Text_GruntDefeat3:
	.string "No way.$"

Route136_Text_GruntDefeat4:
	.string "That's how...$"

Route136_Text_GruntDefeat5:
	.string "I shouldn't have wished you luck.$"

Route136_Text_GruntAfter1:
	.string "Straits' will must be completed.$"

Route136_Text_GruntAfter2:
	.string "We will still win.$"

Route136_Text_GruntAfter3:
	.string "My failure changes nothing.$"

Route136_Text_GruntAfter4:
	.string "Even with our megas...$"

Route136_Text_GruntAfter5:
	.string "Luck will only get you so far.$"

Route136_EventScript_PreFinale::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_PlayerEnter
	waitmovement 0
	applymovement LOCALID_GRUNT_1, Route136_Movement_GruntsApproach
	applymovement LOCALID_GRUNT_2, Route136_Movement_GruntsApproach
	applymovement LOCALID_GRUNT_3, Route136_Movement_GruntsApproach
	applymovement LOCALID_GRUNT_4, Route136_Movement_GruntsApproach
	waitmovement 0
	msgbox Route136_Text_Grunts, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GRUNT_1, Common_Movement_ExclamationMark
	applymovement LOCALID_GRUNT_2, Common_Movement_ExclamationMark
	applymovement LOCALID_GRUNT_3, Common_Movement_ExclamationMark
	applymovement LOCALID_GRUNT_4, Common_Movement_ExclamationMark
	waitmovement 0
	addobject LOCALID_RYE
	addobject LOCALID_DASH
	applymovement LOCALID_DASH, Route136_Movement_RivalsApproach
	applymovement LOCALID_RYE, Route136_Movement_RivalsApproach
	waitmovement 0
	msgbox Route136_Text_WeWillHandle, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_Progress
	waitmovement 0
	msgbox Route136_Text_AdminsNoGo, MSGBOX_DEFAULT
	closemessage
	addobject LOCALID_WALLY
	clearflag FLAG_HIDE_WALLY_STRATA
	applymovement LOCALID_WALLY, Route136_Movement_WallyApproach
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_PlayerFaceWally
	waitmovement 0
	msgbox Route136_Text_WallyHiHeal, MSGBOX_DEFAULT
	closemessage
	special HealPlayerParty
	msgbox Route136_Text_LetsDoThis, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_PlayerAdmin1
	applymovement LOCALID_WALLY, Route136_Movement_Admin2
	waitmovement 0
	goto Route136_EventScript_AdminsPrep
	end

Route136_EventScript_Rye::
	msgbox Route136_Text_RyeTalk, MSGBOX_NPC
	end

Route136_EventScript_Dash::
	msgbox Route136_Text_DashTalk, MSGBOX_NPC
	end

Route136_EventScript_Wally::
	msgbox Route136_Text_WallyTalk, MSGBOX_NPC
	end

Route136_Text_RyeTalk:
	.string "We took care of those grunts.\p"
	.string "Go on, win!$"

Route136_Text_DashTalk:
	.string "Those guys were no sweat.\p"
	.string "Save Hoenn!$"

Route136_Text_WallyTalk:
	.string "You can win, {PLAYER}!$"

Route136_Movement_PlayerEnter:
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Route136_Movement_GruntsApproach:
	walk_down
	step_end

Route136_Text_Grunts:
	.string "All: Heh, us four will stop you\n"
	.string "right here.\p"
	.string "No more hero work.\p"
	.string "Just try and take us all.\p"
	.string "Dash & Rye: No you don't!$"

Route136_Movement_RivalsApproach:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Route136_Text_WeWillHandle:
	.string "Dash: We'll take care of these\n"
	.string "idiots.\p"
	.string "Rye: Go on ahead and stop them!$"

Route136_Movement_Progress:
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
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_up
	step_end

Route136_Text_AdminsNoGo:
	.string "Admins: Woah, stop right there.\p"
	.string "There's no way you're getting\n"
	.string "through us.\p"
	.string "Our leader is finally achieving\n"
	.string "his dream, and you won't stop it.\p"
	.string "Your heroism ends here.$"

Route136_Movement_WallyApproach:
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	step_end

Route136_Movement_PlayerFaceWally:
	face_right
	step_end

Route136_Text_WallyHiHeal:
	.string "Wally: {PLAYER}!\p"
	.string "I'm here for you.\p"
	.string "You won't do this alone.\p"
	.string "You've helped me since Petalburg,\n"
	.string "now I can return the favor.\p"
	.string "Let's take these guys on together.\p"
	.string "I'll heal your Pokemon.$"
	
Route136_Text_LetsDoThis:
	.string "Come on, we will save Hoenn!$"

Route136_Movement_PlayerAdmin1:
	walk_left
	face_up
	step_end

Route136_Movement_Admin2:
	face_up
	step_end

Route136_EventScript_AdminsPrep::
	special SavePlayerParty
	fadescreen FADE_TO_BLACK
	special ChooseHalfPartyForBattle
	waitstate
	goto_if_ne VAR_RESULT, 0, Route136_EventScript_AdminsBattle
	special LoadPlayerParty
	goto Route136_EventScript_Huh
	end

Route136_EventScript_Huh::
	msgbox Route136_Text_WeHaveToFight, MSGBOX_DEFAULT
	closemessage
	goto Route136_EventScript_AdminsPrep
	end

Route136_Text_WeHaveToFight:
	.string "Wally: {PLAYER}, we need to do this.$"


Route136_EventScript_AdminsBattle::
	multi_2_vs_2 TRAINER_STRATA_ADMIN_1, Route136_Text_WeFailed, TRAINER_STRATA_ADMIN_2, Route136_Text_StraitsWillWin, PARTNER_WALLY
	switch VAR_RESULT
	case 1, Route136_EventScript_DefeatedAdmins
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate

Route136_Text_WeFailed:
	.string "We failed the leader...$"

Route136_Text_StraitsWillWin:
	.string "Leader Straits will win.$"

Route136_EventScript_DefeatedAdmins::
	msgbox Route136_Text_GoAhead, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_GoToLeave
	waitmovement 0
	applymovement LOCALID_WALLY, Route136_Movement_FacePlayer
	waitmovement 0
	msgbox Route136_Text_HealAgainGo, MSGBOX_DEFAULT
	closemessage
	special HealPlayerParty
	setvar VAR_ROUTE136_STATE, 1
	release
	end

Route136_Text_GoAhead:
	.string "Pfft, fine, you win.\p"
	.string "Go ahead, you won't beat the Leader\n"
	.string "anyway.$"

Route136_Movement_GoToLeave:
	walk_right
	step_end

Route136_Movement_FacePlayer:
	face_left
	step_end

Route136_Text_HealAgainGo:
	.string "Wally: {PLAYER}, I have enough\n"
	.string "items to heal one of us.\p"
	.string "You're strong, really strong.\p"
	.string "You've got this. Go.$"

Route136_EventScript_FinaleM::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_PlayerEnterFinaleM
	waitmovement 0
	msgbox Route136_Text_StraitsSpeech, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STRAITS, Route136_Movement_FacePlayer2
	waitmovement 0
	msgbox Route136_Text_StraitsContinue, MSGBOX_DEFAULT
	closemessage
	goto Route136_EventScript_StraitsBattle
	end

Route136_EventScript_FinaleL::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_PlayerEnterFinaleL
	waitmovement 0
	msgbox Route136_Text_StraitsSpeech, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STRAITS, Route136_Movement_FacePlayer2
	waitmovement 0
	msgbox Route136_Text_StraitsContinue, MSGBOX_DEFAULT
	closemessage
	goto Route136_EventScript_StraitsBattle
	end

Route136_EventScript_FinaleR::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_PlayerEnterFinaleR
	waitmovement 0
	msgbox Route136_Text_StraitsSpeech, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STRAITS, Route136_Movement_FacePlayer2
	waitmovement 0
	msgbox Route136_Text_StraitsContinue, MSGBOX_DEFAULT
	closemessage
	goto Route136_EventScript_StraitsBattle
	end

Route136_Movement_PlayerEnterFinaleM:
	walk_up
	walk_up
	walk_up
	step_end

Route136_Movement_PlayerEnterFinaleL:
	walk_up
	walk_right
	walk_up
	walk_up
	step_end

Route136_Movement_PlayerEnterFinaleR:
	walk_up
	walk_left
	walk_up
	walk_up
	step_end

Route136_Text_StraitsSpeech:
	.string "Rayquaza and Deoxys.\p"
	.string "Together, these Pokemon will unlock\n"
	.string "the potential we ignore.\p"
	.string "I didn't see it fit to tell you\n"
	.string "before, but seeing as you found\l"
	.string "your way here...$"

Route136_Movement_FacePlayer2:
	delay_8
	face_down
	delay_8
	step_end

Route136_Text_StraitsContinue:
	.string "Rayquaza can use mega evolution\n"
	.string "with no stone.\p"
	.string "Deoxys is an incredibly strong\n"
	.string "Pokemon from route136.\p"
	.string "With Rayquaza, I've unlocked the\n"
	.string "secret to mega evolution.\p"
	.string "With Deoxys, the gene modifications\n"
	.string "have created spectacular results.\p"
	.string "We, as humans, limit Pokemon.\p"
	.string "We need them submissive, as we\n"
	.string "fear their power.\p"
	.string "No longer.\p"
	.string "I will power up all Pokemon in the\n"
	.string "world, so that they may rise up.\p"
	.string "Our research is concluded.\p"
	.string "Now, I will combine Rayquaza and\n"
	.string "Deoxys to create an ultimate\l"
	.string "being, capable of greatness.\p"
	.string "When I am done, humans will be the\n"
	.string "ones listening.\p"
	.string "You will not stop me.\p"
	.string "Here and now, the superior Pokemon\n"
	.string "I have created will end you.$"

Route136_EventScript_StraitsBattle:
	trainerbattle_no_intro TRAINER_STRAITS, Route136_Text_StraitsDefeat
	goto Route136_EventScript_StraitsPost
	end

Route136_Text_StraitsDefeat:
	.string "...$"

Route136_EventScript_StraitsPost::
	msgbox Route136_Text_StraitsLoss, MSGBOX_DEFAULT
	closemessage
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER
	waitmoncry
	playmoncry SPECIES_DEOXYS, CRY_MODE_ENCOUNTER
	waitmoncry
	applymovement LOCALID_STRAITS, Common_Movement_ExclamationMark
	waitmovement 0
	delay 8
	applymovement LOCALID_STRAITS, Route136_Movement_FaceRayquazaDeoxys
	waitmovement 0
	msgbox Route136_Text_NoItCantBe, MSGBOX_AUTOCLOSE
	applymovement LOCALID_RAYQUAZA, Route136_Movement_MonsComeCloser
	applymovement LOCALID_DEOXYS, Route136_Movement_MonsComeCloser
	waitmovement 0
	msgbox Route136_Text_NoItCantBe2, MSGBOX_AUTOCLOSE
	applymovement LOCALID_RAYQUAZA, Route136_Movement_MonsComeCloser
	applymovement LOCALID_DEOXYS, Route136_Movement_MonsComeCloser
	waitmovement 0
	playmoncry SPECIES_RAYQUAZA, CRY_MODE_ENCOUNTER
	waitmoncry
	playmoncry SPECIES_DEOXYS, CRY_MODE_ENCOUNTER
	waitmoncry
	fadescreen FADE_TO_BLACK
	delay 8
	msgbox Route136_Text_Nooo, MSGBOX_DEFAULT
	waitbuttonpress
	closemessage
	removeobject LOCALID_STRAITS
	removeobject LOCALID_DEOXYS
	removeobject LOCALID_RAYQUAZA
	setflag FLAG_STRATA_FINALE_HIDE
	setflag FLAG_HIDE_WALLY_STRATA
	setflag FLAG_HIDE_STRATA_SKY_PILLAR
	setflag FLAG_ROUTE136_CAVE
	setvar VAR_ROUTE136_STATE, 2
	setobjectxyperm LOCALID_DASH, 27, 14
	setobjectxyperm LOCALID_RYE, 28, 14
	setobjectxyperm LOCALID_WALLY, 29, 14
	addobject LOCALID_DASH
	addobject LOCALID_RYE
	addobject LOCALID_WALLY
	call Route136_EventScript_PlaceCaveEntrance
	special DrawWholeMapView
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_RYE, Route136_Movement_RivalsComeAfter
	applymovement LOCALID_DASH, Route136_Movement_RivalsComeAfter
	applymovement LOCALID_WALLY, Route136_Movement_RivalsComeAfter
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Route136_Movement_FacePlayer2
	waitmovement 0
	msgbox Route136_Text_RivalsTalk, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, Route136_Movement_RivalsLeaveAfter
	applymovement LOCALID_DASH, Route136_Movement_RivalsLeaveAfter
	applymovement LOCALID_WALLY, Route136_Movement_RivalsLeaveAfter
	waitmovement 0
	removeobject LOCALID_DASH
	removeobject LOCALID_WALLY
	removeobject LOCALID_RYE
	release
	end

Route136_Text_StraitsLoss:
	.string "I..I lost?\p"
	.string "Even with evolutionary perfect\n"
	.string "Pokemon?\p"
	.string "Where, where does that power of\n"
	.string "yours come from?\p"
	.string "My perfect plan...beaten by a\n"
	.string "child, just like Team Aqua\l"
	.string "and Team Magma.\p"
	.string "Perhaps it wasn't meant to be.\p"
	.string "Perhaps, I must reflect...\p"
	.string "Was I, wrong?\p"
	.string "HA, no!\p"
	.string "I can rebuild, I can do it again!$"

Route136_Movement_FaceRayquazaDeoxys:
	face_up
	step_end

Route136_Text_NoItCantBe:
.string "Wait, no!$"

Route136_Movement_MonsComeCloser:
	walk_down
	step_end

Route136_Text_NoItCantBe2:
	.string "No you can't do this!$"

Route136_Text_Nooo:
	.string "NOOOOOOOOO$"

Route136_EventScript_PlaceCaveEntrance:
	setmetatile 23, 7, METATILE_General_RockOnRock_Corner_L, TRUE
	setmetatile 24, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 25, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 26, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 27, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 28, 7, METATILE_General_CaveEntrance_Bottom, FALSE
	setmetatile 29, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 30, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 31, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 32, 7, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 33, 7, METATILE_General_RockOnRock_Corner_R, FALSE
	setmetatile 23, 6, METATILE_General_RockWall_RockSideLeft, TRUE
	setmetatile 24, 6, METATILE_General_RockOnRock_Corner_L, TRUE
	setmetatile 25, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 26, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 27, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 28, 6, METATILE_General_CaveEntrance_Top, TRUE
	setmetatile 29, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 30, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 31, 6, METATILE_General_RockWall_RockBase, TRUE
	setmetatile 32, 6, METATILE_General_RockOnRock_Corner_R, TRUE
	setmetatile 33, 6, METATILE_General_RockWall_RockSideRight, TRUE
	setmetatile 23, 5, METATILE_General_RockOnRock_Corner_LT, TRUE
	setmetatile 24, 5, METATILE_General_RockWall_RockSideLeft, TRUE
	setmetatile 25, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 26, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 27, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 28, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 29, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 30, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 31, 5, METATILE_General_RockMountainPath, TRUE
	setmetatile 32, 5, METATILE_General_RockWall_RockSideRight, TRUE
	setmetatile 33, 5, METATILE_General_RockOnRock_Corner_RT, TRUE
	setmetatile 24, 4, METATILE_General_RockOnRock_Corner_LT, TRUE
	setmetatile 25, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 26, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 27, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 28, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 29, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 30, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 31, 4, METATILE_General_RockWall_RockTop, TRUE
	setmetatile 32, 4, METATILE_General_RockOnRock_Corner_RT, TRUE
	return

Route136_Movement_RivalsComeAfter:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Route136_Text_RivalsTalk:
	.string "All: {PLAYER}!\p"
	.string "Dash: Jesus, did he just...?\p"
	.string "Rye: Well he got his wish, he had\n"
	.string "to submit to Pokemon.\p"
	.string "Wally: That was horrible.\p"
	.string "I'm glad you're okay, {PLAYER}.\p"
	.string "Dash: How many times are you going\n"
	.string "to save this region?\p"
	.string "Rye: The world, even.\p"
	.string "Rye: All of these plans were...\n"
	.string "dangerous, to say the least.\p"
	.string "Wally: Are we gonna talk about...\p"
	.string "Rye: The cave that just appeared?\p"
	.string "Dash: It must've been created when\n"
	.string "Rayquaza and Deoxys used their power.\p"
	.string "Wally: Well whatever it is, nothing\n"
	.string "seems to be happening right now.\p"
	.string "Maybe you can explore it sometime.\p"
	.string "Dash: Well, we're gonna go.\p"
	.string "If you need something to do, I\n"
	.string "heard there's a tournament coming\l"
	.string "to IceFall City.\p"
	.string "It's located above Shoal Cave.\p"
	.string "Rye: Take care, great work as always.\p"
	.string "Wally: I'm glad we could be here.\p"
	.string "All: Goodbye. See you soon!$"

Route136_Movement_RivalsLeaveAfter:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Route136_EventScript_Dialga::
	lock
	faceplayer
	playmoncry SPECIES_DIALGA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_DIALGA, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route136_EventScript_DialgaGone
	release
	end

Route136_EventScript_DialgaGone:
	setflag FLAG_DIALGA
	removeobject LOCALID_DIALGA
	release
	end

Route136_EventScript_Palkia::
	lock
	faceplayer
	playmoncry SPECIES_PALKIA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_PALKIA, 70
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Route136_EventScript_PalkiaGone
	release
	end

Route136_EventScript_PalkiaGone:
	setflag FLAG_PALKIA
	removeobject LOCALID_PALKIA
	release
	end

