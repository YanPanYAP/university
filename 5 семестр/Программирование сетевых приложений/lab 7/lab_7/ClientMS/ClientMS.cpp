#include <iostream>
#include <string>
#include <Windows.h>

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

    // 1. Открываем "ящик" сервера для записи (CreateFile)
    // ВАЖНО: у Mailslot клиента всегда GENERIC_WRITE; SHARE_READ/WRITE допустим
    HANDLE cH = CreateFileW(
        MAILSLOT_NAME_LAN,
        GENERIC_WRITE,                 // только запись
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,                          // безопасность по умолчанию
        OPEN_EXISTING,                 // Mailslot должен существовать (сервер был запущен)
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (cH == INVALID_HANDLE_VALUE) {
        cout << "[ClientMS] Ошибка открытия Mailslot: " << GetErrorMsgText(GetLastError()) << endl;
        cout << "[ClientMS] Убедитесь, что сервер запущен." << endl;
        return 1;
    }

    // 2. Отправка сообщения (WriteFile)
    const char* msg = "Hello from Mailslot-client";
    DWORD written = 0;

    if (!WriteFile(cH, msg, (DWORD)strlen(msg), &written, NULL)) {
        cout << "[ClientMS] Ошибка WriteFile: " << GetErrorMsgText(GetLastError()) << endl;
        CloseHandle(cH);
        return 1;
    }

    cout << "[ClientMS] Отправлено: " << msg << " (" << written << " байт)" << endl;

    // 3. Закрытие
    if (!CloseHandle(cH)) {
        cout << "[ClientMS] Ошибка CloseHandle: " << GetErrorMsgText(GetLastError()) << endl;
        return 1;
    }

    cout << "[ClientMS] Завершено без ошибок." << endl;
    return 0;
}
