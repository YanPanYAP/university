#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>

#pragma warning(disable:4996)
using namespace std;

string GetErrorMsgText(DWORD code) {
    LPSTR buf = nullptr;
    DWORD len = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&buf, 0, NULL
    );
    string msg = (len && buf) ? string(buf) : "Unknown error";
    if (buf) LocalFree(buf);
    return msg;
}
string SetMSError(const string& where, DWORD code) {
    return where + GetErrorMsgText(code);
}

int main() {
    setlocale(LC_ALL, "ru");

    // Локальный формат имени сервера (тот же name "Box")
    LPCWSTR MAILSLOT_NAME_LOCAL = L"\\\\.\\mailslot\\Box";
    LPCWSTR MAILSLOT_NAME_LAN = L"\\\\WIN-FSAHKJTGC3M\\mailslot\\Box";

    vector<wstring> servers = {
    L"\\\\WIN-FSAHKJTGC3M\\mailslot\\Box",
    L"\\\\WIN-FSAHKJTGC3M\\mailslot\\Box",
    L"\\\\WIN-FSAHKJTGC3M\\mailslot\\Box"
    };

    const char* msg = "Broadcast from client to 3 servers";
    for (const auto& target : servers) {
        HANDLE h = CreateFileW(target.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (h == INVALID_HANDLE_VALUE) {
            cout << "[ClientMS] Ошибка открытия " << string(target.begin(), target.end())
                << ": " << GetErrorMsgText(GetLastError()) << endl;
            continue;
        }
        DWORD written = 0;
        if (!WriteFile(h, msg, (DWORD)strlen(msg), &written, NULL)) {
            cout << "[ClientMS] Ошибка WriteFile для "
                << string(target.begin(), target.end()) << ": "
                << GetErrorMsgText(GetLastError()) << endl;
        }
        else {
            cout << "[ClientMS] Отправлено -> " << string(target.begin(), target.end())
                << " (" << written << " байт)" << endl;
        }
        if (!CloseHandle(h)) {
            cout << "[ClientMS] Ошибка CloseHandle: " << GetErrorMsgText(GetLastError()) << endl;
            return 1;
        }
    }


    cout << "[ClientMS] Завершено без ошибок." << endl;
    return 0;
}
