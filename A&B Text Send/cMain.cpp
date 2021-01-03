#include "cMain.h"
#include <cassert>
#include "ListsAndColors.h"


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
	rmd("defaults.txt")
{
	
	//Button Font
	buttonFont = new wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial");
	

	btn0 = new wxButton(this, tag::button_0, "Small Text");
	btn0->SetFont(*buttonFont);
	btn1 = new wxButton(this, tag::button_1, "Large Text");
	btn1->SetFont(*buttonFont);
	txt0 = new wxTextCtrl(this, tag::text_0, "", wxPoint(0, 0), wxSize(1280, 300), wxTE_MULTILINE | wxTE_CENTER | wxTE_BESTWRAP);

	//terminal setup
	terminalFont = new wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Consolas");
	terminal = new wxListBox(this, wxID_ANY);
	terminal->SetFont(*terminalFont);
	terminal->SetBackgroundColour(ListsAndColors::ButtonCols[9]);
	terminal->SetForegroundColour(ListsAndColors::ButtonCols[1]);

	//Create Color Buttons
	
	wxButton* btnCol[8];
	sizer1 = new wxGridSizer(8, 0,0);
	for (int i = 0; i < 8; i++)
	{
		btnCol[i] = new wxButton(this, tag::button_red + i);
		sizer1->Add(btnCol[i],1,wxEXPAND | wxALL);
		btnCol[i]->SetBackgroundColour(ListsAndColors::ButtonCols[i]);
		btnCol[i]->SetFont(*buttonFont);
		btnCol[i]->SetLabelText(ListsAndColors::ButtonLabels[i]);
		btnCol[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClickColor, this);
	}

	sizer0 = new wxBoxSizer(wxVERTICAL);
	sizer2 = new wxBoxSizer(wxHORIZONTAL);
	sizer2->Add(btn0, 2, wxEXPAND | wxALL, 5);
	sizer2->Add(btn1, 2, wxEXPAND | wxALL, 5);
	sizer0->Add(sizer2, 2, wxEXPAND | wxALL, 0);
	sizer0->Add(txt0,20, wxEXPAND | wxALL, 5);
	sizer0->Add(terminal, 5, wxEXPAND | wxALL, 5);
	sizer0->Add(sizer1, 1, wxEXPAND | wxALL, 0);
	sizer0->SetSizeHints(this);
	SetSizer(sizer0);
	
	font0 = new wxFont(36, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Arial");
	this->SetBackgroundColour({ 12,12,12 });
	txt0->SetFont(*font0);
	txt0->SetForegroundColour({ 255,255,255 });
	txt0->SetBackgroundColour({ 0,0,0 });
	txt0->Show();
	txt0->Bind(wxEVT_KEY_DOWN, &cMain::OnKeyDown, this);
	txt0->Bind(wxEVT_KEY_UP, &cMain::OnKeyDown, this);

	timer.Start(5);

	//setup UDP sockets with error checking
	if (rmd.CheckRMIPGood() && rmd.CheckRMPortsGood() && rmd.FileReadOK())
	{
		//sender = std::make_unique<UDPClient>(rmd.GetClientPort(), rmd.GetIP());
		//listener = std::make_unique<UDPServer>(rmd.GetServerPort());
		std::stringstream ss; 
		ss << "Defaults file read OK. Outgoing port: " << rmd.GetClientPort() << " Sending to: "
			<< rmd.GetIP() << " Listening on port: " << rmd.GetServerPort();
		terminal->Append(ss.str());
	}
	else
	{	
		rmd.SetIPToDefault();
		rmd.SetPortToDefault();
		//sender = std::make_unique<UDPClient>(rmd.GetClientPort(), rmd.GetIP());
		//listener = std::make_unique<UDPServer>(rmd.GetServerPort());
		std::stringstream ss;
		ss << "Problem with ports or IP Addr. Reset to defaults. Outgoing port: " << rmd.GetClientPort() << " Sending to: "
			<< rmd.GetIP() << " Listening on port: " << rmd.GetServerPort();
		terminal->Append(ss.str());
	}
	
}

cMain::~cMain()
{
}

void cMain::OnButtonClickColor(wxCommandEvent& evt)
{
	std::string controlMessage = ListsAndColors::Commands[0];
	controlMessage = ListsAndColors::Commands[evt.GetId() + 1]; //color
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, 500);
	//sender->UDP_Send(controlMessage + stlstring);
	txt0->SetForegroundColour(ListsAndColors::ButtonCols[(evt.GetId() - 2) % 8]);
	txt0->SetFont(*font0);
	txt0->SetFocus();
	evt.Skip();
}

void cMain::OnButtonClickSmallText(wxCommandEvent& evt)
{
	std::string controlMessage = ListsAndColors::Commands[0];
	controlMessage = ListsAndColors::Commands[2]; //small
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, 500);
	//sender->UDP_Send(controlMessage + stlstring);
	font0->SetPointSize(36);
	txt0->SetFont(*font0);
	txt0->SetFocus();
	evt.Skip();
}

void cMain::OnButtonClickLargeText(wxCommandEvent& evt)
{
	std::string controlMessage = ListsAndColors::Commands[0];
	controlMessage = ListsAndColors::Commands[1]; //large
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, 500);
	//sender->UDP_Send(controlMessage + stlstring);
	font0->SetPointSize(128);
	txt0->SetFont(*font0);
	txt0->SetFocus();
	evt.Skip();
}

void cMain::OnKeyDown(wxKeyEvent& evt)
{
	std::string controlMessage = ListsAndColors::Commands[0];
	//Send special null string to renderer.
	if (txt0->GetValue().size() == 0)
	{
		controlMessage = ListsAndColors::Commands[0]; //null
		assert(controlMessage.size() == 8u);
		///sender->UDP_Send(controlMessage);
	}
	// send the string
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	//listener->Recieve();
	limitStringSize(stlstring, 500);
	//sender->UDP_Send(controlMessage + stlstring);
	evt.Skip();
}

void cMain::OnTimer(wxTimerEvent& evt)
{
	//listen for echos every event
	//listener->Recieve();
	wxString echo = ""; //listener->GetNetworkMessageWithInfo();
	//check to see if message is unique/new.
	if (echo != oldString)
	{
		oldString = echo;
		terminal->Append(echo);
		terminal->EnsureVisible(terminal->GetCount() - 1);
	}

	//resend string data every 200th timer event to save network traffic
	if (loopCounter >= 200)
	{
		loopCounter = 0;
		std::string controlMessage = ListsAndColors::Commands[0];
		//Timer event sends packets regardless of keystrokes.
		//Send null string to renderer.
		if (txt0->GetValue().size() == 0)
		{
			controlMessage = ListsAndColors::Commands[0]; //null
			assert(controlMessage.size() == 8u);
			//sender->UDP_Send(controlMessage);
		}
		// send the string
		std::string stlstring = std::string(txt0->GetValue().mb_str());
		limitStringSize(stlstring, 500);
		//sender->UDP_Send(controlMessage + stlstring);
	}
	//update looper
	loopCounter++;
}
