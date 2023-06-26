// MCInstaller.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <Windows.h>
#include "FileManager.h"
#pragma comment(lib, "Shlwapi.lib")
#include <shlwapi.h>
#include "DatModifier.h"

//https://learn.microsoft.com/es-es/windows/win32/shell/knownfolderid para mas FOLDERsIDs
#define JAVA_LINK "https://download.oracle.com/java/20/latest/jdk-20_windows-x64_bin.exe"
#define MC_LINK "https://skmedix.pl/data/SKlauncher%203.1.exe"
#define FORGE_LINK "https://maven.minecraftforge.net/net/minecraftforge/forge/1.20-46.0.14/forge-1.20-46.0.14-installer.jar"
#define FORGE_DIR "1.20-forge-46.0.14"

#define JEI_LINK "https://mediafilez.forgecdn.net/files/4581/323/jei-1.20-forge-14.0.0.11.jar"
#define JOURNEYMAP_LINK "https://mediafilez.forgecdn.net/files/4580/627/journeymap-1.20-5.9.8-forge.jar"
#define SPARK_LINK "https://mediafilez.forgecdn.net/files/4587/309/spark-1.10.42-forge.jar"
#define VOICE_LINK "https://mediafilez.forgecdn.net/files/4574/217/voicechat-forge-1.20-2.4.9.jar"
#define COMPATIBLE_LINK "https://mediafilez.forgecdn.net/files/4580/511/MyServerIsCompatible-1.20-1.0.jar"
#define ZOOM_LINK "https://mediafilez.forgecdn.net/files/4576/19/okzoomer-forge-1.20-3.0.0.jar"
#define SERVER_NAME "TFF"
#define SERVER_IP "tff.sytes.net"
#define TEXTURES true

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

/*
* TODO:
*  - Poder elegir cuanta ram se dedica al Minecraft.
*  - ERROR: Si el usuario tiene un espacio en el nombre, se bugea
*  - Hacer que hacepte lo de las texturas.
*  - Poner los links de los mods en array o lista para poder leer facilmente en un while.
*/

bool have_java(int version)
{
    SetConsoleTextAttribute(hConsole, 4);
    char value[32];
    DWORD size = sizeof(value);
    HKEY hKey;
    //Si java esta instalado:
    if (ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\JavaSoft\\JDK", &hKey) and
        ERROR_SUCCESS == RegGetValueA(hKey, NULL, "CurrentVersion", RRF_RT_REG_SZ, NULL, &value, &size))
    {
        int javaVer = (*(value)-'0') * 10 + (*(value + 1) - '0'); //char value --> int javaVer
        if (javaVer >= version)
        {
            SetConsoleTextAttribute(hConsole, 7);
            std::cout << "[INFO] Java version " << value << " detectado\n";
            return true;
        }
        std::cout << "[ERROR] La version de Java es " << value << " se necesita la version 17 o posterior\n";
    }
    else
        std::cout << "[ERROR] Java no esta instalado\n";
    
    return false;
}

