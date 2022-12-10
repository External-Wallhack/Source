#define AntiRE_h__
#define DRIVERS_ARRAY 2048

#include <wininet.h>
#include <Psapi.h>

inline bool HideThread(HANDLE hThread)
{
	typedef NTSTATUS(NTAPI* pNtSetInformationThread) (HANDLE, UINT, PVOID, ULONG);

	NTSTATUS Status;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)GetProcAddress(GetModuleHandle((TEXT("ntdll.dll"))), ("NtSetInformationThread"));

	if (NtSIT == NULL)
		return false;

	if (hThread == NULL)
	{
		Status = NtSIT(GetCurrentThread(), 0x11, 0, 0);
	}
	else
	{
		Status = NtSIT(hThread, 0x11, 0, 0);
	}

	if (Status != 0x00000000)
		return false;
	else
		return true;
}

bool Debug()
{
	LPVOID drivers[DRIVERS_ARRAY];
	DWORD cbNeeded;
	int cDrivers, i;

	if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
	{
		TCHAR szDriver[DRIVERS_ARRAY];

		cDrivers = cbNeeded / sizeof(drivers[0]);

		for (i = 0; i < cDrivers; i++)
		{
			if (GetDeviceDriverBaseName(drivers[i], szDriver, sizeof(szDriver) / sizeof(szDriver[0])))
			{
				std::string strDriver = szDriver;

				if (strDriver.find("HttpDebug") != std::string::npos)
					return true;
			}
		}
	}

	return false;
}

void AntiDumping()
{
	__asm
	{
		MOV EAX, FS: [0x30]
		MOV EAX, [EAX + 0x0C]
		MOV EAX, [EAX + 0x0C]
		MOV DWORD PTR[EAX + 0x20], 0x100000
	}
}

inline void PushPopSS()
{
	__asm
	{
		push ss
		pop ss
		mov eax, 9
		xor edx, edx
	}
}