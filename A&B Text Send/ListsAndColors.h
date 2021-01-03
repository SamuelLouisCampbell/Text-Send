#pragma once
#include <wx/wx.h>

namespace ListsAndColors
{
	wxColor ButtonCols[10] =
	{
		{255,0,0,},
		{0,255,0,},
		{0,0,255},
		{0,255,255,},
		{255,0,255,},
		{255,255,0,},
		{255,255,255,},
		{255,128,0,},
		{64,64,64},
		{0,0,0,}
	};

	const char* Commands[11] =
	{
		"NULL....", //0
		"LARGE...", //1
		"SMALL...", //2
		"RED.....", //3
		"GREEN...", //4
		"BLUE....", //5
		"CYAN....", //6
		"MAGENTA.", //7
		"YELLOW..", //8
		"WHITE...", //9
		"ORANGE..", //10
	};
	const char* ButtonLabels[8] =
	{
		"Red",
		"Green",
		"Blue",
		"Cyan",
		"Magenta",
		"Yellow",
		"White",
		"Orange"
	};
}