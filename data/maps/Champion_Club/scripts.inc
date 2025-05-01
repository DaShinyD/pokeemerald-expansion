Champion_Club_MapScripts::
	.byte 0

Champion_Club_EventScript_FirstEntryL::
	lock
	msgbox Champion_Club_Text_HeyYou, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Movement_ApproachDeskL
	waitmovement 0
	msgbox Champion_Club_Text_ClubExplained, MSGBOX_DEFAULT\
	setvar VAR_CHAMPION_CLUB_ENTER, 1
	release
	end

Champion_Club_EventScript_FirstEntryR::
	lock
	msgbox Champion_Club_Text_HeyYou, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Movement_ApproachDeskR
	waitmovement 0
	msgbox Champion_Club_Text_ClubExplained, MSGBOX_DEFAULT
	setvar VAR_CHAMPION_CLUB_ENTER, 1
	release
	end

Champion_Club_Movement_ApproachDeskL:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	face_up
	step_end

Champion_Club_Movement_ApproachDeskR:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Champion_Club_Text_HeyYou:
	.string "Hey newbie, over here.\p"
	.string "Come to the desk.$"

Champion_Club_Text_ClubExplained:
	.string "Welcome to the Champion Club!\p"
	.string "Here, Master Trainers and other\n"
	.string "Champions gather to train and\l"
	.string "battle each other.\p"
	.string "There are people from all over the\n"
	.string "world!\p"
	.string "This facility is for free battles\n"
	.string "in the singles format.\p"
	.string "Eventually, a tournament will be\n"
	.string "held in doubles to determine the\l"
	.string "best trainer.\p"
	.string "If you would like to challenge\n"
	.string "anyone, simply step on the red\l"
	.string "area opposite of them.\p"
	.string "Enjoy your time here.$"

Champion_Club_EventScript_DeskClerk::
	msgbox Champion_Club_Text_HowIsIt, MSGBOX_NPC
	end

Champion_Club_Text_HowIsIt:
	.string "Are you enjoying your time here?$"




@ Non Battle Dialogue





@ Generic Scripts

Champion_Club_EventScript_DenyBattle::
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Movement_WalkDown
	waitmovement 0
	release
	end

Champion_Club_Movement_MoveBattleCamera:
	walk_slow_left
	walk_slow_left
	walk_slow_left
	walk_slow_left
	step_end

Champion_Club_Movement_MoveBattleCameraBack:
	walk_slow_right
	walk_slow_right
	walk_slow_right
	walk_slow_right
	step_end

Champion_Club_Movement_WalkDown:
	walk_down
	step_end



@ Jake

Champion_Club_EventScript_JakeChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeJake, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_JakeBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeJake:
	.string "Would you like to challenge Master\n"
	.string "Trainer Jake?$"

Champion_Club_EventScript_JakeBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_JakeLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_JakeBattle
	end

Champion_Club_Text_JakeLetsFight:
	.string "Announcer: The battle between Jake\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Jake: Umbreon and I will beat you.$"

Champion_Club_EventScript_JakeBattle::
	trainerbattle_no_intro TRAINER_JAKE, Champion_Club_Text_JakeDefeat
	goto Champion_Club_EventScript_JakeBattleDone
	end

Champion_Club_Text_JakeDefeat:
	.string "It ain't easy being cheesy.$"

Champion_Club_EventScript_JakeBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_JakeBeat
	msgbox Champion_Club_Text_TheWinnerJake, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_JakeBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_JAKE
	release
	end

Champion_Club_Text_TheWinnerPlayer:
	.string "The winner is {PLAYER}!$"

Champion_Club_Text_TheWinnerJake:
	.string "The winner is Jake!$"

@ Percy

Champion_Club_EventScript_ParsleyChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeParsley, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_ParsleyBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeParsley:
	.string "Would you like to challenge Master\n"
	.string "Trainer Parsley?$"

