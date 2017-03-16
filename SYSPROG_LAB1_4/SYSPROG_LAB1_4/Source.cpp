#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <tchar.h> 
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	const char* MyMutexNamedName = "lab1_2_mutex";
	wchar_t wtext1[20];
	mbstowcs(wtext1, MyMutexNamedName, strlen(MyMutexNamedName) + 1);
	LPWSTR ptr1 = wtext1;
	HANDLE hMyMutexNamed = CreateMutex(NULL, FALSE, ptr1);

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		puts("Second: ");
		cout << "Error, process already exist!" << endl;
		return 0;
	}
	else
	{
		puts("FIRST RUN:");
		HANDLE hMyMutexInherited = CreateMutex(&sa, FALSE, NULL);
		
		if (hMyMutexInherited != NULL)
		{
			STARTUPINFO si = { sizeof(si) };
			PROCESS_INFORMATION pi;

			char cmdLn[255];
			sprintf_s(cmdLn, "SYSPROG_LAB1_4 %d\0", hMyMutexInherited);

			wchar_t wtext[255];
			mbstowcs(wtext, cmdLn, strlen(cmdLn) + 1);
			LPWSTR ptr = wtext;

			if (CreateProcess(TEXT("D:\\Visual Studio\\System Programming\\SYSPROG_LAB1_4\\Debug\\SYSPROG_LAB1_4.exe"), ptr, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
			{
				cout << "Process  created!" << endl;
			}
			else
				cout << "Process not created!" << endl;
			system("pause");
			CloseHandle(hMyMutexInherited);
		}
		else
		{
			cout << "Mutex not created" << endl;
		}
		CloseHandle(hMyMutexNamed);
	}

	return 0;
}