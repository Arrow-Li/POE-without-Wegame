#include "string"
#include "time.h"
#include "fstream"
#include "windows.h"
#include "tlhelp32.h"

bool killProcess(LPCTSTR name);

int main(int argc, char const *argv[]) {
	auto exec_code = (int)ShellExecute(NULL, NULL, PCHAR("launcher.exe"), NULL, NULL, SW_NORMAL);
	if (exec_code < 32) //调用失败
		return 0;
	Sleep(2000);

	std::fstream log_file;

	if (argc > 1) //入参有文件路径
		log_file.open(argv[1], std::ios::in);
	if (!log_file.is_open()) //文件是否打开
		return 0;

	std::string line;
	log_file.seekg(0, std::ios::end);

	// Start Scan
	while (true) {
		if (log_file.peek() == EOF) //实时更新文件指针
		{
			if (clock() > 300000) {
				log_file.close();
				return 0;
			}
			log_file.clear();
			Sleep(2000);
		}

		std::getline(log_file, line);

		if (line.find("Connected to") != std::string::npos) //已连接服务器
		{
			log_file.close();
			break;
		}
	}
	// End Scan

	killProcess("tgp_daemon.exe");
	killProcess("GameLoader.exe");

	return 0;
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
