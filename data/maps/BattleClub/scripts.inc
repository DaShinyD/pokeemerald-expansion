.set LOCALID_WELCOME_GUY, 1

BattleClub_MapScripts::
	.byte 0

BattleClub_EventScript_WelcomeToClubL::
	lock
	applymovement LOCALID_WELCOME_GUY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, BattleClub_Movement_WalkToPlayerL
	waitmovement 0
	msgbox BattleClub_Text_WelcomeBattleClub, MSGBOX_DEFAULT
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, BattleClub_Movement_CameraPanToBattlersL
	waitmovement 0
	setobjectxyperm LOCALID_WELCOME_GUY, 3, 11
	special DrawWholeMapView
	msgbox BattleClub_Text_ExplainTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, BattleClub_Movement_CameraPanBackL
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, Common_Movement_FacePlayer
	waitmovement 0
	special RemoveCameraObject
	msgbox BattleClub_Text_ThatsAll, MSGBOX_DEFAULT
	applymovement LOCALID_WELCOME_GUY, BattleClub_Movement_WalkAwayL
	waitmovement 0
	setvar VAR_BATTLE_CLUB_INTRO, 1
	release
	end
	
BattleClub_EventScript_WelcomeToClubR::
	lock
	applymovement LOCALID_WELCOME_GUY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, BattleClub_Movement_WalkToPlayerR
	waitmovement 0
	msgbox BattleClub_Text_WelcomeBattleClub, MSGBOX_DEFAULT
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, BattleClub_Movement_CameraPanToBattlersR
	waitmovement 0
	setobjectxyperm LOCALID_WELCOME_GUY, 4, 11
	special DrawWholeMapView
	msgbox BattleClub_Text_ExplainTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, BattleClub_Movement_CameraPanBackR
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, Common_Movement_FacePlayer
	waitmovement 0
	special RemoveCameraObject
	msgbox BattleClub_Text_ThatsAll, MSGBOX_DEFAULT
	applymovement LOCALID_WELCOME_GUY, BattleClub_Movement_WalkAwayR
	waitmovement 0
	setvar VAR_BATTLE_CLUB_INTRO, 1
	release
	end

BattleClub_Movement_WalkToPlayerL:
	delay_8
	walk_down
	walk_down
	walk_down
	walk_left
	face_down
	step_end

BattleClub_Movement_WalkToPlayerR:
	delay_8
	walk_down
	walk_down
	walk_down
	step_end

BattleClub_Text_WelcomeBattleClub:
	.string "Hello there! Welcome to the Battle Club!\p"
	.string "Here, we train Pokemon to be the best\n"
	.string "they can possibly be.\p"
	.string "Take a look over to the right.$"

BattleClub_Movement_CameraPanToBattlersL:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleClub_Movement_CameraPanToBattlersR:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

BattleClub_Text_ExplainTrainers:
	.string "On the left, you have the HP, ATK, and\n"
	.string "DEF trainers.\p"
	.string "On the right, you have the SPE, SPATK\n"
	.string "and SPDEF trainers.\p"
	.string "Lastly, in the middle is the EXP\n"
	.string "trainer.\p"
	.string "You can battle each trainer\n"
	.string "repeatedly.$"

BattleClub_Movement_CameraPanBackL:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

BattleClub_Movement_CameraPanBackR:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

BattleClub_Text_ThatsAll:
	.string "That's everything. Come talk to me\n"
	.string "for training items.\p"
	.string "Good luck on your training!$"

BattleClub_Movement_WalkAwayL:
	walk_right
	walk_up
	walk_up
	walk_up
	face_right
	step_end

BattleClub_Movement_WalkAwayR:
	walk_up
	walk_up
	walk_up
	face_right
	step_end

