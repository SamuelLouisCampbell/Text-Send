#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "UDPClient.h"
#include "UDPServer.h"
#include "LoadRMData.h"
#include <wx/wx.h>

enum tag
{
	button_0,
	button_1,
	button_red,
	button_green,
	button_blue,
	button_cyan,
	button_magenta,
	button_yellow,
	button_white,
	button_orange,
	text_0,
	timer_0
};

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	void OnButtonClickColor(wxCommandEvent& evt);
	void OnButtonClickSmallText(wxCommandEvent& evt);
	void OnButtonClickLargeText(wxCommandEvent& evt);
	void OnKeyDown(wxKeyEvent& evt);
	void OnTimer(wxTimerEvent& evt);

protected:
	//timer things
	wxTimer timer;
	int loopCounter = 0;

	//buttons and boxes etc
	wxButton* btn0 = nullptr;
	wxButton*btn1 = nullptr;
	wxFont* font0 = nullptr;
	wxFont* buttonFont = nullptr;
	wxFont* terminalFont = nullptr;
	wxTextCtrl* txt0 = nullptr;
	wxListBox* terminal = nullptr;
	wxBoxSizer* sizer0 = nullptr;
	wxGridSizer* sizer1 = nullptr;
	wxBoxSizer* sizer2 = nullptr;

	//network things
	LoadRMData rmd;
	std::unique_ptr<UDPClient> sender = nullptr;
	std::unique_ptr<UDPServer> listener = nullptr;


	//strings for echo
	wxString oldString = "";

private:
	wxDECLARE_EVENT_TABLE();
};



