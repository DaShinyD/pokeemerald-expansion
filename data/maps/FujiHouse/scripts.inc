FujiHouse_MapScripts::
	.byte 0

FujiHouse_EventScript_Fuji::
	msgbox FujiHouse_Text_Fuji, MSGBOX_NPC
	end

FujiHouse_EventScript_Man::
	msgbox FujiHouse_Text_Man, MSGBOX_NPC
	end

FujiHouse_Text_Fuji:
	.string "Why hello there.\p"
	.string "Welcome to my Pokemon rehab center.$"

FujiHouse_Text_Man:
	.string "Dr. Fuji has a big heart.\p"
	.string "He hosts abandoned Pokemon with his\n"
	.string "own money.$"

