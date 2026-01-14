#include <iostream>
#include <cstring>
#include "HT.h"

using namespace std;
using namespace HT;

void assertTest(bool condition, const char* testName) {
    cout << (condition ? "[PASS] " : "[FAIL] ") << testName << endl;
}

int main() {
    const char* filename = "C:\\Users\\user\\Desktop\\VS\\сп\\SP\\lab_10\\OS10_04\\HT_test\\HT_test.ht";
    HTHANDLE* ht = Create(100, 3, 16, 256, filename);
    assertTest(ht != nullptr, "Create");

    Element* e1 = new Element("key1", 4, "value1", 6);
    assertTest(Insert(ht, e1), "Insert key1");

    Element* get1 = Get(ht, new Element("key1", 4));
    assertTest(get1 != nullptr, "Get key1");
    assertTest(memcmp(get1->payload, "value1", 6) == 0, "Payload matches");
    delete get1;

    assertTest(Update(ht, e1, "newval", 6), "Update key1");

    Element* get2 = Get(ht, new Element("key1", 4));
    assertTest(get2 != nullptr, "Get after update");
    assertTest(memcmp(get2->payload, "newval", 6) == 0, "Updated payload matches");
    delete get2;

    assertTest(Delete(ht, e1), "Delete key1");

    Element* get3 = Get(ht, new Element("key1", 4));
    assertTest(get3 == nullptr, "Get after delete");

    assertTest(Snap(ht), "Snap");
    assertTest(Close(ht), "Close");

    ht = Open(filename);
    assertTest(ht != nullptr, "Open");

    assertTest(!Insert(ht, new Element("key1", 4, "again", 5)), "Insert duplicate key1");
    assertTest(!Update(ht, new Element("keyX", 4), "fail", 4), "Update nonexistent key");
    assertTest(!Delete(ht, new Element("keyX", 4)), "Delete nonexistent key");

    char longKey[32] = "thiskeyiswaytoolongforthespecs";
    assertTest(!Insert(ht, new Element(longKey, 31, "val", 3)), "Insert too long key");

    char longPayload[300];
    memset(longPayload, 'A', 300);
    assertTest(!Insert(ht, new Element("key2", 4, longPayload, 300)), "Insert too long payload");

    assertTest(Close(ht), "Final Close");
    delete e1;

    return 0;
}
