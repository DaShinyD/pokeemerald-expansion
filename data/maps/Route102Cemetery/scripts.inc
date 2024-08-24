.set LOCALID_CORSOLA_BALL, 3

Route102Cemetery_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Route102Cemetery_OnLoad
	.byte 0

Route102Cemetery_OnLoad::
	goto_if_set FLAG_GHOSTGIRL_102, Route102Cemetery_EventScript_SetGraveAndBall
	end

Route102Cemetery_EventScript_SetGraveAndBall::
	setmetatile 0, 6, METATILE_Facility_Grave_Normal, TRUE
	setflag FLAG_CYNDAQUIL_BALL
	return

Route102Cemetery_EventScript_SpellTag::
	call Common_EventScript_FindItem
	end

Route102Cemetery_EventScript_GhostGirl::
	lock
	msgbox Route102Cemetery_Text_PleaseHelp, MSGBOX_DEFAULT
	setvar VAR_TEMP_0, 1
	release
	end

Route102Cemetery_Text_PleaseHelp:
	.string "Hello there. This cemetery hasn't been\n"
	.string "maintained in quite a while.\p"
	.string "Would you do me a favor and clean the\n"
	.string "bottom left grave? Thank you.$"

Route102Cemetery_EventScript_Grave::
	lock
	goto_if_set FLAG_GHOSTGIRL_102, Route102Cemetery_EventScript_Grave2
	goto_if_eq VAR_TEMP_0, 0, Route102Cemetery_EventScript_DirtyGrave
	setflag FLAG_GHOSTGIRL_102
	clearflag FLAG_CYNDAQUIL_BALL
	fadescreen FADE_TO_BLACK
	playfanfare MUS_HEAL
	msgbox Route102Cemetery_Text_CleanedGrave MSGBOX_DEFAULT
	closemessage
	waitfanfare
	setmetatile 0, 6, METATILE_Facility_Grave_Normal, TRUE
	special DrawWholeMapView
	delay 10
	fadescreen FADE_FROM_BLACK
	delay 30
	release
	end

Route102Cemetery_EventScript_Grave2::
	msgbox Route102Cemetery_Text_AlreadyCleaned MSGBOX_SIGN
	release
	end

Route102Cemetery_EventScript_DirtyGrave::
	msgbox Route102Cemetery_Text_DirtyGrave MSGBOX_SIGN
	release
	end

Route102Cemetery_Text_DirtyGrave:
	.string "A dirty grave.$"

Route102Cemetery_Text_CleanedGrave:
	.string "You cleaned the grave.$"

Route102Cemetery_Text_AlreadyCleaned:
	.string "It's the grave you cleaned.$"

Route102Cemetery_EventScript_CyndaquilBall::
	lock
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CORSOLA_GALARIAN
	givemon SPECIES_CORSOLA_GALARIAN, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, Route102Cemetery_EventScript_SendMonToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, Route102Cemetery_EventScript_SendMonToPC
	goto Common_EventScript_NoMoreRoomForPokemon
	end

Route102Cemetery_EventScript_SendMonToParty::
	call Route102Cemetery_EventScript_ReceivedCorsolaFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route102Cemetery_EventScript_ReceivedCorsola
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto Route102Cemetery_EventScript_ReceivedCorsola
	end

Route102Cemetery_EventScript_SendMonToPC::
	call Route102Cemetery_EventScript_ReceivedCorsolaFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Route102Cemetery_EventScript_CorsolaTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto Route102Cemetery_EventScript_CorsolaTransferredToPC
	end

Route102Cemetery_EventScript_CorsolaTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto Route102Cemetery_EventScript_ReceivedCorsola
	end

Route102Cemetery_EventScript_ReceivedCorsolaFanfare::
	bufferspeciesname STR_VAR_2, SPECIES_CORSOLA_GALARIAN
	removeobject LOCALID_CORSOLA_BALL
	playfanfare MUS_OBTAIN_ITEM
	message Route102Cemetery_Text_ObtainedCorsola
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, SPECIES_CORSOLA_GALARIAN
	return

Route102Cemetery_EventScript_ReceivedCorsola::
	setflag FLAG_CYNDAQUIL_BALL
	releaseall
	end

Route102Cemetery_Text_ObtainedCorsola:
	.string "You received a Galarian Corsola!$"
	
