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

    // 1. Создаём локальный Mailslot (почтовый ящик сервера)
    // Формат имени для Mailslot: \\.\mailslot\<name>
    // В задании: имя "Box", максимальный размер 300, таймаут для чтения – бесконечность
    LPCWSTR MAILSLOT_NAME = L"\\\\.\\mailslot\\Box";
    DWORD maxMessageSize = 300;
    DWORD readTimeout = 3 * 60 * 1000; // 3 минуты в миллисекундах
    //DWORD readTimeout = MAILSLOT_WAIT_FOREVER;

    HANDLE sH = INVALID_HANDLE_VALUE;

    try {
        sH = CreateMailslotW(
            MAILSLOT_NAME,         // имя
            maxMessageSize,        // максимальный размер сообщения (сервер объявляет лимит)
            readTimeout,           // таймаут ожидания ReadFile (бесконечно)
            NULL                   // безопасность по умолчанию
        );

        if (sH == INVALID_HANDLE_VALUE)
            throw SetMSError("CreateMailslot: ", GetLastError());

        cout << "[ServerMS] Mailslot создан: " << "\\\\.\\mailslot\\Box" << endl;
        cout << "[ServerMS] Ожидание сообщений..." << endl;

        // 2. Чтение сообщений (универсальная ReadFile)
        // ReadFile читает по одному сообщению; размер ≤ maxMessageSize
        DWORD readBytes = 0;
        char rbuf[512] = {}; // буфер больше, чем maxMessageSize, но читаем не более 300

        // Демонстрационный цикл: читаем до нажатия Ctrl+C или до получения "STOP"
        while (true) {
            BOOL ok = ReadFile(sH, rbuf, maxMessageSize, &readBytes, NULL);
            if (!ok) {
                DWORD err = GetLastError();
                if (err == ERROR_SEM_TIMEOUT) {
                    // таймаут — это штатно, если он настроен конечным; здесь бесконечность, так что не увидим
                    cout << "[ServerMS] Нет сообщений от клиента в течение 3 минут." << endl;
                    continue;
                }
                throw SetMSError("ReadFile: ", err);
            }

            if (readBytes == 0) {
                // Пустое сообщение (редко, но возможно)
                continue;
            }

            // Гарантируем терминацию строки
            if (readBytes < sizeof(rbuf)) rbuf[readBytes] = '\0'; else rbuf[sizeof(rbuf) - 1] = '\0';

            cout << "[ServerMS] Получено: " << rbuf << " (" << readBytes << " байт)" << endl;

            // Простая команда остановки
            if (strcmp(rbuf, "STOP") == 0) {
                cout << "[ServerMS] Команда STOP: завершение." << endl;
                break;
            }
        }

        // 3. Закрытие дескриптора
        if (!CloseHandle(sH))
            throw SetMSError("CloseHandle: ", GetLastError());

        cout << "[ServerMS] Завершено без ошибок." << endl;
    }
    catch (const string& e) {
        cout << "\n[FATAL] " << e << endl;
        if (sH != INVALID_HANDLE_VALUE) CloseHandle(sH);
    }

    return 0;
}
