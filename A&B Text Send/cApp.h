#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "UDPClient.h"
#include "UDPServer.h"
#include <wx/wx.h>
#include "cMain.h"




class cApp : public wxApp
{
public:
	cApp();
	~cApp();
private:
	cMain* fram0 = nullptr;
	virtual bool OnInit();
};

