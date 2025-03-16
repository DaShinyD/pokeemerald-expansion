Route135_MapScripts::
	.byte 0

Route135_EventScript_ReturnVillage::
	lock
	faceplayer
	msgbox Route135_Text_WantReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Route135_EventScript_ReturnYes
	msgbox Route135_Text_GuessNot, MSGBOX_DEFAULT
	release
	end

Route135_Text_WantReturn:
	.string "Would you like to return?$"

Route135_Text_GuessNot:
	.string "Alright, just let me know.$"

Route135_EventScript_ReturnYes::
	warp MAP_DESERT_VILLAGE, 19, 21
	waitstate
	release
	end

