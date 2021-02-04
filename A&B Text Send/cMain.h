#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "ClientTCP.h"
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "ListsAndColors.h"
#include "Terminal.h"

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
	std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();

	int maxStrSize = 500;

	//buttons and boxes etc
	wxButton* btn0 = nullptr;
	wxButton*btn1 = nullptr;
	wxFont* font0 = nullptr;
	wxFont* buttonFont = nullptr;
	wxFont* terminalFont = nullptr;
	wxTextCtrl* terminalWindow = nullptr;
	wxTextCtrl* txt0 = nullptr;
	wxBoxSizer* sizer0 = nullptr;
	wxGridSizer* sizer1 = nullptr;
	wxBoxSizer* sizer2 = nullptr;

	//terminal
	TerminalData td;
	size_t oldNumMessages = 0;
	size_t maxNumMessages = 512;

	//network things
	ClientTCP client;

	//strings for echo
	wxString oldString = "";

private:
	wxTextAttr textDesc;
	ColorManager cc;

	wxDECLARE_EVENT_TABLE();
};



