#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "cMain.h"
#include <wx/wx.h>


class cApp : public wxApp
{
public:
	cApp();
	~cApp();
private:
	cMain* fram0 = nullptr;
	virtual bool OnInit();
};

