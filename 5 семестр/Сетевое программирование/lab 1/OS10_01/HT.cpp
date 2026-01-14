#include "HT.h"
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdint>

using namespace std;

namespace HT {

    // -------------------- Константы формата --------------------

    // Header layout (фиксированный размер)
    static const int HDR_MAGIC_OFF = 0;   // 8 байт: "HTSNAP10"
    static const int HDR_VERSION_OFF = 8;   // uint32
    static const int HDR_CAP_OFF = 12;  // int32
    static const int HDR_SNAPINT_OFF = 16;  // int32
    static const int HDR_MAXKEY_OFF = 20;  // int32
    static const int HDR_MAXPAY_OFF = 24;  // int32
    static const int HDR_LASTSNAP_OFF = 28;  // int64
    static const int HEADER_SIZE = 36;  // 8 + 4*5 + 8

    enum BucketState : uint8_t { B_EMPTY = 0, B_OCCUPIED = 1, B_DELETED = 2 };

    // -------------------- Утилиты --------------------

    static void setError(HTHANDLE* ht, const char* msg) {   // установка сообщения об ошибке
        strncpy_s(ht->LastErrorMessage, msg, _TRUNCATE);
    }

    static uint64_t fnv1a(const uint8_t* data, size_t len) {    // хэш-функция, вычисляет 64-битный хэш по ключу
        const uint64_t FNV_OFFSET = 1469598103934665603ull;
        const uint64_t FNV_PRIME = 1099511628211ull;
        uint64_t h = FNV_OFFSET;
        for (size_t i = 0; i < len; ++i) { h ^= data[i]; h *= FNV_PRIME; }
        return h;
    }

    static inline int bucketSize(const HTHANDLE* ht) {  // вычисляет размер одного бакета
        // state(1) + keylen(4) + paylen(4) + key[MaxKeyLength] + payload[MaxPayloadLength]
        return 1 + 4 + 4 + ht->MaxKeyLength + ht->MaxPayloadLength;
    }
    static inline uint64_t fileSize(const HTHANDLE* ht) {   // размер файла
        return uint64_t(HEADER_SIZE) + uint64_t(ht->Capacity) * uint64_t(bucketSize(ht));
    }

    static inline uint8_t* basePtr(const HTHANDLE* ht) {    // начало отображенной памяти
        return reinterpret_cast<uint8_t*>(ht->Addr);
    }
    static inline uint8_t* headerPtr(const HTHANDLE* ht) {  // указатель на заголовок
        return basePtr(ht);
    }
    static inline uint8_t* bucketPtr(const HTHANDLE* ht, int idx) { // указатель на i-тый бакет в таблице
        return basePtr(ht) + HEADER_SIZE + size_t(idx) * size_t(bucketSize(ht));
    }

    // bucket accessors - доступ к полям данным
    static inline BucketState b_state(uint8_t* b) { return (BucketState)b[0]; }
    static inline void        b_set_state(uint8_t* b, BucketState s) { b[0] = (uint8_t)s; }
    static inline int         b_keylen(uint8_t* b) { int v; memcpy(&v, b + 1, 4); return v; }
    static inline void        b_set_keylen(uint8_t* b, int v) { memcpy(b + 1, &v, 4); }
    static inline int         b_paylen(uint8_t* b) { int v; memcpy(&v, b + 5, 4); return v; }
    static inline void        b_set_paylen(uint8_t* b, int v) { memcpy(b + 5, &v, 4); }
    static inline uint8_t* b_key_area(const HTHANDLE* ht, uint8_t* b) { return b + 9; }
    static inline uint8_t* b_pay_area(const HTHANDLE* ht, uint8_t* b) { return b + 9 + ht->MaxKeyLength; }

