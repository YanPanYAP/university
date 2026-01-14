#include <iostream>
#include <clocale>
#include "Winsock2.h"
#include <Windows.h>
#pragma comment(lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;

string GetErrorMsgText(int code)
{
    string msgText;
    switch (code)
    {
    case WSAEINTR:          msgText = "WSAEINTR";         break;
    case WSAEACCES:         msgText = "WSAEACCES";        break;
    case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
    case WSAETIMEDOUT:      msgText = "WSAETIMEDOUT";     break;
    default:                msgText = "***ERROR***";      break;
    };
    return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
    return msgText + GetErrorMsgText(code);
}

// -- отправить широковещательный запрос и получить ответ от сервера
bool GetServer(char* call, short port, sockaddr* addr, int* flen, SOCKET cC)
{
    // Разрешить широковещание
    BOOL optval = TRUE;
    if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
        throw SetErrorMsgText("setsockopt SO_BROADCAST:", WSAGetLastError());

    // Установить таймаут на прием
    int timeout = 3000;
    if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
        throw SetErrorMsgText("setsockopt SO_RCVTIMEO:", WSAGetLastError());

    // Широковещательный адрес
    SOCKADDR_IN to;
    to.sin_family = AF_INET;
    to.sin_port = htons(port);
    to.sin_addr.s_addr = inet_addr("172.20.10.15");

    // Отправка позывного
    int lb = sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&to, sizeof(to));
    if (lb == SOCKET_ERROR)
        throw SetErrorMsgText("sendto:", WSAGetLastError());

    // Ожидание ответа
    char recvBuf[512];
    lb = recvfrom(cC, recvBuf, sizeof(recvBuf), NULL, addr, flen);
    if (lb == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err == WSAETIMEDOUT)
            return false;
        else
            throw SetErrorMsgText("recvfrom:", err);
    }

    recvBuf[lb] = '\0';
    cout << "[RECV] Ответ от сервера: " << recvBuf << endl;

    if (strcmp(recvBuf, call) == 0)
        return true;
    else
        return false;
}

int main()
{
    setlocale(LC_ALL, "ru");

    SOCKET cC;
    WSADATA wsaData;

    try
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());

        cC = socket(AF_INET, SOCK_DGRAM, NULL);
        if (cC == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        SOCKADDR_IN serverAddr;
        int addrLen = sizeof(serverAddr);

        cout << "ClientB: Поиск сервера с позывным \"Hello\"...\n";

        if (GetServer((char*)"Hello", 2000, (sockaddr*)&serverAddr, &addrLen, cC))
        {
            cout << "[FOUND] Сервер найден!\n";
            cout << "IP: " << inet_ntoa(serverAddr.sin_addr)
                << ", Port: " << ntohs(serverAddr.sin_port) << endl;
        }
        else
        {
            cout << "[NOT FOUND] Сервер не откликнулся или позывной неверный.\n";
        }

        if (closesocket(cC) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << "\nWSAGetLastError: " << errorMsgText << endl;
    }

    return 0;
}
