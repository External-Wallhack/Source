#pragma once

#include "main.h"
#include "func.h"

Matrix bMatrix;
RECT rc;

#define WIN32_LEAN_AND_MEAN
#define BREAK_WITH_ERROR( e ) { ShowWindow(GetConsoleWindow(), SW_SHOW); printf( "[-] %s. Error=%d", e, GetLastError() ); system("pause"); break; }

bool isActive = true;
uint16_t uStyle = 1;
HINSTANCE hInstance;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_CREATE:
		DwmExtendFrameIntoClientArea(hWnd, &margin);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
};


DWORD GetProcId(const char* procname)
{
	PROCESSENTRY32 pe;
	HANDLE hSnap;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Process32First(hSnap, &pe)) {
		do {
			if (strcmp(pe.szExeFile, procname) == 0)
				break;
		} while (Process32Next(hSnap, &pe));
	}
	return pe.th32ProcessID;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow) {
	CreateConsole();
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcId("gta_sa.exe"));

	LPVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, sizeof(cavedata), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(hProcess, pRemoteThread, (LPVOID)RemoteThread, sizeof(cavedata), 0);
	cavedata* pData = (cavedata*)VirtualAllocEx(hProcess, NULL, sizeof(cavedata), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, pData, &CaveData, sizeof(cavedata), NULL);
	HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, pData, 0, 0);

	WaitForSingleObject(hThread, INFINITE);
	
	VirtualFreeEx(hProcess, pRemoteThread, sizeof(cavedata), MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	getchar();
/*	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AutoDeleteOldFile, 0, 0, 0);

	HANDLE curThread = GetCurrentThread();
	HideThread(curThread);
	CloseHandle(curThread);

	AntiDumping();
	PushPopSS();

	//VMProtectBeginUltra("MainFunction");

	if (Debug()) return 1;
	CreateConsole();

#if ENABLE_LICENSING == 0
	bool Exit = true;

	CLicense License;

	string OldSerial = License.GetOldSerial();
	string NewSerial = License.GetSerial();
	string ReLicData = OldSerial + "|" + NewSerial;

	ReLicData = base64_encode(ReLicData.c_str(), ReLicData.size());

	string path = PATH;
	path.append("rebind.php?data=");

	string ReLicenseUrl = path + ReLicData;
	string ServerRequest = License.GetUrlData(ReLicenseUrl);

	if (ServerRequest == "error: 1") Exit = false;
	else if (ServerRequest == "error: 2") Exit = true;
	else if (ServerRequest == "error: 3") Exit = true;
	else if (ServerRequest == "success") printf("Successfully activated the license for a new hwid !\n");

	if (License.CheckLicense() && !Exit) {
		if (License.CheckVersion())
		{
			system("color 2");

			cout << "  ////////////////////////////" << endl;
			cout << " /// It is time to UPDATE ///" << endl;
			cout << "////////////////////////////" << endl << endl;

			License.UpdateExe();
		}

		playMusicLogin();
		system("cls");
		system("color 2");

		cout << "  ///////////////////////////" << endl;
		cout << " /// U SUCCESFUL LOGINED ///" << endl;
		cout << "///////////////////////////" << endl << endl;

		Sleep(500);
		cout << "Left " << License.GetUserDayCount() << " day" << endl;
		cout << "Finding SA:MP process.." << endl;
	}

	if (Exit)
	{
		playMusicFailed();
		system("color 4");

		cout << "********************************" << endl;
		cout << "* Error while checking license *" << endl;
		cout << "********************************" << endl << endl;
		cout << "Your Serial: " << NewSerial << endl;

		Sleep(8000);
		return 0;
	}
#endif

	//VMProtectEnd();

	while (!mem.Attach("gta_sa.exe", PROCESS_VM_READ) || (int)FindWindow(0, "GTA:SA:MP") == 0) { Sleep(12); }
	pSamp = mem.GetModule("samp.dll");

	system("cls");
	system("color d");

	cout << "Process founded.. | Injected successfully injected" << endl;
	cout << "Closing console.. | Good Luck, mate";

	Sleep(1200);

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	HANDLE Threads[] =
	{
		CreateThread(0 , 0 , (LPTHREAD_START_ROUTINE)InitOverlay , 0 , 0 , 0),
		CreateThread(0 , 0 , (LPTHREAD_START_ROUTINE)JuniorHotkeys , 0 , 0 , 0),
	};

	WaitForMultipleObjects(sizeof(Threads) / sizeof(HANDLE), Threads, TRUE, INFINITE);*/
	return 0;
}

void AutoDeleteOldFile() {
	HANDLE curThread = GetCurrentThread();
	HideThread(curThread);
	CloseHandle(curThread);

	//VMProtectBeginUltra("AutoDelete");

	bool isActiveDelete = true;
	while (isActiveDelete) {
		if (ifstream(ExePath() + "/qWertYz.exe"))
			remove((ExePath() + "/qWertYz.exe").c_str());
		else
			isActiveDelete = false;

		Sleep(50);
	}

	//VMProtectEnd();
}

