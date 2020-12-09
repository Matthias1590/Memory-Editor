#include <iostream>
#include <windows.h>
#include <memoryapi.h>
#include <codecvt>
#include <string>

using namespace std;

int main()
{
    // Getting target window input
    string window;
    cout << "Enter window name: ";
    getline(cin, window);

    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    wstring wide = converter.from_bytes(window);
    LPCWSTR result = wide.c_str();

    HWND hwnd = FindWindow(0, result);
    if (!hwnd)
    {
        cout << "Window not found" << endl;
        system("pause");
        return 0;
    }

    // Getting process id
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!hproc)
    {
        cout << "Failed to open process" << endl;
        system("pause");
        return 0;
    }

    // Getting address input
    DWORD address;
    cout << "Enter address to read/write: ";
    cin >> hex >> address;

    int read_write;
    cout << "\n[0] Read value\n[1] Write value\n\nPick an option: ";
    cin >> read_write;

    int loop;
    cout << "\n[0] Read/write value once\n[1] Read/write value continuously\n\nPick an option: ";
    cin >> loop;
    cout << endl;

    int value;
    if (read_write == 1)
    {
        cout << "Enter a value: ";
        cin >> value;
        cout << endl;
    }
    if (loop == 0)
    {
        if (read_write == 0)
        {
            ReadProcessMemory(hproc, (void*)address, &value, sizeof(value), 0);
            cout << "Value: " << value << endl;
        }
        else if (read_write == 1)
        {
            if (WriteProcessMemory(hproc, (void*)address, &value, sizeof(value), 0))
            {
                cout << "Succesfully changed value." << endl;
            }
            else
            {
                cout << "Failed to write" << endl;
            }
        }
    }
    else if (loop == 1)
    {
        if (read_write == 0)
        {
            while (true)
            {
                ReadProcessMemory(hproc, (void*)address, &value, sizeof(value), 0);
                cout << "Value: " << value << endl;
            }
        }
        else if (read_write == 1)
        {
            while (true)
            {
                if (WriteProcessMemory(hproc, (void*)address, &value, sizeof(value), 0))
                {
                    cout << "Succesfully changed value." << endl;
                }
                else
                {
                    cout << "Failed to write" << endl;
                }
            }
        }
    }

    system("pause");
    return 0;
}
