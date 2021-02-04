#pragma once
#include <wx/colour.h>

class ColorManager
{
public:
	void SetColor(const int col_in);
	const char* GetButtonLabel(const int index);
	const char* GetCommandd(const int index);
	wxColor GetAColor(const int index) const;
	wxColor GetCurrColor() const;
private:
	wxColor currColor = ButtonCols[0];
	wxColor ButtonCols[13] =
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
		{64,0,255}, //10 Terminal Font
		{128,0,0}, //11 Dark Red
		{0,128,64}, //12 Dark Green
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
};

