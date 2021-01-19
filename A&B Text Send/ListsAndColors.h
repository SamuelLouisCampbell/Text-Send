#pragma once
#include <wx/wx.h>

namespace ListsAndColors
{
	wxColor ButtonCols[11] =
	{
		{255,255,255,},	
		{255,0,0,},		
		{0,255,0,},		
		{0,0,255},		
		{0,255,255,},	
		{255,0,255,},	
		{255,255,0,},	
		{255,128,0,},	
		{64,64,64},		
		{0,0,0,},		
		{128,25,255}	//10 Terminal Font
	};

	const char* Commands[11] =
	{
		"NULL....", //0
		"LARGE...", //1
		"SMALL...", //2
		"WHITE...", //9
		"RED.....", //3
		"GREEN...", //4
		"BLUE....", //5
		"CYAN....", //6
		"MAGENTA.", //7
		"YELLOW..", //8
		"ORANGE..", //10
	};
	const char* ButtonLabels[8] =
	{
		"White",
		"Red",
		"Green",
		"Blue",
		"Cyan",
		"Magenta",
		"Yellow",
		"Orange"
	};
}