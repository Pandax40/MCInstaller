#ifndef FILEMANAGER_HPP
#define FILEMANAGER_HPP

#include <iostream>
#include <Windows.h>
#include <Shlobj.h>
#pragma comment(lib, "Urlmon.lib")

using namespace std;

class FileManager
{
public:
	static bool MakeDir(string path);
	static bool DownloadFile(string link, string name, string path);
	static bool InstallFile(string name, string path);
	static bool GetPath(GUID fast_path, string& path);
};

#endif