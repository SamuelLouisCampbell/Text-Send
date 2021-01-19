#include "ListsAndColors.h"

void ColorManager::SetColor(const int index)
{
	currColor = ButtonCols[index];
}

const char* ColorManager::GetButtonLabel(const int index)
{
	return ButtonLabels[index];
}

const char* ColorManager::GetCommandd(const int index)
{
	return Commands[index];
}

wxColor ColorManager::GetAColor(const int index) const
{
	return ButtonCols[index];
}

wxColor ColorManager::GetCurrColor() const
{
	return currColor;
}
