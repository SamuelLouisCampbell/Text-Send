#pragma once
#include <wx/wx.h>
#include <wx/colour.h>
#include <chrono>
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include "ListsAndColors.h"

struct ReadOut
{
	wxString message;
	wxColor colour;
	size_t messageStart;
	size_t messageEnd = 1000;
};

class TerminalData
{
public:
	void ClearMessages();
	void AppendMessage(const wxString str, const wxColor& col);
	size_t GetNoMessages() const;
	ReadOut GetLastMessage()const;
	std::vector<ReadOut>& GetMessages();


private:
	std::string return_current_time_and_date();
	std::vector<ReadOut> terminalMessages;

};