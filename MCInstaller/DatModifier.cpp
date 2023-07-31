//AVISO: Este codigo es una shit, no me gusta nada. Porque se hizo rapido para poder hacer la Release. Hay reimplementarlo. 

#include "DatModifier.h"

using std::string;

bool DatModifier::MakeSpace(HANDLE& hFile, int& space, char*& buffer, DWORD& bytesRead)
{
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize); //Obtenemos el size del archivo.

	DWORD remainingBytes = DWORD(fileSize.QuadPart - 18); //Cuantos bytes quedan hasta el final del archivo.

	buffer = new char[remainingBytes];
	bytesRead = -1;

	if (not ReadFile(hFile, buffer, remainingBytes, &bytesRead, NULL) or remainingBytes != bytesRead)
		return false;

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
	space = 1 + 17 + int(ip.size()) + 9 + int(name.size()) + 1;
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

//Mira si existe ya la ip.
bool DatModifier::Exist(const HANDLE& hFile, string& ip)
{
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);
	char* buffer = new char[fileSize.QuadPart];

	DWORD bytesRead;
	ReadFile(hFile, buffer, DWORD(fileSize.QuadPart), &bytesRead, NULL);
	string fileContent(buffer, bytesRead);
	delete[] buffer;
	size_t found = fileContent.find(ip);
	if (found != std::string::npos) 
		return true;
	SetFilePointer(hFile, 18, NULL, FILE_BEGIN);
	return false;
}

void DatModifier::CreateFileServers(HANDLE& hFile, std::wstring path)
{
	hFile = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	char head[] = { 0x0A, 0x00, 0x00, 0x09, 0x00, 0x07, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	WriteFile(hFile, &head, 19, NULL, NULL);
}

void DatModifier::SetServer(string name, string ip, bool textures, std::wstring path)
{
	path += L"\\.minecraft\\servers.dat";
	HANDLE hFile = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		CreateFileServers(hFile, path);

	char n[1];
	SetFilePointer(hFile, 17, NULL, FILE_BEGIN);

	if (not ReadFile(hFile, &n, 1, NULL, NULL) or Exist(hFile, ip))
		return;
	
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
	CloseHandle(hFile);
}

//server.dat.txt
/*
El archivo servers.dat empiza por :

��	�servers
���
0A 00 00 09 00 07 73 65 72 76 65 72 73 0A 00 00 00 0B

Empieza con 01 00 06 que indica el tag hidden que se pone en 00. El tag hidden es : 68 69 64 64 65 6E y depues 00.
Se continua con 08 00 02 que indica que hay otro tag y que es el de la ip.El cual es : 69 70 00, "ip" en hex con centinela.
Luego el valor que empieza con XX que indica cuantos char mide la ip.
Acaba con 08 00 y luego va el name que se indica con 04 mas la codificacion 6E 61 6D 65 00.
El valor del nombre es el primero cuanto mide y despues la codificacion en este caso es : 03  54 46 46 que es TFF.
Si se quiere poner el de acceptTextures 01 se tiene que poner 01 al final del name, sino se pone 00 si se quiere acabar ya el servidor.
Para poner el tag de acceptTextures, hay que poner 00 0E   61 63 63 65 70 74 54 65 78 74 75 72 65 73 que es la codificacion de acceptTextures.
Para indicar si acceptar o denegar poner 01, o 00 respectivamente. Y despues un 00.
Si hay mas servidores hay que poner 01 00, sino 00.


01 00 indica que se a acabado la info del servidor y hay mas servidores, si hay 00 00 se deja de leer al final.
08 al final del valor de los tag indica que hay otro que todavia pertenece a ese slot, si quieres indicar un salto,
menos al final del nombre si hay depues lo de texturas, que es 01.
Todos los valores de los tags y los tags acaban con 00 para indicar que se ha acabado la palabra.
*/
