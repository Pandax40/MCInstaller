// MCInstaller.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.

#include <iostream>
#include <Windows.h>
#pragma comment(lib, "Shlwapi.lib")
#include <shlwapi.h>
#include "FileManager.h"
#include "DatModifier.h"
#include "ConsoleHandler.h"
using std::wstring_view, std::string_view, std::wstring, std::string;

//https://learn.microsoft.com/es-es/windows/win32/shell/knownfolderid para mas FOLDERsIDs
constexpr wstring_view JAVA_LINK = L"https://download.oracle.com/java/20/latest/jdk-20_windows-x64_bin.exe";
constexpr wstring_view MC_LINK = L"https://skmedix.pl/_data/SKlauncher-3.1.exe";
constexpr wstring_view MC_JSON_DEF = L"https://gitfront.io/r/Pandax40/raVGmnGL3ad1/MCInstaller/raw/JSON/launcher_profiles.json";
constexpr wstring_view FORGE_LINK = L"https://maven.minecraftforge.net/net/minecraftforge/forge/1.20-46.0.14/forge-1.20-46.0.14-installer.jar";
const wchar_t * FORGE_DIR = L"1.20-forge-46.0.14";
constexpr int MIN_JAVA_VERSION = 17;
constexpr wstring_view MODS[6] =
{
    L"https://mediafilez.forgecdn.net/files/4581/323/jei-1.20-forge-14.0.0.11.jar",
    L"https://mediafilez.forgecdn.net/files/4580/627/journeymap-1.20-5.9.8-forge.jar",
    L"https://mediafilez.forgecdn.net/files/4574/217/voicechat-forge-1.20-2.4.9.jar",
    L"https://mediafilez.forgecdn.net/files/4580/511/MyServerIsCompatible-1.20-1.0.jar",
    L"https://mediafilez.forgecdn.net/files/4576/19/okzoomer-forge-1.20-3.0.0.jar",
    L"https://mediafilez.forgecdn.net/files/4587/309/spark-1.10.42-forge.jar"
};
constexpr wstring_view MODS_NAME[6] =
{
    L"jei-1.20.jar",
    L"hjourneymap-1.20.jar",
    L"voicechat-forge-1.20.jar",
    L"MyServerIsCompatible-1.20.jar",
    L"okzoomer-forge-1.20.jar",
    L"spark-forge.jar"
};
constexpr int numMods = 6;
constexpr string_view SERVER_NAME = "TFF";
constexpr string_view SERVER_IP = "tff.sytes.net";
constexpr bool TEXTURES = true;

/*
* TODO:
*  - Poder elegir cuanta ram se dedica al Minecraft.
*  - Hacer que hacepte lo de las texturas.
*  - Implementar una barra de carga de descarga.
* 
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
    int javaChecks = 0;
    while (javaVersion() < MIN_JAVA_VERSION)
    {
        if (javaChecks > 0)
        {
            if (javaChecks == 3)
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
        fileManager.InstallFile(console, (wstring)JAVA_LINK, L"java-20.exe");
        if (fileManager.Failed()) return EXIT_FAILURE;
        ++javaChecks;
    }

    //Comprobar Minecraft
    Sleep(2000);
    const wchar_t* foldersMC[] = { L".minecraft"};
    fileManager.SetPath(console, FOLDERID_RoamingAppData, NULL, NULL);
    if (fileManager.Failed()) return EXIT_FAILURE;
    bool sky_installed = false;
    if (!PathIsDirectoryW((LPCWSTR)(fileManager.GetActualPath() + L'\\' + L".minecraft").c_str()))
    {
        console.Print("Descargando Minecraft...", ConsoleHandler::tWARN);
        Sleep(3000);
        
        fileManager.SetPath(console, FOLDERID_Desktop, NULL, NULL);
        fileManager.DownloadFile(console, (wstring)MC_LINK, L"Minecraft.exe", true);
        if (fileManager.Failed()) return EXIT_FAILURE;

        const wchar_t* foldersMC[] = { L".minecraft" };
        fileManager.SetPath(console, FOLDERID_RoamingAppData, foldersMC, sizeof(foldersMC));
        fileManager.DownloadFile(console, (wstring)MC_JSON_DEF, L"launcher_profiles.json", false);
        if (fileManager.Failed()) return EXIT_FAILURE;
        
        sky_installed = true;
    }
    
    //Instalar Mods
    const wchar_t* foldersMods[] = { L".minecraft", L"mods"};
    fileManager.SetPath(console, FOLDERID_RoamingAppData, foldersMods, sizeof(foldersMods));
    if (fileManager.Failed()) return EXIT_FAILURE;
    CleanModsDir(fileManager);
    for (int i = 0; i < numMods; ++i)
    {
        fileManager.DownloadFile(console, (wstring)MODS[i], (wstring)MODS_NAME[i], false);
    }
    if (fileManager.Failed()) return EXIT_FAILURE;
    console.Print("Se han instalado los mods correctamente", ConsoleHandler::tINFO);
    
    //Añadir el servidor a la lista
    fileManager.SetPath(console, FOLDERID_RoamingAppData, NULL, NULL);
    DatModifier::SetServer((string)SERVER_NAME, (string)SERVER_IP, TEXTURES, fileManager.GetActualPath());
    Sleep(2000);

    //Comprobar Forge
    const wchar_t* foldersVersion[] = { L".minecraft", L"versions"};
    fileManager.SetPath(console, FOLDERID_RoamingAppData, foldersVersion, sizeof(foldersVersion));
    if (fileManager.Failed()) return EXIT_FAILURE;
    if (!PathIsDirectoryW((fileManager.GetActualPath() + L'\\' + FORGE_DIR).c_str()))
    {
        console.Print("Instala Forge en la carpeta por DEFECTO. Descargando Forge...", ConsoleHandler::tWARN);
        Sleep(2000);
        fileManager.SetPath(console, FOLDERID_LocalAppData, foldersTemp, sizeof(foldersTemp));
        fileManager.InstallFile(console, (wstring)FORGE_LINK, L"forge.jar");
        
        if(fileManager.Failed()) return EXIT_FAILURE;
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
