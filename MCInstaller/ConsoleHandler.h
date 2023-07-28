#pragma once

#include<iostream>
#include<windows.h>

class ConsoleHandler
{
	inline static bool isHandled = false;
	HANDLE hConsole;

	enum colors
	{
		BLUE = FOREGROUND_BLUE,
		GREEN = FOREGROUND_GREEN,
		RED = FOREGROUND_RED,
		PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
		LIGHT_BLUE = FOREGROUND_GREEN | FOREGROUND_BLUE,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	};

public:
	enum messageType { tINFO, tWARN, tERROR};
	
	ConsoleHandler();
	void Print(std::string_view text, messageType type) const;

};

