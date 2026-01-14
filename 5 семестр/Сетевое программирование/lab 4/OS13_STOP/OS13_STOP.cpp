#include <iostream>
#include <Windows.h>
#include "HT.h" 

using namespace std;

// OS13_STOP.exe <FileName>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: OS13_STOP.exe <FileName>" << endl;
        return 1;
    }

    const char* fileName = argv[1];
    HT::HTHANDLE* ht = NULL;

    try {
        cout << "OS13_STOP: Attempting to open HT-Storage to stop..." << endl;

        // Явно передаем false для режима Клиента
        ht = HT::Open(fileName, false);

        if (ht) {
            cout << "HT-Storage opened successfully (Client Mode)." << endl;


            // 1. Закрываем и завершаем работу (Close вызовет SetEvent)
            cout << "Closing HT-Storage (Close) and signaling STOP event... ";
            if (HT::Close(ht)) {
                cout << "SUCCESS. Access stopped." << endl;
            }
            else {
                // Если ошибка, это обычно значит, что не удалось освободить мьютекс
                cout << "FAILED: " << HT::GetLastError(ht) << endl;
                return 3;
            }
        }
        else {
            cout << "Error opening HT-Storage: " << HT::GetLastError(NULL) << endl;
            // Если здесь ошибка (2), это значит, что не удалось найти именованные объекты
            // (Сервер OS13_START.exe не запущен или имена не совпадают).
            return 2;
        }

    }
    catch (int e) {
        cerr << "Critical Error (HRESULT): " << hex << e << dec << endl;
        if (ht) HT::Close(ht);
        return 4;
    }

    return 0;
}