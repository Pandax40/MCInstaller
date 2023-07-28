#include "JavaInstaller.h"
#include <iostream>
#include <windows.h>

int javaVersion()
{
    char value[32];
    DWORD size = sizeof(value);
    HKEY hKey;
    if (ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\JavaSoft\\JDK", &hKey) and
        ERROR_SUCCESS == RegGetValueA(hKey, NULL, "CurrentVersion", RRF_RT_REG_SZ, NULL, &value, &size))
        return (*value - '0') * 10 + (*(value + 1) - '0'); //char value --> int javaVer
    return 0;
}