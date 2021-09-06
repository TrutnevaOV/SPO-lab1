#define WINVER 0x0601
#include <windows.h>
#include <iostream>
#include <tchar.h>
using namespace std;

#define BUFFER_SIZE 32767

// Функция для пункта 2
void Counter()
{
    unsigned long freq = 0;
    LARGE_INTEGER t0;
    LARGE_INTEGER t;
    __int64 CounterStart = 0;

    QueryPerformanceCounter(&t0);
    CounterStart = t0.QuadPart;
    if (QueryPerformanceFrequency(&t)== 0)
        cout << "Function QueryPerformanceFrequency() failed!\n";
    freq = long(t.QuadPart);
    cout << "\nCPU frequency: " << freq << " Hz\n";

    if (QueryPerformanceCounter(&t)== 0) {
        cout << "Function QueryPerformanceCounter() failed!\n";
    }
    else {
        double result = (1000000 * (t.QuadPart - CounterStart)) / freq;
        cout << "Number of cpu cycles: " << result  << " us \n";
    }
}

int main()
{
    // 1.1 Получение версии операционной системы
        OSVERSIONINFO os_info = { 0 };
        os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx(&os_info);
        printf("Version OS: %i.%i\n", os_info.dwMajorVersion, os_info.dwMinorVersion);

    // 1.2 Получение системной директории
        TCHAR buf[BUFFER_SIZE];
        GetSystemDirectory(buf, BUFFER_SIZE);
        printf("\nSystem Directory: %s \n", buf);

    // 1.3 Получение названия компьютера и псевдонима текущего пользователя
        DWORD siz = sizeof(buf);
        GetComputerName(buf,&siz);
        printf("\nName Computer: %s \n", buf);
        GetUserName(buf,&siz);
        printf("Name User: %s \n", buf);

    // 1.4 Вывод характеристик для каждого тома
        TCHAR buffer[MAX_PATH];
        TCHAR name[MAX_PATH];
        DWORD  BufferLength = MAX_PATH;
        ULARGE_INTEGER freespac;
        ULARGE_INTEGER vol;

        HANDLE search = FindFirstVolume(buffer, sizeof(buffer));
        do  {
            GetVolumePathNamesForVolumeName(name, buffer, BufferLength, &BufferLength);
            printf("\n\n%s", name);
            printf("\n%s", buffer);
            if (GetDiskFreeSpaceEx(name, &freespac, &vol, NULL)!= 0) {
                cout << "\nVolume: " << vol.QuadPart << "\nFree space: " << freespac.QuadPart;
            }
            else{
                 printf("\nNo information.");
            }
        } while (FindNextVolume(search, name, sizeof(buffer)));
        FindVolumeClose(search);

    // 1.5 Получение списка программ из реестра Windows
        HKEY hkres;
        DWORD i = 0;
        TCHAR val[BUFFER_SIZE];
        DWORD sval;
        TCHAR data[BUFFER_SIZE];
        DWORD sizedata = BUFFER_SIZE;

        printf("\n\nList of commands:\n");
        RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkres);
        do {
            sval = sizeof(val);
            if (RegEnumValue(hkres, i, val, &sval, NULL, REG_NONE, NULL, NULL) == ERROR_SUCCESS) {
                RegQueryValueEx(hkres, val, NULL, REG_NONE, (LPBYTE)data, &sizedata);
                printf("%i) %s:  %s\n", i + 1, val, data);
                i++;
            }
            else break;
        } while (1);
        RegCloseKey(hkres);

    // 2 Измерение производительности ЦП
        Counter();

    return 0;
}
