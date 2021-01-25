#include "cMain.h"
#include <cassert>
#include <iomanip>



wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(tag::button_0, OnButtonClickSmallText)
EVT_BUTTON(tag::button_1, OnButtonClickLargeText)
EVT_TIMER(tag::timer_0, OnTimer)
wxEND_EVENT_TABLE()

static void limitStringSize(std::string& str, size_t maxSize)
{
	str.resize(maxSize);
}

cMain::cMain()
	:
	wxFrame(nullptr, wxID_ANY, "Text Input & Send", wxPoint(30, 30), wxSize(720, 1280)),
	timer(this, tag::timer_0),
	client(td)
{
	
	//Button Font
	buttonFont = new wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial");
	

	btn0 = new wxButton(this, tag::button_0, "Small Text");
	btn0->SetFont(*buttonFont);
	btn1 = new wxButton(this, tag::button_1, "Large Text");
	btn1->SetFont(*buttonFont);
	txt0 = new wxTextCtrl(this, tag::text_0, "", wxPoint(0, 0), wxSize(1280, 300), wxTE_NOHIDESEL | wxTE_RICH | wxTE_NO_VSCROLL | wxNO_BORDER | wxTE_MULTILINE | wxTE_CENTER | wxTE_BESTWRAP);


	//terminal setup
	terminalFont = new wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	terminalWindow = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_NOHIDESEL | wxTE_RICH | wxNO_BORDER | wxTE_MULTILINE | wxTE_LEFT);
	terminalWindow->SetBackgroundColour(Colors::black);
	

	//Create Color Buttons.	
	wxButton* btnCol[8];
	sizer1 = new wxGridSizer(8, 0,0);
	for (int i = 0; i < 8; i++)
	{
		btnCol[i] = new wxButton(this, tag::button_red + i);
		sizer1->Add(btnCol[i],1,wxEXPAND | wxALL);
	
		btnCol[i]->SetBackgroundColour(cc.GetAColor(i));
		btnCol[i]->SetFont(*buttonFont);
	
		btnCol[i]->SetLabelText(cc.GetButtonLabel(i));
		btnCol[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClickColor, this);
	}

	sizer0 = new wxBoxSizer(wxVERTICAL);
	sizer2 = new wxBoxSizer(wxHORIZONTAL);
	sizer2->Add(btn0, 2, wxEXPAND | wxALL, 5);
	sizer2->Add(btn1, 2, wxEXPAND | wxALL, 5);
	sizer0->Add(sizer2, 2, wxEXPAND | wxALL, 0);
	sizer0->Add(txt0,20, wxEXPAND | wxALL, 5);
	sizer0->Add(terminalWindow, 5, wxEXPAND | wxALL, 5);
	sizer0->Add(sizer1, 1, wxEXPAND | wxALL, 0);
	sizer0->SetSizeHints(this);
	SetSizer(sizer0);

	font0 = new wxFont(36, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "ABOVEANDBYOND2013");

	txt0->SetFont(*font0);
	txt0->SetBackgroundColour({ 0,0,0 });
	cc.SetColor(Colors::white);
	txt0->SetStyle(0, 0, cc.GetCurrColor());
	txt0->Show();
	txt0->Bind(wxEVT_KEY_DOWN, &cMain::OnKeyDown, this);
	txt0->Bind(wxEVT_KEY_UP, &cMain::OnKeyDown, this);

	//timer things
	timer.Start(5);

	//Get Client Ready
	client.StartClient();
	
}

cMain::~cMain()
{

}

void cMain::OnButtonClickColor(wxCommandEvent& evt)
{
	std::string controlMessage = cc.GetCommandd(0);
	controlMessage = cc.GetCommandd(evt.GetId() + 1); //color
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, maxStrSize);
	client.SendMsg(controlMessage + stlstring);
	cc.SetColor((evt.GetId() - 2) % 8);
	txt0->SetStyle(0, txt0->GetValue().size(), cc.GetCurrColor());
	txt0->SetFont(*font0);
	txt0->SetFocus();
	evt.Skip();
}

void cMain::OnButtonClickSmallText(wxCommandEvent& evt)
{
	std::string controlMessage;
	controlMessage = cc.GetCommandd(2); //small
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, maxStrSize);
	client.SendMsg(controlMessage + stlstring);
	font0->SetPointSize(36);
	txt0->SetFont(*font0);
	txt0->SetFocus();
	txt0->SetStyle(0, txt0->GetValue().size(), cc.GetCurrColor());
	evt.Skip();
}

void cMain::OnButtonClickLargeText(wxCommandEvent& evt)
{
	std::string controlMessage;
	controlMessage = cc.GetCommandd(1); //large
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, maxStrSize);
	client.SendMsg(controlMessage + stlstring);
	font0->SetPointSize(128);
	txt0->SetFont(*font0);
	txt0->SetFocus();
	txt0->SetStyle(0, txt0->GetValue().size(), cc.GetCurrColor());
	evt.Skip();
}

void cMain::OnKeyDown(wxKeyEvent& evt)
{
	std::string controlMessage = cc.GetCommandd(0);
	//Send special null string to renderer.
	if (txt0->GetValue().size() == 0)
	{
		controlMessage = cc.GetCommandd(0); //null
		assert(controlMessage.size() == 8u);
		client.SendMsg(controlMessage);
	}
	// send the string
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, maxStrSize);
	client.SendMsg(controlMessage + stlstring);
	evt.Skip();
}

void cMain::OnTimer(wxTimerEvent& evt)
{
	//Update Network 
	client.Update();

	//Refresh the colour if size is Zero.

	if (txt0->GetValue().size() == 0)
	{
		txt0->SetStyle(0, txt0->GetValue().size(), cc.GetCurrColor());
	}

	//Terminal mamagement
	if (td.GetNoMessages() != oldNumMessages)
	{
		terminalWindow->SetStyle(td.GetLastMessage().messageStart, td.GetLastMessage().messageEnd, td.GetLastMessage().colour);
		*terminalWindow << td.GetLastMessage().message << "\n";
		if (td.GetNoMessages() >= maxNumMessages)
		{
			terminalWindow->Clear();
			td.ClearMessages();
		}
		oldNumMessages = td.GetNoMessages();
	}

	
	//listen for echos every event
	wxString echo;
	
	//resend string data every 60th timer event to save network traffic
	if (loopCounter >= 60)
	{
		client.CheckForTimeOut();
		client.AttemptReconnect();
		std::string controlMessage = cc.GetCommandd(0);
		//Timer event sends packets regardless of keystrokes.
		//Send null string to renderer.
		if (txt0->GetValue().size() == 0)
		{
			controlMessage = cc.GetCommandd(0); //null
			assert(controlMessage.size() == 8u);

		}
		// send the string
		std::string stlstring = std::string(txt0->GetValue().mb_str());
		limitStringSize(stlstring, maxStrSize);
		client.SendMsg(controlMessage + stlstring);
		
		loopCounter = 0;
	}
	//update looper
	loopCounter++;

}
