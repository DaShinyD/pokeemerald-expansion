AncientPyramid_MapScripts::
	.byte 0

AncientPyramid_EventScript_Trigger1::
	lock
	fadescreen FADE_TO_BLACK
	msgbox AncientPyramid_Text_Activated1, MSGBOX_DEFAULT
	closemessage
	setmetatile 15, 19, METATILE_PyramidInteriorP_Pillar_Right_1, TRUE
	setmetatile 15, 18, METATILE_PyramidInteriorP_Pillar_Right_2, TRUE
	setmetatile 15, 17, METATILE_PyramidInteriorP_Pillar_Right_3, TRUE
	setmetatile 14, 17, METATILE_PyramidInteriorP_Pillar_Middle, FALSE
	setmetatile 14, 18, METATILE_PyramidInteriorP_Pyramid_Floor, FALSE
	setmetatile 14, 19, METATILE_PyramidInteriorP_Pyramid_Floor, FALSE
	setmetatile 13, 17, METATILE_PyramidInteriorP_Pillar_Left_3, TRUE
	setmetatile 13, 18, METATILE_PyramidInteriorP_Pillar_Left_2, TRUE
	setmetatile 13, 19, METATILE_PyramidInteriorP_Pillar_Left_1, TRUE
	special DrawWholeMapView
	delay 10
	fadescreen FADE_FROM_BLACK
	release
	end

AncientPyramid_Text_Activated1:
	.string "You found a switch.\p"
	.string "You pressed it and heard a noise.$"

AncientPyramid_EventScript_Trigger2::
	lock
	fadescreen FADE_TO_BLACK
	msgbox AncientPyramid_Text_Activated1, MSGBOX_DEFAULT
	closemessage
	setmetatile 15, 10, METATILE_PyramidInteriorP_Pillar_Right_1, TRUE
	setmetatile 15, 9, METATILE_PyramidInteriorP_Pillar_Right_2, TRUE
	setmetatile 15, 8, METATILE_PyramidInteriorP_Pillar_Right_3, TRUE
	setmetatile 14, 8, METATILE_PyramidInteriorP_Pillar_Middle, FALSE
	setmetatile 14, 9, METATILE_PyramidInteriorP_Pyramid_Floor, FALSE
	setmetatile 14, 10, METATILE_PyramidInteriorP_Pyramid_Floor, FALSE
	setmetatile 13, 8, METATILE_PyramidInteriorP_Pillar_Left_3, TRUE
	setmetatile 13, 9, METATILE_PyramidInteriorP_Pillar_Left_2, TRUE
	setmetatile 13, 10, METATILE_PyramidInteriorP_Pillar_Left_1, TRUE
	special DrawWholeMapView
	delay 10
	fadescreen FADE_FROM_BLACK
	release
	end

