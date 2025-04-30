UnknownRuins_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, UnknownRuins_OnLoad
	.byte 0

UnknownRuins_OnLoad:
	call_if_set FLAG_UNOWN_PUZZLE, UnknownRuins_EventScript_Door
	end

UnknownRuins_EventScript_Door::
	setmetatile 28, 13, METATILE_ruins_CaveEntranceUBot, FALSE
	setmetatile 28, 12, METATILE_ruins_CaveEntranceUTop, TRUE
	return

UnknownRuins_EventScript_Puzzle::
	lock
	goto_if_set FLAG_UNOWN_PUZZLE, UnknownRuins_EventScript_AlreadyDone
	msgbox UnknownRuins_Text_Achoo, MSGBOX_DEFAULT
	specialvar VAR_RESULT, HasBLESSUParty
	goto_if_eq VAR_RESULT, TRUE, UnknownRuins_EventScript_Yes
	msgbox UnknownRuins_Text_Rude, MSGBOX_DEFAULT
	release
	end

UnknownRuins_Text_Achoo:
	.string "You read the text.\p"
	.string "'Achoo!'\p"
	.string "...$"

UnknownRuins_Text_Rude:
	.string "The wall briefly changes.\p"
	.string "'Rude.'$"

UnknownRuins_EventScript_Yes::
	call UnknownRuins_EventScript_Door
	special DrawWholeMapView
	setflag FLAG_UNOWN_PUZZLE
	msgbox UnknownRuins_Text_Manners, MSGBOX_DEFAULT
	release
	end

UnknownRuins_Text_Manners:
	.string "The wall says 'Thanks.'$"

UnknownRuins_EventScript_AlreadyDone::
	release
	end