void JuniorHotkeys() {
	HANDLE curThread = GetCurrentThread();
	HideThread(curThread);
	CloseHandle(curThread);

	while (1) {
		if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
			while (GetAsyncKeyState(VK_INSERT) & 0x8000) { Sleep(100); };
			isActive = !isActive;
		}

		if (GetAsyncKeyState(VK_END) & 0x8000) {
			while (GetAsyncKeyState(VK_END) & 0x8000) { Sleep(100); };
			if (uStyle == 1) uStyle++;
			else uStyle--;
		}

		if (GetAsyncKeyState(VK_DELETE) & 0x8000) ExitProcess(0);

		Sleep(50);
	}
}

void InitOverlay() {
	HANDLE curThread = GetCurrentThread();
	HideThread(curThread);
	CloseHandle(curThread);

	GetWindowRect(FindWindow(NULL, "GTA:SA:MP"), &rc);

	OVERLAY_WIDTH = rc.right - rc.left;
	OVERLAY_HEIGHT = rc.bottom - rc.top;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = RGB(0, 0, 0);
	wc.lpszClassName = " ";

	RegisterClassEx(&wc);

	OVERLAY_WINDOW = CreateWindowEx(0, " ", " ", WS_EX_TOPMOST | WS_POPUP, rc.left, rc.top, OVERLAY_WIDTH, OVERLAY_HEIGHT, nullptr, nullptr, hInstance, nullptr);
	SetWindowLong(OVERLAY_WINDOW, GWL_EXSTYLE, GetWindowLong(OVERLAY_WINDOW, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);

	SetLayeredWindowAttributes(OVERLAY_WINDOW, 0, 255, LWA_ALPHA);
	ShowWindow(OVERLAY_WINDOW, 1);

	initD3D(OVERLAY_WINDOW);
	MSG msg;
	while (TRUE)
	{
		SetWindowPos(OVERLAY_WINDOW, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		if (isActive)
			RenderESP(true);
		else
			RenderESP(false);

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}
}

void DrawScene() {
	DWORD pInfo = mem.ReadPointer<DWORD>(pSamp.dwBase + SAMP_INFO);
	DWORD pPools = mem.ReadPointer<DWORD>(pInfo + SAMP_POOLS);
	DWORD pPlayerPool = mem.ReadPointer<DWORD>(pPools + 0x18);

	for (DWORD i = 0; i < 4016; i += 4) {
		int IsPlayerListed = mem.ReadPointer<int>(pPlayerPool + 0xFDE + i);

		if (IsPlayerListed > 0x00) {
			DWORD pRemotePlayer = mem.ReadPointer<DWORD>(pPlayerPool + 0x2E + i);
			DWORD pRemotePlayerData = mem.ReadPointer<DWORD>(pRemotePlayer + 0x0);
			DWORD dwLastStreamedInTick = mem.ReadPointer<DWORD>(pRemotePlayerData + 0x1C9);

			if (dwLastStreamedInTick == 0) {
				float pHealth = mem.ReadPointer<float>(pRemotePlayerData + 0x1BC);

				if (pHealth > 0.0f) {
					DWORD		pSAMPActor	= mem.ReadPointer<DWORD>(pRemotePlayerData + 0x0);
					DWORD		dwPed		= mem.ReadPointer<DWORD>(pSAMPActor + 676);

					int			pVehicle	= mem.ReadPointer<int>(pRemotePlayerData + 9);
					int			pSkinID		= (int)(mem.ReadPointer<byte>(dwPed + 0x22));

					D3DVECTOR	pPosition;

					if (pVehicle == 19) pPosition = D3DVECTOR{ mem.ReadPointer<float>(pRemotePlayerData + 147), mem.ReadPointer<float>(pRemotePlayerData + 151), mem.ReadPointer<float>(pRemotePlayerData + 155) };
					else pPosition = D3DVECTOR{ mem.ReadPointer<float>(pRemotePlayerData + 123), mem.ReadPointer<float>(pRemotePlayerData + 127), mem.ReadPointer<float>(pRemotePlayerData + 131) };

					D3DXMATRIX matrix = mem.ReadPointer<D3DXMATRIX>(0xB6FA2C);

					DWORD dwXLen = mem.ReadPointer<DWORD>(0xC17044);
					DWORD dwYLen = mem.ReadPointer<DWORD>(0xC17048);
					DWORD dwZLen = mem.ReadPointer<DWORD>(0xC1704C);

					D3DVECTOR Screen = bMatrix.CalcScreenCoors(Resolution{ dwXLen, dwYLen, dwZLen }, matrix, pPosition);

					if (Screen.z < 1.0f
						|| Screen.x < 0.0f
						|| Screen.y < 0.0f
						|| Screen.x > dwXLen
						|| Screen.y > dwYLen) continue;

					switch (uStyle)
					{
                        case 1:
                            DrawCircle(Screen.x, Screen.y, 20, 0, full, true, 100, getColorGangBySkinID(pSkinID));
                            break;
                        case 2:
                            DrawBoxBordered(Screen.x - 14, Screen.y - 22, 30, 30, 1, D3DCOLOR_ARGB(0, 0, 0, 0), getColorGangBySkinID(pSkinID));
                            break;
					}

					DrawString(Screen.x - 20, Screen.y - 39, D3DCOLOR_ARGB(255, 255, 255, 255), pFont, "HP: " + to_string((int)pHealth));
				}
			}
		}
	}
}

void CreateConsole()
{
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	freopen("CONIN$", "rt", stdin);
}