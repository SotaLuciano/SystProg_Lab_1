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

	//CreateMutex
	const char* MyMutexNamedName = "lab1_2_mutex";
	wchar_t wtext1[20];
	mbstowcs(wtext1, MyMutexNamedName, strlen(MyMutexNamedName) + 1);
	LPWSTR ptr1 = wtext1;
	HANDLE hMyMutexNamed = CreateMutex(NULL, FALSE, ptr1);

	//Create Semaphores
	const char* Semaphore_1_Name = "Semaphore_1";
	mbstowcs(wtext1, Semaphore_1_Name, strlen(Semaphore_1_Name) + 1);
	LPWSTR ptr2_1 = wtext1;
	HANDLE hSemaphore_1 = CreateSemaphore(NULL, 1, 1, ptr2_1);
	const char* Semaphore_2_Name = "Semaphore_2";
	mbstowcs(wtext1, Semaphore_2_Name, strlen(Semaphore_2_Name) + 1);
	LPWSTR ptr2_2 = wtext1;
	HANDLE hSemaphore_2 = CreateSemaphore(NULL, 1, 1, ptr2_2);
	HANDLE hSemaphores[2];
	hSemaphores[0] = hSemaphore_1;
	hSemaphores[1] = hSemaphore_2;

	//Create Timer
	const char* Timer_Name = "timer_1";
	mbstowcs(wtext1, Timer_Name, strlen(Timer_Name) + 1);
	LPWSTR ptr3 = wtext1;
	HANDLE hTimer = CreateWaitableTimer(NULL, true, ptr3);
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -50000000LL;

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		puts("SECOND RUN:");
		printf("2: Mutex `%s` already exists!\n", MyMutexNamedName);
		printf("2: Semaphores `%s` and `%s` already exists!\n", Semaphore_1_Name , Semaphore_2_Name);
		printf("2: Timer `%s` already exists!\n", Timer_Name);
		
		puts("\n\tMUTEX:");

		HANDLE hMyMutexInherited = (HANDLE)atoi(argv[1]);
		printf("2: %s (%d) : 0x%X\n", argv[0], argc, hMyMutexInherited);
		switch (WaitForSingleObject(hMyMutexNamed, INFINITE))
		{
		case WAIT_OBJECT_0:
			puts("2: Got Mutex!");
			puts("2: Mutex to be released...");
			ReleaseMutex(hMyMutexNamed);
			break;
		case WAIT_FAILED:
			puts("2: Failed to wait mutex!");
			break;
		case WAIT_ABANDONED:
			puts("2: Wait abandoned!");
			break;
		default:
			puts("2: Some other wait result");
			break;
		}

		puts("\n\tSEMAPHORES:");

		HANDLE hSemaphoreInherited_1 = (HANDLE)atoi(argv[2]);
		printf("2: %s (%d) : 0x%X\n", argv[0], argc, hSemaphoreInherited_1);
		HANDLE hSemaphoreInherited_2 = (HANDLE)atoi(argv[3]);
		printf("2: %s (%d) : 0x%X\n", argv[0], argc, hSemaphoreInherited_2);
		HANDLE hSemaphoresInherited[2];
		hSemaphoresInherited[0] = hSemaphoreInherited_1;
		hSemaphoresInherited[1] = hSemaphoreInherited_2;
		
		if (WaitForMultipleObjects(2, hSemaphoresInherited, true, INFINITE) == WAIT_OBJECT_0)
		{

			cout << "2: Got Semaphores!" << endl;
			ReleaseSemaphore(hSemaphoreInherited_1, 1, NULL);
			ReleaseSemaphore(hSemaphoreInherited_2, 1, NULL);
		}
		else	cout << "2: Failed to wait semaphores!" << GetLastError() << endl;


		puts("\n\tTIMER:");

		HANDLE hTimerInherited = (HANDLE)atoi(argv[4]);
		printf("2: %s (%d) : 0x%X\n", argv[0], argc, hTimerInherited);

		puts("Wait 5 seconds...");
		SetWaitableTimer(hTimerInherited, &liDueTime, 0, NULL, NULL, 0);
		

		if (WaitForSingleObject(hTimerInherited, INFINITE) != WAIT_OBJECT_0)
			cout << "SetWaitableTimer failed: " << GetLastError() << endl;
		else
			cout << "\nTimer was signaled.\n";

		CloseHandle(hMyMutexInherited);
		CloseHandle(hMyMutexNamed);
		
		CloseHandle(hSemaphoresInherited[0]);
		CloseHandle(hSemaphoresInherited[1]);
		CloseHandle(hSemaphores[0]);
		CloseHandle(hSemaphores[1]);

		CloseHandle(hTimerInherited);
		CloseHandle(hTimer);

		cout << "\nSECOND IS OVER\n";
	}
	else
	{
		puts("FIRST RUN:");

		HANDLE hMyMutexInherited = CreateMutex(&sa, FALSE, NULL);

		HANDLE hSemaphoreInherited_1 = CreateSemaphore(&sa, 1, 1, NULL);
		HANDLE hSemaphoreInherited_2 = CreateSemaphore(&sa, 1, 1, NULL);
		HANDLE hSemaphoresInherited[2];
		hSemaphoresInherited[0] = hSemaphoreInherited_1;
		hSemaphoresInherited[1] = hSemaphoreInherited_2;

		HANDLE hTimerInherited = CreateWaitableTimer(&sa, true, NULL);

		if (hMyMutexInherited != NULL || hSemaphoreInherited_1 != NULL || hSemaphoreInherited_2 != NULL || hTimerInherited != NULL)
		{
			printf("1: Mutex 0x%X created!\n", hMyMutexInherited);
			printf("1: Semaphores 0x%X and 0x%X created!\n", hSemaphoresInherited[0], hSemaphoresInherited[1]);
			printf("1: Timer 0x%X created!\n", hTimerInherited);
			STARTUPINFO si = { sizeof(si) };
			PROCESS_INFORMATION pi;
			char cmdLn[255];
			sprintf_s(cmdLn, "SYSPROG_LAB1_2 %d %d %d %d\0", hMyMutexInherited, hSemaphoreInherited_1, hSemaphoreInherited_2, hTimerInherited);
		
			wchar_t wtext[255];
			mbstowcs(wtext, cmdLn, strlen(cmdLn) + 1);
			LPWSTR ptr = wtext;
			
			if (CreateProcess(TEXT("D:\\Visual Studio\\System Programming\\SYSPROG_LAB1_2\\Debug\\SYSPROG_LAB1_2.exe"),ptr, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
				puts("1: Process created!");
			else {
				puts("1: Failed to create process!");
			}
			puts("1: Mutex to be released...");
			puts("1: Semaphores to be released...");
			system("pause");

			ReleaseMutex(hMyMutexInherited);

			ReleaseSemaphore(hSemaphoresInherited[0], 1, NULL);
			ReleaseSemaphore(hSemaphoresInherited[1], 1, NULL);

			CloseHandle(hMyMutexInherited);
			CloseHandle(hSemaphoresInherited[0]);
			CloseHandle(hSemaphoresInherited[1]);
			CloseHandle(hTimerInherited);
		}
		else puts("1: Error: mutex or semaphores or timer not created.");

		CloseHandle(hMyMutexNamed);
		CloseHandle(hSemaphores[0]);
		CloseHandle(hSemaphores[1]);
		CloseHandle(hTimer);

		puts("FIRST IS OVER");
	}
	return 0;
}