int main()
{   
    int left_pos = GetSystemMetrics(SM_CXSCREEN) * 0.16;
    SetWindowPos(GetConsoleWindow(), NULL, left_pos, 0, 1260, 300, SWP_SHOWWINDOW);
    SetConsoleTextAttribute(hConsole, 1);
    std::cout << " /$$$$$$$$/$$                       /$$$$$$$$/$$                     /$$       /$$$$$$                      /$$              /$$ /$$                    " << "\n";
    std::cout << "|__  $$__/ $$                      | $$_____/__/                    | $$      |_  $$_/                     | $$             | $$| $$                    " << "\n";
    std::cout << "   | $$  | $$$$$$$   /$$$$$$       | $$      /$$ /$$$$$$$   /$$$$$$ | $$        | $$   /$$$$$$$   /$$$$$$$/$$$$$$   /$$$$$$ | $$| $$  /$$$$$$   /$$$$$$ " << "\n";
    std::cout << "   | $$  | $$__  $$ /$$__  $$      | $$$$$  | $$| $$__  $$ |____  $$| $$        | $$  | $$__  $$ /$$_____/_  $$_/  |____  $$| $$| $$ /$$__  $$ /$$__  $$" << "\n";
    std::cout << "   | $$  | $$  \\ $$| $$$$$$$$      | $$__/  | $$| $$  \\ $$  /$$$$$$$| $$        | $$  | $$  \\ $$|  $$$$$$  | $$     /$$$$$$$| $$| $$| $$$$$$$$| $$  \\__/" << "\n";
    std::cout << "   | $$  | $$  | $$| $$_____/      | $$     | $$| $$  | $$ /$$__  $$| $$        | $$  | $$  | $$ \\____  $$ | $$ /$$/$$__  $$| $$| $$| $$_____/| $$      " << "\n";
    std::cout << "   | $$  | $$  | $$|  $$$$$$$      | $$     | $$| $$  | $$|  $$$$$$$| $$       /$$$$$$| $$  | $$ /$$$$$$$/ |  $$$$/  $$$$$$$| $$| $$|  $$$$$$$| $$      " << "\n";
    std::cout << "   |__/  |__/  |__/ \\_______/      |__/     |__/|__/  |__/ \\_______/|__/      |______/|__/  |__/|_______/   \\___/  \\_______/|__/|__/ \\_______/|__/      " << "\n" << "\n";
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << "                                        <|> Instalador del servidor 'The Final Frontier' creado por ";
    SetConsoleTextAttribute(hConsole, 11);
    std::cout << "Pandax40";
    SetConsoleTextAttribute(hConsole, 3);
    std::cout << " <|>\n\n";
    Sleep(2000);
    SetConsoleTextAttribute(hConsole, 4);
    string t_path;
    if (not FileManager::GetPath(FOLDERID_LocalAppData, t_path))
        return 0;
    t_path += "\\Temp\\mcinstaller";
    while (not have_java(17))
    {
        Sleep(2000); 
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "[INFO] Descargando la version de Java 20, espere\n";
        SetConsoleTextAttribute(hConsole, 4);
        if (not FileManager::MakeDir(t_path))
            return 0;
        if(not FileManager::DownloadFile(JAVA_LINK, "java-20.exe", t_path))
            return 0;
        SetConsoleTextAttribute(hConsole, 7);
        std::cout << "[INFO] Sigue los pasos del instalador de Java\n";
        if (not FileManager::InstallFile("java-20.exe", t_path))
            return 0;
    }
    Sleep(2000);
    string mc_path;
    if (not FileManager::GetPath(FOLDERID_RoamingAppData, mc_path))
        return 0;
    mc_path += "\\.minecraft";
    bool sky_installed = false;
    if (not PathIsDirectoryA(mc_path.c_str()))
    {
        SetConsoleTextAttribute(hConsole, 4);
        std::cout << "[ERROR] Minecraft no esta instalado!\n";
        Sleep(2000);
        SetConsoleTextAttribute(hConsole, 5);
        std::cout << "[WARN] Cuando SKLauncher se habra, cierralo\n";
        Sleep(3000);
        string d_path;
        if (not FileManager::GetPath(FOLDERID_Desktop, d_path))
            return 0;
        if (not FileManager::DownloadFile(MC_LINK, "Minecraft.exe", d_path))
            return 0;
        if (not FileManager::InstallFile("Minecraft.exe", d_path))
            return 0;
        sky_installed = true;
        //COMPROVAR QUE .MINECRAFT ESTA.
    }
    SetConsoleTextAttribute(hConsole, 4);
    mc_path += "\\mods";
    if (not PathIsDirectoryA(mc_path.c_str()))
    {
        if (not FileManager::MakeDir(mc_path))
            return 0;
    }
    else
    {
        WIN32_FIND_DATAA data;
        HANDLE hFind = FindFirstFileA((mc_path + "\\*.jar").c_str(), &data);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                string filePath = mc_path + "\\" + string(data.cFileName);
                DeleteFileA(filePath.c_str());
            } while (FindNextFileA(hFind, &data) != 0);
            FindClose(hFind);
        }
    }
    if (not FileManager::DownloadFile(JEI_LINK, "jei.jar", mc_path))
        return 0;

    if (not FileManager::DownloadFile(JOURNEYMAP_LINK, "journeymap.jar", mc_path))
        return 0;

    if (not FileManager::DownloadFile(SPARK_LINK, "spark.jar", mc_path))
        return 0;

    if (not FileManager::DownloadFile(VOICE_LINK, "voice-chat.jar", mc_path))
        return 0;

    if (not FileManager::DownloadFile(COMPATIBLE_LINK, "is-compatible.jar", mc_path))
        return 0;

    if (not FileManager::DownloadFile(ZOOM_LINK, "zoom.jar", mc_path))
        return 0;

    SetConsoleTextAttribute(hConsole, 7);
    cout << "[INFO] Se han instalado los mods correctamente\n";
    
    SetConsoleTextAttribute(hConsole, 4);
    DatModifier::SetServer(SERVER_NAME, SERVER_IP, TEXTURES);
    Sleep(2000);

    if (not FileManager::GetPath(FOLDERID_RoamingAppData, mc_path))
        return 0;

    mc_path += "\\.minecraft\\versions\\";
    mc_path += FORGE_DIR;
    if (not PathIsDirectoryA(mc_path.c_str()))
    {
        SetConsoleTextAttribute(hConsole, 5);
        cout << "[WARN] Espera a que se abra el Forge e Instalalo en la carpeta por defecto!\n";
        Sleep(2000);

        if (not FileManager::DownloadFile(FORGE_LINK, "forge.jar", t_path))
            return 0;
        if (not FileManager::InstallFile("forge.jar", t_path))
            return 0;
    }
    if (sky_installed)
    {
        SetConsoleTextAttribute(hConsole, 5);
        std::cout << "[WARN] Como usar SKLauncher:\n";
        Sleep(2000);
        std::cout << "[WARN]   - Cambia a modo sin conexion, si no has comprado el minecraft\n";
        Sleep(2000);
        std::cout << "[WARN]   - Introduce el nombre que tendras en el juego\n";
        Sleep(2000);
        std::cout << "[WARN]   - Dale a 'Inicia sesion sin conexion'\n";
        Sleep(2000);
        std::cout << "[WARN]   - Selecciona 'Forge' y dale a Jugar\n";
        Sleep(2000);
        system("PAUSE");
    }

    SetConsoleTextAttribute(hConsole, 7);
    return 1;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
