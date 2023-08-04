// MCInstaller.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.

#include <iostream>
#include <Windows.h>
#pragma comment(lib, "Shlwapi.lib")
#include <shlwapi.h>
#include "FileManager.h"
#include "DatModifier.h"
#include "ConsoleHandler.h"
#include "Const.h"

using std::wstring_view, std::string_view, std::wstring, std::string;

/*
* TODO:
*  - Poder elegir cuanta ram se dedica al Minecraft.
*  - Hacer que hacepte lo de las texturas.
*/

int javaVersion()
{
    char value[32];
    DWORD size = sizeof(value);
    HKEY hKey;
    if (ERROR_SUCCESS == RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\JavaSoft\\JDK", 0, KEY_READ | KEY_WOW64_64KEY, &hKey) and
        ERROR_SUCCESS == RegGetValueA(hKey, NULL, "CurrentVersion", RRF_RT_REG_SZ, NULL, &value, &size))
        return (*value - '0') * 10 + (*(value + 1) - '0'); //char value --> int javaVer
    return 0;
}

static void CleanModsDir(const FileManager& fileManager)
{
    WIN32_FIND_DATAW data;
    HANDLE hFind = FindFirstFileW((fileManager.GetActualPath() + L"\\*.jar").c_str(), &data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            wstring filePath = fileManager.GetActualPath() + L'\\' + data.cFileName;
            DeleteFileW(filePath.c_str());
        } while (FindNextFileW(hFind, &data) != 0);
        FindClose(hFind);
    }
}

int main()
{   
    ConsoleHandler console = ConsoleHandler();
    FileManager fileManager;
    
    //Comprobar Java
    const wchar_t* foldersTemp[] = {L"Temp", L"mcinstaller"};
    for (int i = 0; javaVersion() < Constants::MIN_JAVA_VERSION; ++i)
    {
        if (i > 0)
        {
            if (i == 3)
            {
                console.Print("CONTACTA POR DISCORD CON \"pandax40\"", ConsoleHandler::tERROR);
                return EXIT_FAILURE;
            }
            else
            {
                console.Print("No se ha podido detectar la version de Java", ConsoleHandler::tWARN);
                Sleep(2000);
                console.Print("Prueba a instalar Java en la carpeta por defecto", ConsoleHandler::tWARN);
            }
        }
        Sleep(2000);
        console.Print("Sigue los pasos del instalador. Descargando Java 20...", ConsoleHandler::tINFO);
        fileManager.SetPath(console, FOLDERID_LocalAppData, foldersTemp, sizeof(foldersTemp));
        fileManager.InstallFile(console, (wstring)Constants::JAVA_LINK, L"java-20.exe");
        if (fileManager.Failed()) 
            return EXIT_FAILURE;
    }

    //Comprobar Minecraft
    Sleep(2000);
    fileManager.SetPath(console, FOLDERID_RoamingAppData, NULL, NULL);
    if (fileManager.Failed()) 
        return EXIT_FAILURE;
    bool sky_installed = false;
    if (!PathIsDirectoryW((LPCWSTR)(fileManager.GetActualPath() + L'\\' + L".minecraft").c_str()))
    {
        console.Print("Descargando Minecraft...", ConsoleHandler::tWARN);
        Sleep(3000);
        
        //Descarga Minecraft en Desktop
        fileManager.SetPath(console, FOLDERID_Desktop, NULL, NULL);
        fileManager.DownloadFile(console, (wstring)Constants::MC_LINK, L"Minecraft.exe", true);
        if (fileManager.Failed()) 
            return EXIT_FAILURE;

        //Creacion del archivo launcher_profiles.json necesario para Forge.jar
        const wchar_t* foldersMC[] = { L".minecraft" };
        fileManager.SetPath(console, FOLDERID_RoamingAppData, foldersMC, sizeof(foldersMC));
        fileManager.DownloadFile(console, (wstring)Constants::MC_JSON_DEF, L"launcher_profiles.json", false);
        if (fileManager.Failed()) 
            return EXIT_FAILURE;
        sky_installed = true;
    }
    
    //Instalar Mods
    const wchar_t* foldersMods[] = { L".minecraft", L"mods"};
    fileManager.SetPath(console, FOLDERID_RoamingAppData, foldersMods, sizeof(foldersMods));
    CleanModsDir(fileManager);
    for (int i = 0; i < Constants::NUM_MODS; ++i)
        fileManager.DownloadFile(console, (wstring)Constants::MODS[i], (wstring)Constants::MODS_NAME[i], false);
    if (fileManager.Failed())
        return EXIT_FAILURE;
    console.Print("Se han instalado los mods correctamente", ConsoleHandler::tINFO);
    
    //Añadir el servidor a la lista
    fileManager.SetPath(console, FOLDERID_RoamingAppData, NULL, NULL);
    DatModifier::SetServer((string)Constants::SERVER_NAME, (string)Constants::SERVER_IP, Constants::TEXTURES, fileManager.GetActualPath());
    Sleep(2000);

    //Comprobar Forge
    const wchar_t* foldersVersion[] = { L".minecraft", L"versions"};
    fileManager.SetPath(console, FOLDERID_RoamingAppData, foldersVersion, sizeof(foldersVersion));
    if (!PathIsDirectoryW((fileManager.GetActualPath() + L'\\' + Constants::FORGE_DIR).c_str()))
    {
        console.Print("Instala Forge en la carpeta por DEFECTO. Descargando Forge...", ConsoleHandler::tWARN);
        Sleep(2000);
        fileManager.SetPath(console, FOLDERID_LocalAppData, foldersTemp, sizeof(foldersTemp));
        fileManager.InstallFile(console, (wstring)Constants::FORGE_LINK, L"forge.jar");
        if(fileManager.Failed()) 
            return EXIT_FAILURE;
    }
    
    //Instrucciones del SK Launcher
    if (sky_installed)
    {
        console.Print("Como usar SKLauncher:", ConsoleHandler::tINFO);
        console.Print("  - Cambia a modo sin conexion, si no has comprado el minecraft", ConsoleHandler::tINFO);
        console.Print("  - Introduce el nombre que tendras en el juego", ConsoleHandler::tINFO);
        console.Print("  - Dale a 'Inicia sesion sin conexion'", ConsoleHandler::tINFO);
        console.Print("  - Selecciona 'Forge' y dale a Jugar", ConsoleHandler::tINFO);
        Sleep(3000);

        //Abre el SK Launcher del escritorio
        fileManager.SetPath(console, FOLDERID_Desktop, NULL, NULL);
        fileManager.ExecuteFile(console, L"Minecraft.exe");
        if (fileManager.Failed()) return EXIT_FAILURE;
        
        Sleep(20000); //Pausa para leer las instrucciones
        system("PAUSE");
    }

    return EXIT_SUCCESS;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
