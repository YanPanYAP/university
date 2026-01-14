#include <iostream>
#include <clocale>
#include "Winsock2.h"
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
    default:                msgText = "***ERROR***";      break;
    };
    return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
    return msgText + GetErrorMsgText(code);
}

int main()
{
    SOCKET cC;
    WSADATA wsaData;

    try
    {
        int count;
        cout << "Enter number of messages:\n";
        cin >> count;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());

        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        // Установка таймаута на прием (500 мс)
        int timeout = 500;
        setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = inet_addr("172.20.10.12");
        int lc = sizeof(serv);

        char obuf[50] = "Hello from ClientU";
        int lobuf = 0;

        for (int i = 0; i < count; i++)
        {
            if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, lc)) == SOCKET_ERROR)
            {
                cerr << SetErrorMsgText("sendto:", WSAGetLastError()) << endl;
                continue;
            }

            Sleep(200);

            if ((lobuf = recvfrom(cC, obuf, sizeof(obuf), NULL, (sockaddr*)&serv, &lc)) == SOCKET_ERROR)
            {
                cerr << "[MISS] No response for packet #" << i << endl;
                continue; // не прерываем цикл
            }

            cout << "[RECV] " << obuf << " (packet " << i << ")\n";
        }

        strcpy(obuf, "CLOSE");
        sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, lc);

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
