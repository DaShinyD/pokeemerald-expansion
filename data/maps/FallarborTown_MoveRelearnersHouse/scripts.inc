.set LOCALID_MOVE_RELEARNER, 1
.set LOCALID_WELCOME_GUY, 2

FallarborTown_MoveRelearnersHouse_MapScripts::
	.byte 0

FallarborTown_MoveRelearnersHouse_EventScript_MoveRelearner::
	lockall
	applymovement LOCALID_MOVE_RELEARNER, Common_Movement_FacePlayer
	waitmovement 0
	goto_if_set FLAG_TEMP_1, FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMove
	msgbox FallarborTown_MoveRelearnersHouse_Text_ImTheMoveTutor, MSGBOX_DEFAULT
	setflag FLAG_TEMP_1
	goto FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMove
	end

FallarborTown_MoveRelearnersHouse_EventScript_AskTeachMove::
	checkitem ITEM_HEART_SCALE
	goto_if_eq VAR_RESULT, FALSE, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
	msgbox FallarborTown_MoveRelearnersHouse_Text_ThatsAHeartScaleWantMeToTeachMove, MSGBOX_YESNO
	switch VAR_RESULT
	case NO, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	end

FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon::
	msgbox FallarborTown_MoveRelearnersHouse_Text_TutorWhichMon, MSGBOX_DEFAULT
	special ChooseMonForMoveRelearner
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
	special IsSelectedMonEgg
	goto_if_eq VAR_RESULT, TRUE, FallarborTown_MoveRelearnersHouse_EventScript_CantTeachEgg
	goto_if_eq VAR_0x8005, 0, FallarborTown_MoveRelearnersHouse_EventScript_NoMoveToTeachMon
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMove
	end

FallarborTown_MoveRelearnersHouse_EventScript_ChooseMove::
	msgbox FallarborTown_MoveRelearnersHouse_Text_TeachWhichMove, MSGBOX_DEFAULT
	special TeachMoveRelearnerMove
	waitstate
	goto_if_eq VAR_0x8004, 0, FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	msgbox FallarborTown_MoveRelearnersHouse_Text_HandedOverHeartScale, MSGBOX_DEFAULT
	removeitem ITEM_HEART_SCALE
	goto FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale
	end

FallarborTown_MoveRelearnersHouse_EventScript_NoMoveToTeachMon::
	msgbox FallarborTown_MoveRelearnersHouse_Text_DontHaveMoveToTeachPokemon, MSGBOX_DEFAULT
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	end

FallarborTown_MoveRelearnersHouse_EventScript_CantTeachEgg::
	msgbox FallarborTown_MoveRelearnersHouse_Text_CantTeachEgg, MSGBOX_DEFAULT
	goto FallarborTown_MoveRelearnersHouse_EventScript_ChooseMon
	end

FallarborTown_MoveRelearnersHouse_EventScript_ComeBackWithHeartScale::
	msgbox FallarborTown_MoveRelearnersHouse_Text_ComeBackWithHeartScale, MSGBOX_DEFAULT
	releaseall
	end

FallarborTown_MoveRelearnersHouse_Text_ImTheMoveTutor:
	.string "I'm the MOVE TUTOR.\p"
	.string "I know all the moves that POKéMON\n"
	.string "learn--every one of them--and I can\l"
	.string "teach POKéMON those moves.\p"
	.string "I can teach a move to a POKéMON\n"
	.string "of yours if you'd like.\p"
	.string "I'll do it for a HEART SCALE.\n"
	.string "I'm collecting those now.$"

FallarborTown_MoveRelearnersHouse_Text_ThatsAHeartScaleWantMeToTeachMove:
	.string "Oh! That's it! That's an honest to\n"
	.string "goodness HEART SCALE!\p"
	.string "Let me guess, you want me to teach\n"
	.string "a move?$"

FallarborTown_MoveRelearnersHouse_Text_TutorWhichMon:
	.string "Which POKéMON needs tutoring?$"

FallarborTown_MoveRelearnersHouse_Text_TeachWhichMove:
	.string "Which move should I teach?$"

FallarborTown_MoveRelearnersHouse_Text_DontHaveMoveToTeachPokemon:
	.string "Sorry…\p"
	.string "It doesn't appear as if I have any move\n"
	.string "I can teach that POKéMON.$"

FallarborTown_MoveRelearnersHouse_Text_HandedOverHeartScale:
	.string "{PLAYER} handed over one HEART SCALE\n"
	.string "in exchange.$"

FallarborTown_MoveRelearnersHouse_Text_ComeBackWithHeartScale:
	.string "If your POKéMON need to learn a move,\n"
	.string "come back with a HEART SCALE.$"

FallarborTown_MoveRelearnersHouse_Text_CantTeachEgg:
	.string "Hunh? There isn't a single move that\n"
	.string "I can teach an EGG.$"






FallarborTown_MoveRelearnersHouse_EventScript_WelcomeToClubL::
	lock
	applymovement LOCALID_WELCOME_GUY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, FallarborTown_MoveRelearnersHouse_Movement_WalkToPlayerL
	waitmovement 0
	msgbox FallarborTown_MoveRelearnersHouse_Text_WelcomeBattleClub, MSGBOX_DEFAULT
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, FallarborTown_MoveRelearnersHouse_Movement_CameraPanToBattlersL
	waitmovement 0
	setobjectxyperm LOCALID_WELCOME_GUY, 3, 11
	special DrawWholeMapView
	msgbox FallarborTown_MoveRelearnersHouse_Text_ExplainTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, FallarborTown_MoveRelearnersHouse_Movement_CameraPanBackL
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, Common_Movement_FacePlayer
	waitmovement 0
	special RemoveCameraObject
	msgbox FallarborTown_MoveRelearnersHouse_Text_ThatsAll, MSGBOX_DEFAULT
	applymovement LOCALID_WELCOME_GUY, FallarborTown_MoveRelearnersHouse_Movement_WalkAwayL
	waitmovement 0
	setvar VAR_BATTLE_CLUB_INTRO, 1
	release
	end
	