Champion_Club_EventScript_ParsleyBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_ParsleyLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_ParsleyBattle
	end

Champion_Club_Text_ParsleyLetsFight:
	.string "Announcer: The battle between Parsley\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Parsley: Let's do this.$"

Champion_Club_EventScript_ParsleyBattle::
	trainerbattle_no_intro TRAINER_PERCY, Champion_Club_Text_ParsleyDefeat
	goto Champion_Club_EventScript_ParsleyBattleDone
	end

Champion_Club_Text_ParsleyDefeat:
	.string "I need my vape.$"

Champion_Club_EventScript_ParsleyBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_ParsleyBeat
	msgbox Champion_Club_Text_TheWinnerParsley, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_ParsleyBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_PERCY
	release
	end

Champion_Club_Text_TheWinnerParsley:
	.string "The winner is Parsley!$"

@ Sky

Champion_Club_EventScript_SkyChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeSky, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_SkyBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeSky:
	.string "Would you like to challenge Master\n"
	.string "Trainer Sky?$"

Champion_Club_EventScript_SkyBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_SkyLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_SkyBattle
	end

Champion_Club_Text_SkyLetsFight:
	.string "Announcer: The battle between Sky\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Sky: Are you challenging me?$"

Champion_Club_EventScript_SkyBattle::
	trainerbattle_no_intro TRAINER_SKY, Champion_Club_Text_SkyDefeat
	goto Champion_Club_EventScript_SkyBattleDone
	end

Champion_Club_Text_SkyDefeat:
	.string "Back to Best Buy...$"

Champion_Club_EventScript_SkyBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_SkyBeat
	msgbox Champion_Club_Text_TheWinnerSky, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_SkyBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_SKY
	release
	end

Champion_Club_Text_TheWinnerSky:
	.string "The winner is Sky!$"

@ Joey

Champion_Club_EventScript_JoeyChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeJoey, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_JoeyBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeJoey:
	.string "Would you like to challenge Master\n"
	.string "Trainer Joey?$"

Champion_Club_EventScript_JoeyBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_JoeyLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_JoeyBattle
	end

Champion_Club_Text_JoeyLetsFight:
	.string "Announcer: The battle between Joey\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Joey: My Raticate is best.$"

Champion_Club_EventScript_JoeyBattle::
	trainerbattle_no_intro TRAINER_JOEY2, Champion_Club_Text_JoeyDefeat
	goto Champion_Club_EventScript_JoeyBattleDone
	end

Champion_Club_Text_JoeyDefeat:
	.string "But Raticate is top!$"

Champion_Club_EventScript_JoeyBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_JoeyBeat
	msgbox Champion_Club_Text_TheWinnerJoey, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_JoeyBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_JOEY
	release
	end

Champion_Club_Text_TheWinnerJoey:
	.string "The winner is Joey!$"

@ Morde

Champion_Club_EventScript_MordeChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeMorde, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_MordeBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeMorde:
	.string "Would you like to challenge Master\n"
	.string "Trainer Morde?$"

Champion_Club_EventScript_MordeBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_MordeLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_MordeBattle
	end

Champion_Club_Text_MordeLetsFight:
	.string "Announcer: The battle between Morde\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Morde: I love big mons.$"

Champion_Club_EventScript_MordeBattle::
	trainerbattle_no_intro TRAINER_MORDE, Champion_Club_Text_MordeDefeat
	goto Champion_Club_EventScript_MordeBattleDone
	end

Champion_Club_Text_MordeDefeat:
	.string "How could you top my Kaiju?$"

Champion_Club_EventScript_MordeBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_MordeBeat
	msgbox Champion_Club_Text_TheWinnerMorde, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_MordeBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_MORDE
	release
	end

Champion_Club_Text_TheWinnerMorde:
	.string "The winner is Morde!$"

@ Sam

Champion_Club_EventScript_SamChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeSam, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_SamBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeSam:
	.string "Would you like to challenge Master\n"
	.string "Trainer Sam?$"

