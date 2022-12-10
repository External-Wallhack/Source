#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Wincrypt.h>
#include <vector>
#include <Shellapi.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <urlmon.h>
#include <Winsock.h>
#include <WinInet.h>
#include <WbemIdl.h>
#include <iphlpapi.h>
#include <string>
#include <sstream>
#include <string>

using namespace std;

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "iphlpapi")
#pragma comment(lib, "urlmon.lib")

//[enc_string_enable /]
#define HOST			"takeyourlife.000webhostapp.com"
#define PATH			"/WebPanel/"

#define HOST_GATE		"gate.php?serial="
#define HOST_KEY_GATE	"license-success-ok-"

#define HOST_CHECK		"check.php?serial="
#define HOST_KEY_CHECK	"D2DF62F3E61D4696-"

#define CHEAT_VERSION	"4"

class CLicense
{
private:
	//[swap_lines]
	string	StringToHex(const string input);
	string	GetHashText(const void* data, const size_t data_size);

	string	GetHwUID();
	string	GetMacID();
	DWORD	GetVolumeID();
	string	GetCompUserName(bool User);
	string	GetSerialKey();
	string	GetHashSerialKey();
	//[/swap_lines]
public:
	//[swap_lines]
	string	GetUrlData(string url);
	string	GetOldSerial();
	string	GetSerial();
	string	GetSerial64();
	string	GetUserDayCount();
	bool	CheckLicenseURL(string URL, string GATE, string KEY);
	bool	CheckLicense();
	bool	CheckVersion();
	void	UpdateExe();
	//[/swap_lines]
};

string base64_encode(char const* bytes_to_encode, unsigned int in_len);