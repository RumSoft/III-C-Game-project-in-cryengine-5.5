#pragma once

#include <map>
#include <CryString/CryString.h>

class Logger
{
	Logger() = default;
	std::map<string, std::map<int, string>> logs;
public:
	static Logger& Get()
	{
		static auto* instance = new Logger();
		return *instance;
	}

	void Log(const string& source, const string& text, const int line = 0)
	{
		logs[source][line] = text;
	}

	string ReadLog()
	{
		string str;
		for (auto& source : logs)
			for (auto& line : source.second)
			{
				str.append(source.first);
				str.append(": ");
				str.append(line.second);
				str.append("\n");
			}
		return str;
	}
};