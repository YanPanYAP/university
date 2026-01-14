#include <iostream>
#include <string>
#include <Windows.h>
#include <chrono>
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

int main() {
    setlocale(LC_ALL, "ru");
    LPCWSTR MAILSLOT_NAME_LOCAL = L"\\\\.\\mailslot\\Box";

    HANDLE cH = CreateFileW(MAILSLOT_NAME_LOCAL, GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (cH == INVALID_HANDLE_VALUE) {
        cout << "[Perf] Ошибка открытия: " << GetErrorMsgText(GetLastError()) << endl;
        return 1;
    }

    const int N = 1000;
    string payload = "Perf test message"; // ≤ maxMessageSize
    DWORD written = 0;

    auto t0 = chrono::high_resolution_clock::now();
    for (int i = 1; i <= N; ++i) {
        if (!WriteFile(cH, payload.c_str(), (DWORD)payload.size(), &written, NULL)) {
            cout << "[Perf] Ошибка WriteFile: " << GetErrorMsgText(GetLastError()) << endl;
            CloseHandle(cH);
            return 1;
        }
    }
    auto t1 = chrono::high_resolution_clock::now();
    double seconds = chrono::duration<double>(t1 - t0).count();

    cout << "[Perf] Отправлено " << N << " сообщений за " << seconds << " сек" << endl;
    cout << "[Perf] Скорость ~ " << (N / seconds) << " сообщений/сек" << endl;

    CloseHandle(cH);
    return 0;
}
