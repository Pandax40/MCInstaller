#pragma once
#include <string>

namespace Constants
{
    constexpr std::wstring_view JAVA_LINK = L"https://download.oracle.com/java/20/latest/jdk-20_windows-x64_bin.exe";
    constexpr std::wstring_view MC_LINK = L"https://skmedix.pl/_data/SKlauncher-3.1.exe";
    constexpr std::wstring_view MC_JSON_DEF = L"https://gitfront.io/r/Pandax40/raVGmnGL3ad1/MCInstaller/raw/JSON/launcher_profiles.json";
    constexpr std::wstring_view FORGE_LINK = L"https://maven.minecraftforge.net/net/minecraftforge/forge/1.20-46.0.14/forge-1.20-46.0.14-installer.jar";
    const wchar_t* FORGE_DIR = L"1.20-forge-46.0.14";
    constexpr int MIN_JAVA_VERSION = 17;
    constexpr std::wstring_view MODS[6] =
    {
        L"https://mediafilez.forgecdn.net/files/4581/323/jei-1.20-forge-14.0.0.11.jar",
        L"https://mediafilez.forgecdn.net/files/4580/627/journeymap-1.20-5.9.8-forge.jar",
        L"https://mediafilez.forgecdn.net/files/4574/217/voicechat-forge-1.20-2.4.9.jar",
        L"https://mediafilez.forgecdn.net/files/4580/511/MyServerIsCompatible-1.20-1.0.jar",
        L"https://mediafilez.forgecdn.net/files/4576/19/okzoomer-forge-1.20-3.0.0.jar",
        L"https://mediafilez.forgecdn.net/files/4587/309/spark-1.10.42-forge.jar"
    };
    constexpr std::wstring_view MODS_NAME[6] =
    {
        L"jei-1.20.jar",
        L"hjourneymap-1.20.jar",
        L"voicechat-forge-1.20.jar",
        L"MyServerIsCompatible-1.20.jar",
        L"okzoomer-forge-1.20.jar",
        L"spark-forge.jar"
    };
    constexpr int NUM_MODS = 6;
    constexpr std::string_view SERVER_NAME = "TFF";
    constexpr std::string_view SERVER_IP = "tff.sytes.net";
    constexpr bool TEXTURES = true;
}
