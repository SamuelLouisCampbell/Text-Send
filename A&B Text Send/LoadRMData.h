#pragma once
#include "UDPClient.h"
#include <fstream>

class LoadRMData
{
public:
	LoadRMData(const char* fileName)
	{
		std::string line;
		std::string::size_type n;
		std::ifstream defFile(fileName);
		if (defFile.is_open())
		{
			//serverPort
			std::getline(defFile, line);
			n = line.find("#");
			std::string substr = line.substr(n + 1, line.size());
			serverPort = std::stoi(substr);
			//clientPort
			std::getline(defFile, line);
			n = line.find("#");
			substr = line.substr(n + 1, line.size());
			clientPort = std::stoi(substr);
			//IP
			std::getline(defFile, line);
			n = line.find("#");
			clientIP = line.substr(n + 1, line.size());
			//close out
			defFile.close();
			fileReadOk = true;
		}
		else
		{
			serverPort = 5000U;
			clientPort = 6000U;
			clientIP = "127.0.0.1";
			fileReadOk = false;
		}
	}
	bool CheckRMIPGood()
	{
		const char* addrBuff; // Thrown away here :-(
		if (inet_pton(AF_INET, clientIP.c_str(), &addrBuff) != 1)
			return false;
		else
			return true;
	}
	bool CheckRMPortsGood()
	{
		if (serverPort == clientPort)
			return false;
		if (serverPort >= std::numeric_limits<unsigned short>::max() || serverPort <= 0U)
			return false;
		if (clientPort >= std::numeric_limits<unsigned short>::max() || clientPort <= 0U)
			return false;
		else return true;
	}
	unsigned short GetClientPort() const
	{
		return unsigned short(clientPort);
	}
	unsigned short GetServerPort() const
	{
		return unsigned short(serverPort);
	}
	const char* GetIP() const
	{
		return clientIP.c_str();
	}
	void SetPortToDefault()
	{
		clientPort = 6000U;
		serverPort = 5000U;
	}
	void SetIPToDefault()
	{
		clientIP = "127.0.0.1";
	}
	bool FileReadOK() const
	{
		return fileReadOk;
	}
private:
	bool fileReadOk = false;
	size_t clientPort;
	size_t serverPort;
	std::string clientIP;
};