#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "NetClient.h"
#include "LoadRMData.h"
#include <wx/wx.h>

enum class CustomMsgType : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageServer,
	EchoMessage,
	HealthCheckServer,
};

class CustomClient : public netcommon::ClientInterface<CustomMsgType>
{
public:
	void PingServer()
	{	
		netcommon::message<CustomMsgType> msg;
		msg.header.id = CustomMsgType::ServerPing;
		//caution
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		msg << now;
		m_connection->Send(msg);
		
	}
	void SendMsg(const std::string& messsage)	{
		netcommon::message<CustomMsgType> msg;

		bool finished = false;
		for (size_t i = 0; i < messsage.size(); i++)
		{
			if (!finished)
			{
				msg << messsage[i];
				if (messsage[i] == '\0')
				{
					finished = true;
					break;
				}
			}
		}
		msg.header.id = CustomMsgType::MessageServer;
		m_connection->Send(msg);
	}
	void EchoHealthCheck()
	{
		netcommon::message<CustomMsgType> msg;
		msg.header.id = CustomMsgType::EchoMessage;
		m_connection->Send(msg);
	}

private:
	
};

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
	wxTextCtrl* txt0 = nullptr;
	wxListBox* terminal = nullptr;
	wxBoxSizer* sizer0 = nullptr;
	wxGridSizer* sizer1 = nullptr;
	wxBoxSizer* sizer2 = nullptr;

	//network things
	LoadRMData rmd;
	std::unique_ptr<CustomClient> client;

	//strings for echo
	wxString oldString = "";

private:
	int healthCheck = 0;
	bool NetworkHealthChecker() const;
	void UpdateHealthChecker(bool updateBool);
	void SendProtectedMessage(std::string message);


	wxDECLARE_EVENT_TABLE();
};



