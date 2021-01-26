#pragma once
#include "NetClient.h"
#include "LoadRMData.h"
#include "Terminal.h"
#include "ListsAndColors.h"

enum Colors
{
	white,
	red,
	green,
	blue,
	cyan,
	magenta,
	yellow,
	orange,
	grey,
	black,
	purple,
	darkRed,
	darkGreen
};

enum class CustomMsgType : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageServer,
	EchoMessage,
	HealthCheckServer,
	ServerValidated,
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
	void SendMsg(const std::string& messsage) {
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

class ClientTCP
{
public:

	ClientTCP(TerminalData& tdRef)
		:
		tdRef(tdRef),
		rmd("defaults.txt")
	{}
	void StartClient()
	{
		if (!clientStarted)
		{
			tdRef.AppendMessage("Attempting to reach Server\n", cc.GetAColor(Colors::darkGreen));
			clientStarted = InitClient();
		}
	}
	void Update()
	{
		if (client != nullptr && client->IsConnected())
		{
			if (!client->IncomingMessages().empty())
			{
				auto msg = client->IncomingMessages().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMsgType::ServerValidated:
				{
					tdRef.AppendMessage("Server Validated Connection", cc.GetAColor(Colors::green));
					clientValid = true;
					break;
				}
				case CustomMsgType::MessageServer:
				{
					std::stringstream ss; 
					ss << "Echo from server : ";
					for (size_t i = 0; i < msg.body.size(); i++)
					{
						ss << msg.body[i];
					}
					tdRef.AppendMessage(ss.str().c_str(), cc.GetAColor(Colors::purple));
					break;
				}
				case CustomMsgType::EchoMessage:
				{
					break;
				}
				case CustomMsgType::HealthCheckServer:
				{
					tdRef.AppendMessage("HealthCheck Poll Recieved", cc.GetAColor(Colors::orange));
					PollForTermination(1); //reset termination poll
					break;
				}

				case CustomMsgType::ServerPing:
				{
					std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point then;
					msg >> then;
					std::stringstream ss;
					ss << "Ping Recieved: " << std::chrono::duration((now - then)).count() << "ms.\n";
					tdRef.AppendMessage(ss.str().c_str(), cc.GetAColor(Colors::purple));
					break;
				}
				default:
					break;
				}
			}

		}
	}

	void CheckForDisconnect()
	{
		PollForTermination(-1);
		if (client != nullptr)
		{
			// detect Timeout
			if (!client->IsConnected() || !clientValid)
			{
				std::stringstream ss;
				ss << "Client Disconnected - Trying in 4 : " << timeOutCounter;
				tdRef.AppendMessage(ss.str().c_str(), cc.GetAColor(Colors::red));
				timeOutCounter++;
			}
		
			if (timeOutCounter >= 5)
			{
				tdRef.AppendMessage("Deleting Client Socket", cc.GetAColor(Colors::red));
				clientStarted = StopClient();
				timeOutCounter = 0;
				clientValid = false;
				clientStarted = false;
			}
		}	
	}

	void AttemptReconnect()
	{
		if (client == nullptr)
		{
			StartClient();
		}
	}

	void SendMsg(std::string msg)
	{
		if (client != nullptr && client->IsConnected() && clientValid)
		{
			client->SendMsg(msg);
		}
	}

private:
	bool InitClient()
	{
		client = std::make_unique<CustomClient>();
		std::string ip = rmd.GetIP();
		uint16_t port = rmd.GetServerPort();
		client->Connect(ip.c_str(), port);
		std::stringstream ss;
		ss << "Connecting to: " << ip << " : " << port;
		tdRef.AppendMessage(ss.str().c_str(), cc.GetAColor(Colors::darkGreen));
		return true;
	}
	bool StopClient()
	{
		client->IncomingMessages().eraseQ();
		client->Disconnect();
		client.release();
		client = nullptr;
		return false;
	}
	void PollForTermination(int add_subtrct)
	{
		//send a 1 to reset, sent a -1 to get closer to fail state.
		if (client != nullptr)
		{
			if (add_subtrct >= 1)
			{
				pollCounter = 0;
			}
			else
			{
				pollCounter--;
			}

			if (pollCounter < -10 && client->IsConnected())
			{
				clientStarted = StopClient();
				pollCounter = 0;
				clientValid = false;
				clientStarted = false;
			}
		}
	}
private:
	ColorManager cc;
	LoadRMData rmd;
	TerminalData& tdRef;
	std::unique_ptr<CustomClient> client;
	int pollCounter = 0;
	bool clientValid = false;
	bool clientStarted = false;
	int timeOutCounter = 0;

};