Champion_Club_EventScript_SamBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_SamLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_SamBattle
	end

Champion_Club_Text_SamLetsFight:
	.string "Announcer: The battle between Sam\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Sam: My Pokemon are beautiful.$"

Champion_Club_EventScript_SamBattle::
	trainerbattle_no_intro TRAINER_SAM, Champion_Club_Text_SamDefeat
	goto Champion_Club_EventScript_SamBattleDone
	end

Champion_Club_Text_SamDefeat:
	.string "My poor babies...$"

Champion_Club_EventScript_SamBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_SamBeat
	msgbox Champion_Club_Text_TheWinnerSam, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_SamBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_SAM
	release
	end

Champion_Club_Text_TheWinnerSam:
	.string "The winner is Sam!$"

@ Alex

Champion_Club_EventScript_AlexChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengeAlex, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_AlexBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengeAlex:
	.string "Would you like to challenge Master\n"
	.string "Trainer Alex?$"

Champion_Club_EventScript_AlexBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_AlexLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_AlexBattle
	end

Champion_Club_Text_AlexLetsFight:
	.string "Announcer: The battle between Alex\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Alex: It's time to duel.$"

Champion_Club_EventScript_AlexBattle::
	trainerbattle_no_intro TRAINER_ALEX, Champion_Club_Text_AlexDefeat
	goto Champion_Club_EventScript_AlexBattleDone
	end

Champion_Club_Text_AlexDefeat:
	.string "I have no pathetic Pokemon.$"

Champion_Club_EventScript_AlexBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_AlexBeat
	msgbox Champion_Club_Text_TheWinnerAlex, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_AlexBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_ALEX
	release
	end

Champion_Club_Text_TheWinnerAlex:
	.string "The winner is Alex!$"

@ Pena

Champion_Club_EventScript_PenaChallenge::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceLeft
	waitmovement 0
	msgbox Champion_Club_Text_WantToChallengePena, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Champion_Club_EventScript_PenaBattleSetup
	goto_if_eq VAR_RESULT, NO, Champion_Club_EventScript_DenyBattle
	end

Champion_Club_Text_WantToChallengePena:
	.string "Would you like to challenge Master\n"
	.string "Trainer Pena?$"

Champion_Club_EventScript_PenaBattleSetup::
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCamera
	waitmovement 0
	msgbox Champion_Club_Text_PenaLetsFight, MSGBOX_DEFAULT
	closemessage
	special RemoveCameraObject
	goto Champion_Club_EventScript_PenaBattle
	end

Champion_Club_Text_PenaLetsFight:
	.string "Announcer: The battle between Pena\n"
	.string "and {PLAYER} will now begin!\p"
	.string "Pena: Gooning time.$"

Champion_Club_EventScript_PenaBattle::
	trainerbattle_no_intro TRAINER_PENA, Champion_Club_Text_PenaDefeat
	goto Champion_Club_EventScript_PenaBattleDone
	end

Champion_Club_Text_PenaDefeat:
	.string "Out gooned in front of the huzz.$"

Champion_Club_EventScript_PenaBattleDone::
	specialvar VAR_RESULT, GetBattleOutcome
	compare VAR_RESULT, B_OUTCOME_WON
	goto_if_eq Champion_Club_EventScript_PenaBeat
	msgbox Champion_Club_Text_TheWinnerPena, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	release
	end

Champion_Club_EventScript_PenaBeat::
	msgbox Champion_Club_Text_TheWinnerPlayer, MSGBOX_DEFAULT
	closemessage
	special SpawnCameraObject
	applymovement OBJ_EVENT_ID_CAMERA, Champion_Club_Movement_MoveBattleCameraBack
	waitmovement 0
	special RemoveCameraObject
	special HealPlayerParty
	setflag FLAG_BEAT_PENA
	release
	end

Champion_Club_Text_TheWinnerPena:
	.string "The winner is Pena!$"