FallarborTown_MoveRelearnersHouse_EventScript_WelcomeToClubR::
	lock
	applymovement LOCALID_WELCOME_GUY, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, FallarborTown_MoveRelearnersHouse_Movement_WalkToPlayerR
	waitmovement 0
	msgbox FallarborTown_MoveRelearnersHouse_Text_WelcomeBattleClub, MSGBOX_DEFAULT
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, FallarborTown_MoveRelearnersHouse_Movement_CameraPanToBattlersR
	waitmovement 0
	setobjectxyperm LOCALID_WELCOME_GUY, 4, 11
	special DrawWholeMapView
	msgbox FallarborTown_MoveRelearnersHouse_Text_ExplainTrainers, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_CAMERA, FallarborTown_MoveRelearnersHouse_Movement_CameraPanBackR
	waitmovement 0
	applymovement LOCALID_WELCOME_GUY, Common_Movement_FacePlayer
	waitmovement 0
	special RemoveCameraObject
	msgbox FallarborTown_MoveRelearnersHouse_Text_ThatsAll, MSGBOX_DEFAULT
	applymovement LOCALID_WELCOME_GUY, FallarborTown_MoveRelearnersHouse_Movement_WalkAwayR
	waitmovement 0
	setvar VAR_BATTLE_CLUB_INTRO, 1
	release
	end

FallarborTown_MoveRelearnersHouse_Movement_WalkToPlayerL:
	delay_8
	walk_down
	walk_down
	walk_down
	walk_left
	face_down
	step_end

FallarborTown_MoveRelearnersHouse_Movement_WalkToPlayerR:
	delay_8
	walk_down
	walk_down
	walk_down
	step_end

FallarborTown_MoveRelearnersHouse_Text_WelcomeBattleClub:
	.string "Hello there! Welcome to the Battle Club!\p"
	.string "Here, we train Pokemon to be the best\n"
	.string "they can possibly be.\p"
	.string "Take a look over to the right.$"

FallarborTown_MoveRelearnersHouse_Movement_CameraPanToBattlersL:
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

FallarborTown_MoveRelearnersHouse_Movement_CameraPanToBattlersR:
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

FallarborTown_MoveRelearnersHouse_Text_ExplainTrainers:
	.string "On the left, you have the HP, ATK, and\n"
	.string "DEF trainers.\p"
	.string "On the right, you have the SPE, SPATK\n"
	.string "and SPDEF trainers.\p"
	.string "Lastly, in the middle is the EXP\n"
	.string "trainer.\p"
	.string "You can battle each trainer\n"
	.string "repeatedly.$"

FallarborTown_MoveRelearnersHouse_Movement_CameraPanBackL:
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

FallarborTown_MoveRelearnersHouse_Movement_CameraPanBackR:
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

FallarborTown_MoveRelearnersHouse_Text_ThatsAll:
	.string "That's everything. Come talk to me\n"
	.string "for training items.\p"
	.string "Good luck on your training!$"

FallarborTown_MoveRelearnersHouse_Movement_WalkAwayL:
	walk_right
	walk_up
	walk_up
	walk_up
	face_right
	step_end

FallarborTown_MoveRelearnersHouse_Movement_WalkAwayR:
	walk_up
	walk_up
	walk_up
	face_right
	step_end

FallarborTown_MoveRelearnersHouse_EventScript_WelcomeGuyGiveItems::
	lock
	faceplayer
	goto_if_set FLAG_POWER_ITEMS, FallarborTown_MoveRelearnersHouse_EventScript_HowAreYou
	msgbox FallarborTown_MoveRelearnersHouse_Text_TakeThese, MSGBOX_DEFAULT
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

FallarborTown_MoveRelearnersHouse_Text_TakeThese:
	.string "Take these power items!$"

FallarborTown_MoveRelearnersHouse_EventScript_HowAreYou::
	lock
	faceplayer
	msgbox FallarborTown_MoveRelearnersHouse_Text_TrainingHard, MSGBOX_DEFAULT
	release
	end

FallarborTown_MoveRelearnersHouse_Text_TrainingHard:
	.string "Training hard?$"

FallarborTown_MoveRelearnersHouse_Text_GenericDefeat:
	.string "Nice one.$"

FallarborTown_MoveRelearnersHouse_EventScript_HPTrainer::
	lock
	faceplayer
	msgbox FallarborTown_MoveRelearnersHouse_Text_WouldLikeToBattleHP, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, FallarborTown_MoveRelearnersHouse_EventScript_HPBattle
	goto_if_eq VAR_RESULT, NO, FallarborTown_MoveRelearnersHouse_EventScript_HPNoBattle
	release
	end

FallarborTown_MoveRelearnersHouse_Text_WouldLikeToBattleHP:
	.string "Would you like to train HP EVs?$"

FallarborTown_MoveRelearnersHouse_EventScript_HPBattle::
	trainerbattle_no_intro TRAINER_HPTRAIN, FallarborTown_MoveRelearnersHouse_Text_GenericDefeat
	release
	end

FallarborTown_MoveRelearnersHouse_EventScript_HPNoBattle::
	release
	end

