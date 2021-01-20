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
	rmd("defaults.txt")
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
	terminalWindow = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_NOHIDESEL | wxTE_RICH | wxNO_BORDER | wxTE_MULTILINE | wxTE_LEFT);
	terminalWindow->SetBackgroundColour(Colors::black);
	

	//Create Color Buttons.	
	wxButton* btnCol[8];
	sizer1 = new wxGridSizer(8, 0,0);
	for (int i = 0; i < 8; i++)
	{
		btnCol[i] = new wxButton(this, tag::button_red + i);
		sizer1->Add(btnCol[i],1,wxEXPAND | wxALL);
		//btnCol[i]->SetBackgroundColour(ListsAndColors::ButtonCols[i]);
		btnCol[i]->SetBackgroundColour(cc.GetAColor(i));
		btnCol[i]->SetFont(*buttonFont);
		//btnCol[i]->SetLabelText(ListsAndColors::ButtonLabels[i]);
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
	this->SetBackgroundColour({ 12,12,12 });
	txt0->SetFont(*font0);
	txt0->SetBackgroundColour({ 0,0,0 });
	cc.SetColor(Colors::white);
	txt0->SetStyle(0, 0, cc.GetCurrColor());
	txt0->Show();
	txt0->Bind(wxEVT_KEY_DOWN, &cMain::OnKeyDown, this);
	txt0->Bind(wxEVT_KEY_UP, &cMain::OnKeyDown, this);

	

	timer.Start(5);

	//setup network with error checking
	client = std::make_unique<CustomClient>();
	if (rmd.FileReadOK() && rmd.CheckRMIPGood() && rmd.CheckRMPortsGood())
	{
		client->Connect(rmd.GetIP(), rmd.GetServerPort());
		
		std::stringstream ss; 
		ss << "Defaults file read OK. Sending to: " << rmd.GetIP() << " Port: " << rmd.GetServerPort() << " ";
		ss << "Local Information: " << client->GetConnectionInfo();
		//terminal->Append(ss.str());
		td.AppendMessage(ss.str(), cc.GetAColor(Colors::green));
	}
	else
	{	
		rmd.SetIPToDefault();
		rmd.SetPortToDefault();
		std::stringstream ss;
		ss << "Problem with ports or IP Addr. Reset to defaults. Sending to: "
			<< rmd.GetIP() << " Port: " << rmd.GetServerPort();
		td.AppendMessage(ss.str(), cc.GetAColor(Colors::red));
		//terminal->Append(ss.str());

	}
	
}

cMain::~cMain()
{
	client.release();
}

void cMain::OnButtonClickColor(wxCommandEvent& evt)
{
	std::string controlMessage = cc.GetCommandd(0);
	controlMessage = cc.GetCommandd(evt.GetId() + 1); //color
	assert(controlMessage.size() == 8u);
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, maxStrSize);
	SendProtectedMessage(controlMessage + stlstring);
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
	SendProtectedMessage(controlMessage + stlstring);
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
	SendProtectedMessage(controlMessage + stlstring);
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
		
		SendProtectedMessage(controlMessage);
	}
	// send the string
	std::string stlstring = std::string(txt0->GetValue().mb_str());
	limitStringSize(stlstring, maxStrSize);
	SendProtectedMessage(controlMessage + stlstring);
	evt.Skip();
}

void cMain::OnTimer(wxTimerEvent& evt)
{
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
		if (td.GetNoMessages() > 100)
		{
			terminalWindow->Clear();
			td.ClearMessages();
		}
		oldNumMessages = td.GetNoMessages();
	}

	//delete client if network times out
	if (!NetworkHealthChecker())
	{
		if (client != nullptr)
		{
			client->Disconnect();
			client.release();
			client = nullptr;
		}
		UpdateHealthChecker(true);
	}


	//listen for echos every event
	wxString echo;
	if (client != nullptr && client->IsConnected())
	{
		if (!client->IncomingMessages().empty())
		{
			auto msg = client->IncomingMessages().pop_front().msg;

			switch (msg.header.id)
			{
			case CustomMsgType::MessageServer:
			{
				for (auto& c : msg.body)
				{
					echo += c;
				}
				break;
			}
			case CustomMsgType::HealthCheckServer:
			{
				//reset health check val;
				UpdateHealthChecker(true);
				client->EchoHealthCheck();
				break;
			}
			case CustomMsgType::ServerPing:
			{
				std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point then;
				msg >> then;
				std::stringstream ss;
				ss << "Ping Recieved: " << std::chrono::duration(now - then).count() << "ms.";
				td.AppendMessage(ss.str(), cc.GetAColor(Colors::orange));
				break;
			}
			default:
				client->IncomingMessages().eraseQ();
				break;
			}
		}
	}

	//check to see if message is unique/new.
	if (client != nullptr)
	{
		if (echo != oldString)
		{
			oldString = echo;
			if (oldString.size() >= 1)
			{
				std::stringstream ss;
				ss << "Echo returned to: " << client->GetConnectionInfo() << " : " << oldString;
				td.AppendMessage(ss.str(), cc.GetAColor(Colors::green));
			}
		}
	}
	
	//resend string data every 60th timer event to save network traffic
	if (loopCounter >= 60)
	{
		//try and timeout healthcheck 
		UpdateHealthChecker(false);
		std::stringstream ss;
		if (client == nullptr || !client->IsConnected())
		{
			ss << "Network Timeout = " << healthCheck - 1;
			td.AppendMessage(ss.str(), cc.GetAColor(Colors::red));
		}

		//if disconnected try to reconnect
		if (client == nullptr && !NetworkHealthChecker())
		{
			client = std::make_unique<CustomClient>();
			client->Connect(rmd.GetIP(), rmd.GetServerPort());
			UpdateHealthChecker(true);
			std::stringstream ss;
			ss << "Network down attempting reconnect to: " << rmd.GetIP() << " Port: " << rmd.GetServerPort() << " ";
			ss << "Local Information: " << client->GetConnectionInfo();
			td.AppendMessage(ss.str(), cc.GetAColor(Colors::blue));
		}

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
		SendProtectedMessage(controlMessage + stlstring);
		
		loopCounter = 0;
	}
	//update looper
	loopCounter++;

}

bool cMain::NetworkHealthChecker() const
{
	if (healthCheck >= 5)
	{
		return false;
	}
	return true;
}

void cMain::UpdateHealthChecker(bool updateBool)
{
	if (updateBool == true)
	{
		//reset if any true condition
		healthCheck = 0;
	}
	else
	{
		//add one this time.
		healthCheck++;
	}
}

void cMain::SendProtectedMessage(std::string message)
{
	if (client != nullptr)
	{
		client->SendMsg(message);
	}
}