    // линейное пробирование
    static int probe_find(const HTHANDLE* ht, const void* key, int keylen, int& foundIndex) {
        const uint8_t* k = reinterpret_cast<const uint8_t*>(key);
        const int cap = ht->Capacity;
        uint64_t h = fnv1a(k, size_t(keylen));
        int firstDeleted = -1;
        foundIndex = -1;

        for (int i = 0; i < cap; ++i) {
            int idx = int((h + (uint64_t)i) % (uint64_t)cap);
            uint8_t* b = bucketPtr(ht, idx);
            BucketState st = b_state(b);

            if (st == B_EMPTY) {
                return (firstDeleted != -1) ? firstDeleted : idx;
            }
            if (st == B_DELETED) {
                if (firstDeleted == -1) firstDeleted = idx;
                continue;
            }
            // OCCUPIED
            int klen = b_keylen(b);
            if (klen == keylen) {
                if (memcmp(b_key_area(ht, b), k, keylen) == 0) {
                    foundIndex = idx;
                    return idx;
                }
            }
        }
        return (firstDeleted != -1) ? firstDeleted : -1;
    }

    // -------------------- Конструкторы --------------------

    HTHANDLE::HTHANDLE() {
        Capacity = 0;
        SecSnapshotInterval = 0;
        MaxKeyLength = 0;
        MaxPayloadLength = 0;
        File = NULL;
        FileMapping = NULL;
        Addr = NULL;
        LastErrorMessage[0] = '\0';
        lastsnaptime = 0;
        FileName[0] = '\0';
    }

    HTHANDLE::HTHANDLE(int capacity, int interval, int keylen, int payloadlen, const char filename[512]) {
        Capacity = capacity;
        SecSnapshotInterval = interval;
        MaxKeyLength = keylen;
        MaxPayloadLength = payloadlen;
        File = NULL;
        FileMapping = NULL;
        Addr = NULL;
        LastErrorMessage[0] = '\0';
        lastsnaptime = 0;
        strcpy_s(FileName, filename);
    }

    Element::Element() {
        key = nullptr;
        keylength = 0;
        payload = nullptr;
        payloadlength = 0;
    }

    Element::Element(const void* k, int kl) {
        keylength = kl;
        payload = nullptr;
        payloadlength = 0;
        key = malloc(kl);
        memcpy((void*)key, k, kl);
    }

    Element::Element(const void* k, int kl, const void* p, int pl) {
        keylength = kl;
        payloadlength = pl;
        key = malloc(kl);
        payload = malloc(pl);
        memcpy((void*)key, k, kl);
        memcpy((void*)payload, p, pl);
    }

    Element::Element(Element* oldelement, const void* newpayload, int newpayloadlength) {
        keylength = oldelement->keylength;
        key = malloc(keylength);
        memcpy((void*)key, oldelement->key, keylength);
        payloadlength = newpayloadlength;
        payload = malloc(payloadlength);
        memcpy((void*)payload, newpayload, payloadlength);
    }

    // -------------------- Сервисные --------------------

    char* GetLastError(HTHANDLE* ht) {
        return ht->LastErrorMessage;
    }

    void print(const Element* element) {
        cout << "Key[" << element->keylength << "]: ";
        for (int i = 0; i < element->keylength; ++i) cout << ((const char*)element->key)[i];
        cout << " | Payload[" << element->payloadlength << "]: ";
        for (int i = 0; i < element->payloadlength; ++i) cout << ((const char*)element->payload)[i];
        cout << endl;
    }

    // -------------------- Header I/O --------------------

    static void init_header(HTHANDLE* ht) { // запись заголовка файла при создании новой хэш-таблицы
        uint8_t* h = headerPtr(ht);
        const char magic[8] = { 'H','T','S','N','A','P','1','0' };
        memcpy(h + HDR_MAGIC_OFF, magic, 8);
        uint32_t ver = 1;
        memcpy(h + HDR_VERSION_OFF, &ver, 4);
        memcpy(h + HDR_CAP_OFF, &ht->Capacity, 4);
        memcpy(h + HDR_SNAPINT_OFF, &ht->SecSnapshotInterval, 4);
        memcpy(h + HDR_MAXKEY_OFF, &ht->MaxKeyLength, 4);
        memcpy(h + HDR_MAXPAY_OFF, &ht->MaxPayloadLength, 4);
        int64_t ts = (int64_t)time(NULL);
        memcpy(h + HDR_LASTSNAP_OFF, &ts, 8);
        ht->lastsnaptime = (time_t)ts;
    }

