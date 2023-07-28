#include "DatModifier.h"

using std::string;

/*bool DatModifier::MakeSpace(HANDLE& hFile, int& space, char*& buffer, DWORD& bytesRead)
{
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize); //Obtenemos el size del archivo.

	DWORD remainingBytes = fileSize.QuadPart - 18; //Cuantos bytes quedan hasta el final del archivo.

	buffer = new char[remainingBytes];
	bytesRead = -1;

	if (not ReadFile(hFile, buffer, remainingBytes, &bytesRead, NULL) or remainingBytes != bytesRead)
	{
		std::cout << "[ERROR] Desconocido al leer servers.dat\n";
		return false;
	}
	if (bytesRead == 1)
	{
		SetFilePointer(hFile, 13, NULL, FILE_BEGIN);
		uint16_t ipSize = 0x0A;
		WriteFile(hFile, &ipSize, 1, NULL, NULL);
	}
	SetFilePointer(hFile, space, NULL, FILE_END);
	SetEndOfFile(hFile);
	SetFilePointer(hFile, 17, NULL, FILE_BEGIN); //Vuelves a la posicion
	return true;
}

char* DatModifier::CreateServer(string name, string ip, bool textures, int& space, char n[])
{
	space = 1 + 17 + ip.size() + 9 + name.size() + 1;
	char* buffer = new char[space + 1];
	memcpy(buffer, n, 1);
	char bytes16[] = {0x01, 0x00, 0x06, 0x68, 0x69, 0x64, 0x64, 0x65, 0x6E, 0x00, 0x08, 0x00, 0x02, 0x69, 0x70, 0x00};
	memcpy(buffer + 1, bytes16, 16);
	size_t ipSize = ip.size();
	memcpy(buffer + 17, &ipSize, 1);
	memcpy(buffer + 18, ip.c_str(), ip.size());
	char bytes8[] = { 0x08, 0x00, 0x04, 0x6E, 0x61, 0x6D, 0x65, 0x00 };
	memcpy(buffer + 18 + ip.size(), bytes8, 8);
	size_t nameSize = name.size();
	memcpy(buffer + 26 + ip.size(), &nameSize, 1);
	memcpy(buffer + 27 + ip.size(), name.c_str(), name.size());
	char bytes1[] = { 0x00 };
	memcpy(buffer + 27 + ip.size() + name.size(), bytes1, 1);
	return buffer;
}

bool Exist(HANDLE& hFile, string& ip)
{
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);
	char* buffer = new char[fileSize.QuadPart];

	DWORD bytesRead;
	ReadFile(hFile, buffer, fileSize.QuadPart, &bytesRead, NULL);
	string fileContent(buffer, bytesRead);
	delete[] buffer;
	size_t found = fileContent.find(ip);
	if (found != std::string::npos) 
		return true;
	SetFilePointer(hFile, 18, NULL, FILE_BEGIN);
	return false;
}

void DatModifier::SetServer(string name, string ip, bool textures, std::string path)
{
	path += L"\\.minecraft\\servers.dat";
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		char head[] = { 0x0A, 0x00, 0x00, 0x09, 0x00, 0x07, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		WriteFile(hFile, &head, 19, NULL, NULL);
	}
	char n[1];
	SetFilePointer(hFile, 17, NULL, FILE_BEGIN);
	if (not ReadFile(hFile, &n, 1, NULL, NULL))
	{
		std::cout << "[ERROR] Desconocido al leer servers.dat\n";
	}
	else 
	{
		if (not Exist(hFile, ip))
		{
			int space;
			++n[0];
			char* server = CreateServer(name, ip, false, space, n);
			char* buffer_aux;
			DWORD bytesRead;
			--space;
			if (MakeSpace(hFile, space, buffer_aux, bytesRead))
			{
				++space;
				WriteFile(hFile, server, space, NULL, NULL);
				WriteFile(hFile, buffer_aux, bytesRead, NULL, NULL);
			}
		}
	}
	CloseHandle(hFile);
}*/