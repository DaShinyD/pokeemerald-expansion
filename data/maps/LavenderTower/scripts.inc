.set LOCALID_SCARY, 1

LavenderTower_MapScripts::
	.byte 0

LavenderTower_EventScript_GhostGirl::
	lock
	faceplayer
	createfieldmugshot MUGSHOT_PRE_SCARY
	msgbox LavenderTower_Text_HelloGhost, MSGBOX_YESNO
	removefieldmugshot
	switch VAR_RESULT
	case NO, LavenderTower_EventScript_GhostMad
	case YES, LavenderTower_EventScript_GhostMad
	release
	end

LavenderTower_Text_HelloGhost:
	.string "Hello there...\p"
	.string "Tell me, am I pretty?$"

LavenderTower_EventScript_GhostMad::
	msgbox LavenderTower_Text_IsThatSo, MSGBOX_DEFAULT
	closemessage
	createfieldmugshot MUGSHOT_SCARY
	playse SE_SELECT
	msgbox LavenderTower_Text_AndNow, MSGBOX_DEFAULT
	closemessage
	removefieldmugshot
	goto LavenderTower_EventScript_GhostBattle
	release
	end

LavenderTower_Text_IsThatSo:
	.string "Is that so...$"

LavenderTower_Text_AndNow:
	.string "AND NOW?$"

LavenderTower_EventScript_GhostBattle::
	trainerbattle_no_intro TRAINER_SCARY, LavenderTower_Text_DotDotDot
	goto LavenderTower_EventScript_GhostGone
	release
	end

LavenderTower_Text_DotDotDot:
	.string "...$"

LavenderTower_EventScript_GhostGone::
	fadescreen FADE_TO_BLACK
	setflag FLAG_GHOST_PRETTY
	removeobject LOCALID_SCARY
	fadescreen FADE_FROM_BLACK
	release
	end

