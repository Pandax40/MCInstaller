#pragma once

#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "FileManager.h"

class DatModifier
{
private:
	static bool MakeSpace(HANDLE& hFile, int& space, char*& buffer_end, DWORD& bytesRead);
	static char* CreateServer(std::string name, std::string ip, bool textures, int& space, char n[]);
public:
	static void SetServer(std::string name, std::string ip, bool textures, std::wstring path);
};

