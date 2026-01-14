#include "pch.h"
#include <Windows.h>
#include <string>
#include <objbase.h>
#include <iostream>
#include "Interface.h" 
#include "OS13_HTCOM.h" 
#pragma comment(lib, "ole32.lib") 
#pragma comment(lib, "uuid.lib")


// --- Глобальное состояние для COM ---
// Мы будем держать один экземпляр COM-объекта в памяти, пока клиент не вызовет Close.
static IHT* g_pIHT = NULL;
static bool g_comInitialized = false;

// --- Вспомогательные функции конвертации ---

// WCHAR* (COM) -> char* (HT API/ANSI)
std::string WStringToString(const WCHAR* wstr) {
    if (!wstr) return "";
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, &strTo[0], size_needed, NULL, NULL);
    if (!strTo.empty() && strTo.back() == '\0') strTo.pop_back();
    return strTo;
}

// char* (HT API/ANSI) -> WCHAR* (COM)
std::wstring StringToWString(const char* str) {
    if (!str) return L"";
    int size_needed = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstrTo[0], size_needed);
    return wstrTo;
}

// --- Инициализация COM-компонента ---
// Вызывается из HT::Create и HT::Open
bool InitializeCOM() {
    if (g_pIHT != NULL) return true;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    // Допускаем S_OK, S_FALSE, и игнорируем RPC_E_CHANGED_MODE
    if (SUCCEEDED(hr)) {
        g_comInitialized = true;
    }
    else if (hr != S_FALSE && hr != RPC_E_CHANGED_MODE) {
        fprintf(stderr, "CoInitializeEx FAILED! HRESULT: 0x%lX\n", hr);
        return false;
    }

    // Создание экземпляра COM-компонента
    hr = CoCreateInstance(CLSID_OS13, NULL, CLSCTX_INPROC_SERVER, IID_IHT, (void**)&g_pIHT);


    if (FAILED(hr)) {
        std::cerr << "CoCreateInstance failed! HRESULT: 0x" << std::hex << hr << std::endl;
        if (g_comInitialized) CoUninitialize();
        g_pIHT = NULL;
        return false;
    }
    return true;
}


// ====================================================================
// HT API Implementation (Wrapper)
// ====================================================================

namespace HT {

    static void* copyData(const void* src, int len) {
        if (!src || len <= 0) return NULL;
        void* dest = malloc(len);
        if (dest) memcpy(dest, src, len);
        return dest;
    }

    // Конструктор HT::Element (Key only)
    Element::Element(const void* key, int keylen) :
        keylength(keylen), payloadlength(0), key(nullptr), payload(nullptr) {
        this->key = copyData(key, keylen);
    }

    // Конструктор HT::Element (Key and Payload) - тот, на который ругается компоновщик
    Element::Element(const void* key, int keylen, const void* payload, int paylen) :
        keylength(keylen), payloadlength(paylen), key(nullptr), payload(nullptr) {
        this->key = copyData(key, keylen);
        this->payload = copyData(payload, paylen);
    }


    HTHANDLE* Create(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
    {
        if (!InitializeCOM()) return NULL;

        std::wstring wFile = StringToWString(FileName);
        unsigned __int64 hHandle = 0;

        // Вызов COM-метода
        HRESULT hr = g_pIHT->Create(
            Capacity,
            SecSnapshotInterval,
            MaxKeyLength,
            MaxPayloadLength,
            wFile.c_str(),
            hHandle
        );

        if (FAILED(hr)) {
            // В случае ошибки, мы не вызываем Release, чтобы клиент мог получить сообщение об ошибке
            // через GetLastError
            return NULL;
        }

        // Преобразуем 64-битный адрес обратно в указатель HTHANDLE*
        return reinterpret_cast<HTHANDLE*>(hHandle);
    }


    HTHANDLE* Open(const char FileName[512], bool isStartMode)
    {
        if (!InitializeCOM()) return NULL;

        std::wstring wFile = StringToWString(FileName);
        unsigned __int64 hHandle = 0;

        HRESULT hr = g_pIHT->Open(wFile.c_str(), isStartMode, hHandle);

        if (FAILED(hr)) {
            fprintf(stderr, "\n=== HT::Open COM FAILED ===\n");
            fprintf(stderr, "g_pIHT->Open HRESULT: 0x%lX\n", hr);
            return NULL;
        }

        return reinterpret_cast<HTHANDLE*>(hHandle);
    }

    BOOL Snap(const HTHANDLE* hthandle)
    {
        if (!g_pIHT || !hthandle) return FALSE;

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);
        HRESULT hr = g_pIHT->Snap(hHandle);
        return SUCCEEDED(hr);
    }

