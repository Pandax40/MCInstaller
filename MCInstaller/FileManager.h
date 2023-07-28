#pragma once 

#include <iostream>
#include <Windows.h>
#include <Shlobj.h>
#include <sstream>
#pragma comment(lib, "Urlmon.lib")

#include "ConsoleHandler.h"

class FileManager
{
	std::wstring path;
	bool isError = false;
	bool MakeDir(const std::wstring& folder);
public:
	void SetPath(const ConsoleHandler& console, GUID fast_path, const wchar_t* folders[], int size);
	void DownloadFile(const ConsoleHandler& console, const std::wstring& link, const std::wstring& name);
	void InstallFile(const ConsoleHandler& console, const std::wstring& link, const std::wstring& name);
	void ExecuteFile(const ConsoleHandler& console, const std::wstring& name);
	bool failed() const;
	std::wstring GetActualPath() const;
};