    static bool read_header(HTHANDLE* ht) { // прочитать заголовок
        uint8_t* h = headerPtr(ht);
        char magic[8]; memcpy(magic, h + HDR_MAGIC_OFF, 8);
        const char expected[8] = { 'H','T','S','N','A','P','1','0' };
        if (memcmp(magic, expected, 8) != 0) { setError(ht, "Open: bad magic"); return false; }

        uint32_t ver = 0; memcpy(&ver, h + HDR_VERSION_OFF, 4);
        if (ver != 1) { setError(ht, "Open: bad version"); return false; }

        memcpy(&ht->Capacity, h + HDR_CAP_OFF, 4);
        memcpy(&ht->SecSnapshotInterval, h + HDR_SNAPINT_OFF, 4);
        memcpy(&ht->MaxKeyLength, h + HDR_MAXKEY_OFF, 4);
        memcpy(&ht->MaxPayloadLength, h + HDR_MAXPAY_OFF, 4);
        int64_t ts = 0; memcpy(&ts, h + HDR_LASTSNAP_OFF, 8);
        ht->lastsnaptime = (time_t)ts;

        return true;
    }

    static void init_buckets(HTHANDLE* ht) {    // инициализация всех бакетов как пустых
        const int cap = ht->Capacity;
        const int bsz = bucketSize(ht);
        uint8_t* p = basePtr(ht) + HEADER_SIZE;
        for (int i = 0; i < cap; ++i) {
            p[0] = (uint8_t)B_EMPTY;
            memset(p + 1, 0, size_t(bsz - 1));
            p += bsz;
        }
    }

    // -------------------- API --------------------

    HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]) {
        HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);

        // Create file
        ht->File = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_WRITE_THROUGH, NULL);
        if (ht->File == INVALID_HANDLE_VALUE) { setError(ht, "Create: CreateFile failed"); delete ht; return nullptr; }

        // Size file
        LARGE_INTEGER sz; sz.QuadPart = fileSize(ht);
        LARGE_INTEGER newpos;
        if (!SetFilePointerEx(ht->File, sz, &newpos, FILE_BEGIN) || !SetEndOfFile(ht->File)) {
            setError(ht, "Create: SetEndOfFile failed");
            CloseHandle(ht->File); delete ht; return nullptr;
        }

        // Mapping
        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!ht->FileMapping) { setError(ht, "Create: CreateFileMapping failed"); CloseHandle(ht->File); delete ht; return nullptr; }

        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (!ht->Addr) { setError(ht, "Create: MapViewOfFile failed"); CloseHandle(ht->FileMapping); CloseHandle(ht->File); delete ht; return nullptr; }

        // Init header + buckets
        init_header(ht);
        init_buckets(ht);

        return ht;
    }

    HTHANDLE* Open(const char FileName[512]) {
        HTHANDLE* ht = new HTHANDLE();
        strcpy_s(ht->FileName, FileName);

        ht->File = CreateFileA(FileName, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS | FILE_FLAG_WRITE_THROUGH, NULL);
        if (ht->File == INVALID_HANDLE_VALUE) { setError(ht, "Open: CreateFile failed"); delete ht; return nullptr; }

        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!ht->FileMapping) { setError(ht, "Open: CreateFileMapping failed"); CloseHandle(ht->File); delete ht; return nullptr; }

        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (!ht->Addr) { setError(ht, "Open: MapViewOfFile failed"); CloseHandle(ht->FileMapping); CloseHandle(ht->File); delete ht; return nullptr; }

        if (!read_header(ht)) {
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht; return nullptr;
        }
        return ht;
    }

    BOOL Snap(const HTHANDLE* h) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!FlushViewOfFile(ht->Addr, 0)) { setError(ht, "Snap: FlushViewOfFile failed"); return FALSE; }
        if (!FlushFileBuffers(ht->File)) { setError(ht, "Snap: FlushFileBuffers failed"); return FALSE; }
        int64_t ts = (int64_t)time(NULL);
        memcpy(headerPtr(ht) + HDR_LASTSNAP_OFF, &ts, 8);
        ht->lastsnaptime = (time_t)ts;
        return TRUE;
    }

    BOOL Close(const HTHANDLE* h) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        Snap(ht);
        if (ht->Addr) UnmapViewOfFile(ht->Addr);
        if (ht->FileMapping) CloseHandle(ht->FileMapping);
        if (ht->File && ht->File != INVALID_HANDLE_VALUE) CloseHandle(ht->File);
        delete ht;
        return TRUE;
    }

    BOOL Insert(const HTHANDLE* h, const Element* e) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!e || !e->key || e->keylength <= 0) { setError(ht, "Insert: bad key"); return FALSE; }
        if (!e->payload || e->payloadlength < 0) { setError(ht, "Insert: bad payload"); return FALSE; }
        if (e->keylength > ht->MaxKeyLength) { setError(ht, "Insert: key too long"); return FALSE; }
        if (e->payloadlength > ht->MaxPayloadLength) { setError(ht, "Insert: payload too long"); return FALSE; }

        int found = -1;
        int idx = probe_find(ht, e->key, e->keylength, found);
        if (idx < 0) { setError(ht, "Insert: no slot"); return FALSE; }
        if (found >= 0) { setError(ht, "Insert: duplicate key"); return FALSE; }

        uint8_t* b = bucketPtr(ht, idx);
        b_set_state(b, B_OCCUPIED);
        b_set_keylen(b, e->keylength);
        b_set_paylen(b, e->payloadlength);
        memcpy(b_key_area(ht, b), e->key, size_t(e->keylength));
        if (e->payloadlength > 0)
            memcpy(b_pay_area(ht, b), e->payload, size_t(e->payloadlength));

        return TRUE;
    }

    Element* Get(const HTHANDLE* h, const Element* e) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!e || !e->key || e->keylength <= 0) { setError(ht, "Get: bad key"); return nullptr; }
        if (e->keylength > ht->MaxKeyLength) { setError(ht, "Get: key too long"); return nullptr; }

        int found = -1;
        (void)probe_find(ht, e->key, e->keylength, found);
        if (found < 0) return nullptr;

        uint8_t* b = bucketPtr(ht, found);
        int paylen = b_paylen(b);
        uint8_t* pay = b_pay_area(ht, b);

        Element* out = new Element(e->key, e->keylength);
        if (paylen > 0) {
            out->payload = malloc(size_t(paylen));
            memcpy((void*)out->payload, pay, size_t(paylen));
            out->payloadlength = paylen;
        }
        else {
            out->payload = nullptr;
            out->payloadlength = 0;
        }
        return out;
    }

    BOOL Update(const HTHANDLE* h, const Element* olde, const void* newpayload, int newpayloadlength) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!olde || !olde->key || olde->keylength <= 0) { setError(ht, "Update: bad key"); return FALSE; }
        if (newpayloadlength < 0) { setError(ht, "Update: bad payload length"); return FALSE; }
        if (olde->keylength > ht->MaxKeyLength) { setError(ht, "Update: key too long"); return FALSE; }
        if (newpayloadlength > ht->MaxPayloadLength) { setError(ht, "Update: payload too long"); return FALSE; }

        int found = -1;
        (void)probe_find(ht, olde->key, olde->keylength, found);
        if (found < 0) { setError(ht, "Update: key not found"); return FALSE; }

        uint8_t* b = bucketPtr(ht, found);
        b_set_paylen(b, newpayloadlength);
        if (newpayloadlength > 0)
            memcpy(b_pay_area(ht, b), newpayload, size_t(newpayloadlength));
        else
            memset(b_pay_area(ht, b), 0, size_t(ht->MaxPayloadLength));
        return TRUE;
    }

    BOOL Delete(const HTHANDLE* h, const Element* e) {
        HTHANDLE* ht = const_cast<HTHANDLE*>(h);
        if (!e || !e->key || e->keylength <= 0) { setError(ht, "Delete: bad key"); return FALSE; }
        if (e->keylength > ht->MaxKeyLength) { setError(ht, "Delete: key too long"); return FALSE; }

        int found = -1;
        (void)probe_find(ht, e->key, e->keylength, found);
        if (found < 0) { setError(ht, "Delete: key not found"); return FALSE; }

        uint8_t* b = bucketPtr(ht, found);
        b_set_state(b, B_DELETED);
        b_set_keylen(b, 0);
        b_set_paylen(b, 0);
        memset(b_key_area(ht, b), 0, size_t(ht->MaxKeyLength + ht->MaxPayloadLength));
        return TRUE;
    }

}
