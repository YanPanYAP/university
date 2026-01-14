#include <iostream>
#include <Windows.h>
#include "HT.h"

using namespace std;
using namespace HT;

int main() {
    HTHANDLE* ht1 = nullptr;
    HTHANDLE* ht2 = nullptr;

    try {
        // Создание первого хранилища
        ht1 = Create(1000, 3, 16, 256, "C:\\Users\\user\\Desktop\\VS\\сп\\SP\\space\\HTspace1.ht");
        if (ht1) cout << "-- ht1 Create: success\n";
        else throw "-- ht1 Create: error";

        // Создание второго хранилища
        ht2 = Create(1000, 3, 16, 256, "C:\\Users\\user\\Desktop\\VS\\сп\\SP\\space\\HTspace2.ht");
        if (ht2) cout << "-- ht2 Create: success\n";
        else throw "-- ht2 Create: error";

        // Вставка в оба
        if (Insert(ht1, new Element("keyA", 5, "payload1", 8))) cout << "-- ht1 Insert: success\n";
        else throw "-- ht1 Insert: error";

        if (Insert(ht2, new Element("keyA", 5, "payload2", 8))) cout << "-- ht2 Insert: success\n";
        else throw "-- ht2 Insert: error";

        // Чтение из обоих
        Element* e1 = Get(ht1, new Element("keyA", 5));
        if (e1) {
            cout << "-- ht1 Get: success\n";
            print(e1);
        }
        else throw "-- ht1 Get: error";

        Element* e2 = Get(ht2, new Element("keyA", 5));
        if (e2) {
            cout << "-- ht2 Get: success\n";
            print(e2);
        }
        else throw "-- ht2 Get: error";

        // Обновление
        if (Update(ht1, e1, "new1", 4)) cout << "-- ht1 Update: success\n";
        else throw "-- ht1 Update: error";

        if (Update(ht2, e2, "new2", 4)) cout << "-- ht2 Update: success\n";
        else throw "-- ht2 Update: error";

        // Повторное чтение
        Element* e1_updated = Get(ht1, new Element("keyA", 5));
        if (e1_updated) {
            cout << "-- ht1 Get after update: success\n";
            print(e1_updated);
        }
        else throw "-- ht1 Get after update: error";

        Element* e2_updated = Get(ht2, new Element("keyA", 5));
        if (e2_updated) {
            cout << "-- ht2 Get after update: success\n";
            print(e2_updated);
        }
        else throw "-- ht2 Get after update: error";

        // Snapshot
        if (Snap(ht1)) cout << "-- ht1 Snap: success\n";
        else throw "-- ht1 Snap: error";

        if (Snap(ht2)) cout << "-- ht2 Snap: success\n";
        else throw "-- ht2 Snap: error";

        // Удаление
        if (Delete(ht1, e1_updated)) cout << "-- ht1 Delete: success\n";
        else throw "-- ht1 Delete: error";

        if (Delete(ht2, e2_updated)) cout << "-- ht2 Delete: success\n";
        else throw "-- ht2 Delete: error";

        // Закрытие
        if (Close(ht1)) cout << "-- ht1 Close: success\n";
        else throw "-- ht1 Close: error";

        if (Close(ht2)) cout << "-- ht2 Close: success\n";
        else throw "-- ht2 Close: error";
    }
    catch (const char* msg) {
        cout << msg << "\n";
        if (ht1) cout << "ht1 error: " << GetLastError(ht1) << "\n";
        if (ht2) cout << "ht2 error: " << GetLastError(ht2) << "\n";
    }

    return 0;
}
