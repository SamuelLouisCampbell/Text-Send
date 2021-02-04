#include "Terminal.h"


void TerminalData::ClearMessages()
{
	terminalMessages.clear();
}

void TerminalData::AppendMessage(const wxString str, const wxColor& col)
{
	wxString temp = return_current_time_and_date() + " " + str;
	if (terminalMessages.size() == 0)
	{
		terminalMessages.push_back({ temp, col, 0, temp.size()});
	}
	else
	{
		size_t lc = GetLastMessage().messageEnd;
		terminalMessages.push_back({ temp, col, lc + 1, lc + temp.size() + 1});
	}
}

size_t TerminalData::GetNoMessages() const
{
	return terminalMessages.size();
}

ReadOut TerminalData::GetLastMessage() const
{
	return terminalMessages.back();
}

std::vector<ReadOut>& TerminalData::GetMessages()
{
	if (terminalMessages.size() > 5)
	{
		terminalMessages.clear();
	}
	return terminalMessages;
}

std::string TerminalData::return_current_time_and_date()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%X");
	return ss.str();
}