    BOOL Close(const HTHANDLE* hthandle)
    {
        if (!g_pIHT || !hthandle) return FALSE;

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);
        HRESULT hr = g_pIHT->Close(hHandle);

        // Очистка COM-состояния обертки после Close HT
        if (g_pIHT) {
            g_pIHT->Release();
            g_pIHT = NULL;
        }
        if (g_comInitialized) {
            CoUninitialize();
            g_comInitialized = false;
        }

        return SUCCEEDED(hr);
    }

    BOOL Insert(const HTHANDLE* hthandle, const Element* element)
    {
        if (!g_pIHT || !hthandle || !element) return FALSE;

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);

        // Конвертация ключа и полезной нагрузки
        std::wstring wKey = StringToWString((const char*)element->key);
        std::wstring wPayload = StringToWString((const char*)element->payload);

        HRESULT hr = g_pIHT->Insert(
            hHandle,
            wKey.c_str(), (int)wKey.length(),
            wPayload.c_str(), (int)wPayload.length()
        );

        return SUCCEEDED(hr);
    }

    BOOL Delete(const HTHANDLE* hthandle, const Element* element)
    {
        if (!g_pIHT || !hthandle || !element) return FALSE;

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);
        std::wstring wKey = StringToWString((const char*)element->key);

        HRESULT hr = g_pIHT->Delete(
            hHandle,
            wKey.c_str(), (int)wKey.length()
        );

        return SUCCEEDED(hr);
    }

    Element* Get(const HTHANDLE* hthandle, const Element* element)
    {
        if (!g_pIHT || !hthandle || !element) return NULL;

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);
        std::wstring wKey = StringToWString((const char*)element->key);

        WCHAR* wResultPayload = NULL;
        int resultPayloadLen = 0;

        HRESULT hr = g_pIHT->Get(
            hHandle,
            wKey.c_str(), (int)wKey.length(),
            &wResultPayload,
            resultPayloadLen
        );

        if (FAILED(hr) || !wResultPayload) {
            // COM-DLL не нашла элемент
            return NULL;
        }

        // 1. Конвертируем WCHAR* в char* для возврата через HT::Element*
        std::string sPayload = WStringToString(wResultPayload);

        // 2. Освобождаем память, выделенную COM-DLL (стандарт COM)
        CoTaskMemFree(wResultPayload);

        // 3. Создаем новый HT::Element для возврата
        HT::Element* result = new HT::Element(
            element->key, element->keylength, // Используем ключ запроса для создания
            sPayload.c_str(), (int)sPayload.length() + 1
        );

        return result;
    }

    BOOL Update(const HTHANDLE* hthandle, const Element* oldElement, const void* newPayload, int newPayloadLength)
    {
        if (!g_pIHT || !hthandle || !oldElement || !newPayload) return FALSE;

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);

        std::wstring wKey = StringToWString((const char*)oldElement->key);
        std::string sNewPayload((const char*)newPayload, newPayloadLength);
        std::wstring wNewPayload = StringToWString(sNewPayload.c_str());

        HRESULT hr = g_pIHT->Update(
            hHandle,
            wKey.c_str(), (int)wKey.length(),
            wNewPayload.c_str(), (int)wNewPayload.length()
        );

        return SUCCEEDED(hr);
    }

    char* GetLastError(HTHANDLE* hthandle)
    {
        if (!g_pIHT || !hthandle) return _strdup("HT-API Error: COM interface not ready or invalid handle.");

        unsigned __int64 hHandle = reinterpret_cast<unsigned __int64>(hthandle);
        WCHAR* wErrorMsg = NULL;

        HRESULT hr = g_pIHT->GetLastErrorMessage(hHandle, &wErrorMsg);

        if (FAILED(hr) || !wErrorMsg) {
            return _strdup("Unknown error occurred during GetLastErrorMessage call.");
        }

        // 1. Конвертируем WCHAR* в char*
        std::string sErrorMsg = WStringToString(wErrorMsg);

        // 2. Освобождаем память, выделенную COM-DLL (стандарт COM)
        CoTaskMemFree(wErrorMsg);

        // 3. Возвращаем копию строки (используем _strdup для выделения памяти, которую клиент должен освободить)
        return _strdup(sErrorMsg.c_str());
    }
}