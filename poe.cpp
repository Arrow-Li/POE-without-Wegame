#include "windows.h"
#include "tlhelp32.h"
#include "iostream"

bool findProcess(LPCTSTR name);
bool killProcess(LPCTSTR name);

int main(int argc, char const *argv[]) {
	auto a = (int)ShellExecute(NULL, NULL, PCHAR("launcher.exe"), NULL, NULL, SW_NORMAL);
	if (a < 32)
		return 0;
	Sleep(2000);
	//std::cout << "Start Scan!";
	while (true) {
		if (findProcess("PathOfExile_x64.exe")) {
			//std::cout << "Find!";
			Sleep(2000);
			while (true) {
				if (findProcess("GameLoader.exe"))
					break;
				Sleep(99);
			}
			Sleep(9999);
			if (killProcess("tgp_daemon.exe")) {
				killProcess("GameLoader.exe");
				return 0;
			}
			else
				return 0;
		}
		Sleep(99);
	}
	while (true);
	return 0;
}


bool findProcess(LPCTSTR name) {
	bool flag = false;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe; pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return false;
	while (true) {
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name) == 0) {
			flag = true;
			break;
		}
		Sleep(1);
	}
	CloseHandle(hSnapshot);
	return flag;
}


bool killProcess(LPCTSTR name) {
	DWORD pid = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe; pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;
	while (true) {
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp(pe.szExeFile, name) == 0) {
			pid = pe.th32ProcessID;
			break;
		}
		Sleep(1);
	}
	CloseHandle(hSnapshot);
	if (pid != 0) {
		HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		if (TerminateProcess(handle, 0)) {
			CloseHandle(handle);
			//std::cout << "Success Terminate!";
			return true;
		}
		else {
			CloseHandle(handle);
			return false;
		}
	}
	else
		return true;
}