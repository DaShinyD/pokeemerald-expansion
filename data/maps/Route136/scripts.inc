.set LOCALID_STRAITS, 1
.set LOCALID_GRUNT_1, 7
.set LOCALID_GRUNT_2, 8
.set LOCALID_GRUNT_3, 9
.set LOCALID_GRUNT_4, 10
.set LOCALID_DASH, 11
.set LOCALID_RYE, 12
.set LOCALID_ADMIN_1, 13
.set LOCALID_ADMIN_2, 14
.set LOCALID_WALLY, 15

Route136_MapScripts::
	.byte 0

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