BattleClub_EventScript_WelcomeGuyGiveItems::
	lock
	faceplayer
	goto_if_set FLAG_POWER_ITEMS, BattleClub_EventScript_HowAreYou
	msgbox BattleClub_Text_TakeThese, MSGBOX_DEFAULT
	giveitem ITEM_POWER_BRACER
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	giveitem ITEM_POWER_ANKLET
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	giveitem ITEM_POWER_BAND
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	giveitem ITEM_POWER_BELT
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	giveitem ITEM_POWER_WEIGHT
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	giveitem ITEM_POWER_LENS
	goto_if_eq VAR_RESULT, 0, Common_EventScript_ShowBagIsFull
	setflag FLAG_POWER_ITEMS
	release
	end

BattleClub_Text_TakeThese:
	.string "Take these power items!$"

BattleClub_EventScript_HowAreYou::
	lock
	faceplayer
	msgbox BattleClub_Text_TrainingHard, MSGBOX_DEFAULT
	release
	end

BattleClub_Text_TrainingHard:
	.string "Training hard?$"

BattleClub_Text_GenericDefeat:
	.string "Nice one.$"

BattleClub_EventScript_HPTrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleHP, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_HPBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end

BattleClub_EventScript_ATKTrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleATK, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_ATKBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end

BattleClub_EventScript_DEFTrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleDEF, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_DEFBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end

BattleClub_EventScript_SPETrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleSPE, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_SPEBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end

BattleClub_EventScript_SPATKTrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleSPATK, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_SPATKBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end

BattleClub_EventScript_SPDEFTrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleSPDEF, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_SPDEFBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end

BattleClub_EventScript_EXPTrainer::
	lock
	faceplayer
	msgbox BattleClub_Text_WouldLikeToBattleEXP, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BattleClub_EventScript_EXPBattle
	goto_if_eq VAR_RESULT, NO, BattleClub_EventScript_NoBattle
	release
	end


BattleClub_Text_WouldLikeToBattleHP:
	.string "Would you like to train HP EVs?$"

BattleClub_Text_WouldLikeToBattleATK:
	.string "Would you like to train ATK EVs?$"

BattleClub_Text_WouldLikeToBattleDEF:
	.string "Would you like to train DEF EVs?$"

BattleClub_Text_WouldLikeToBattleSPE:
	.string "Would you like to train SPE EVs?$"

BattleClub_Text_WouldLikeToBattleSPATK:
	.string "Would you like to train SPATK EVs?$"

BattleClub_Text_WouldLikeToBattleSPDEF:
	.string "Would you like to train SPDEF EVs?$"

BattleClub_Text_WouldLikeToBattleEXP:
	.string "Would you like to train EXP?\p"
	.string "Please note, this is meant for\n"
	.string "people on the level cap setting.\p"
	.string "Would you still like to continue?$"

BattleClub_EventScript_HPBattle::
	trainerbattle_no_intro TRAINER_HPTRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_ATKBattle::
	trainerbattle_no_intro TRAINER_ATKTRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_DEFBattle::
	trainerbattle_no_intro TRAINER_DEFTRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_SPEBattle::
	trainerbattle_no_intro TRAINER_SPETRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_SPATKBattle::
	trainerbattle_no_intro TRAINER_SPATKTRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_SPDEFBattle::
	trainerbattle_no_intro TRAINER_SPDEFTRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_EXPBattle::
	trainerbattle_no_intro TRAINER_EXPTRAIN, BattleClub_Text_GenericDefeat
	release
	end

BattleClub_EventScript_NoBattle::
	release
	end

BattleClub_EventScript_MoneyMan::
	lock
	faceplayer
	msgbox BattleClub_Text_ImTheMoneyMan, MSGBOX_DEFAULT
	closemessage
	goto BattleClub_EventScript_MoneyManBattle
	end

BattleClub_Text_ImTheMoneyMan:
	.string "Ay I'm the money man!$"

BattleClub_EventScript_MoneyManBattle::
	trainerbattle_no_intro TRAINER_MONEY_MAN, BattleClub_Text_GenericDefeat
	release
	end

