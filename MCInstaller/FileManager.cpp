#include "FileManager.h"

bool FileManager::GetPath(GUID folderid, string& path)
{
    path = "";
    PWSTR path_ptr;
    if (SHGetKnownFolderPath(folderid, 0, NULL, &path_ptr) == S_OK)
    {
        PWSTR aux_ptr = path_ptr;
        while (*path_ptr != 0) //Convertimos a un string
        {
            path += (char)*path_ptr;
            path_ptr = path_ptr + 1;
        }
        CoTaskMemFree(static_cast<void*>(aux_ptr)); //Liberamos memoria del puntero del string.
        return true;
    }
    cout << "[ERROR] No se ha encontrado la carpeta indicada\n";
    system("PAUSE");
    return false;
}

bool FileManager::MakeDir(string path)
{
    if (CreateDirectoryA(path.c_str(), NULL) == 0 and GetLastError() == ERROR_PATH_NOT_FOUND)
    {
        cout << "[ERROR] No se puedo crear la carpeta " + path + "\n";
        system("PAUSE");
        return false;
    }
    return true;
}

//https://learn.microsoft.com/es-es/windows/win32/shell/knownfolderid para mas FOLDERsIDs
bool FileManager::DownloadFile(string link, string name, string path)
{
    path += "\\" + name;
    if (URLDownloadToFileA(NULL, link.c_str(), path.c_str(), 0, NULL) != S_OK)
    {
        std::cout << "[ERROR] No se ha podido descargar el archivo " + name + "\n";
        system("PAUSE");
        return false;
    }
    return true;
}

bool FileManager::InstallFile(string name, string path)
{
    string aux_path = "cmd.exe /C " + path + "\\" + name;
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    char cmdLine[1024];
    strcpy_s(cmdLine, aux_path.c_str());
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (not CreateProcessA(NULL, cmdLine, NULL, NULL, false, 0, NULL, path.c_str(), &si, &pi))
    {
        cout << "[ERROR] No se ha podido abrir el archivo " + name + "\n";
        system("PAUSE");
        return false;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}