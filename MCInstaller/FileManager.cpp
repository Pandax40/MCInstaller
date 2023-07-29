#include "FileManager.h"
#include "DowloadStatus.h"

using std::wstring, std::string;

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void FileManager::SetPath(const ConsoleHandler& console, GUID folderid, const wchar_t* folders[], int size)
{
    PWSTR folderPath;
    if (SHGetKnownFolderPath(folderid, 0, NULL, &folderPath) != S_OK)
    {
        console.Print("No se ha podido encontrar el id de la carpeta", ConsoleHandler::tERROR);
        isError = true;
        return;
    }
    std::wostringstream wos;
    wos << folderPath;
    CoTaskMemFree(folderPath); //Liberamos memoria del puntero del string.
    size /= sizeof(wchar_t*);
    for (int i = 0; i < size; ++i)
    {
        wos << '\\' << folders[i];
        if (!CreateDirectoryW(wos.str().c_str(), NULL) and GetLastError() == ERROR_PATH_NOT_FOUND)
        {
            console.Print("No se ha podido crear la carpeta", ConsoleHandler::tERROR);
            isError = true;
            return;
        }
    }
    path = wos.str();
}

void FileManager::DownloadFile(const ConsoleHandler& console, const wstring& link, const wstring& name, bool print)
{
    if (isError) return;

    wstring newPath = path + L'\\' + name;
    DowloadStatus status;
    status.printBar = print;
    ShowConsoleCursor(false);
    if (URLDownloadToFileW(NULL, link.c_str(), newPath.c_str(), 0, &status) != S_OK)
    {
        ShowConsoleCursor(true);
        console.Print("No se ha podido descargar el archivo", ConsoleHandler::tERROR);
        isError = true;
    }
    ShowConsoleCursor(true);
}

void FileManager::InstallFile(const ConsoleHandler& console, const wstring& link, const wstring& name)
{
    if (isError) return;

    DownloadFile(console, link, name, true);
    ExecuteFile(console, name);
}

void FileManager::ExecuteFile(const ConsoleHandler& console, const wstring& name)
{
    if (isError) return;
    wstring newPath = path + L'\\' + name;
    wstring command = L"cmd.exe /C \"" + newPath + L'\"';

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (not CreateProcessW(NULL, (LPWSTR)command.c_str(), NULL, NULL, false, 0, NULL, path.c_str(), &si, &pi))
    {
        console.Print("No se ha podido abrir el archivo", ConsoleHandler::tERROR);
        isError = true;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool FileManager::Failed() const
{
    return isError;
}

wstring FileManager::GetActualPath() const
{
    return isError ? wstring() : path;
}
