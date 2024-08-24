RusturfMine_MapScripts::
	.byte 0

RusturfMine_EventScript_Scientist::
	lock
	faceplayer
	msgbox RusturfMine_Text_ScientistExplain, MSGBOX_DEFAULT
	release
	end

RusturfMine_Text_ScientistExplain:
	.string "We recently discovered this\n"
	.string "new part of Rusturf Tunnel.\p"
	.string "In here, you can find ancient\n"
	.string "Pokemon that are extinct.\p"
	.string "Or were, at least.$"

