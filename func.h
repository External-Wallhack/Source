void InitOverlay();
void JuniorHotkeys();
void initD3D(HWND hWnd);
void CreateConsole();
void AutoDeleteOldFile();

void playMusicLogin() {
	Beep(330, 100); Sleep(100);
	Beep(330, 100); Sleep(300);
	Beep(330, 100); Sleep(300);
	Beep(262, 100); Sleep(100);
	Beep(330, 100); Sleep(300);
	Beep(392, 100);
}

void playMusicFailed() {
	Beep(500, 500);
	Beep(600, 500);
	Beep(700, 500);
}

string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	return string(buffer).substr(0, pos);
}