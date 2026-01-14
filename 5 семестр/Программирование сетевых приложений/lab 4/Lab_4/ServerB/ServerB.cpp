#include <iostream>
#include <clocale>
#include <ctime>
#include "Winsock2.h"
#include <vector>
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

// -- ожидание запроса от клиента
bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen, SOCKET sS)
{
    char recvBuf[512];
    int lb;

    while (true)
    {
        lb = recvfrom(sS, recvBuf, sizeof(recvBuf), NULL, from, flen);
        if (lb == SOCKET_ERROR)
        {
            int err = WSAGetLastError();
            if (err == WSAETIMEDOUT)
                return false;
            else
                throw SetErrorMsgText("recvfrom:", err);
        }

        recvBuf[lb] = '\0';
        cout << "[RECV] " << recvBuf << endl;

        if (strcmp(recvBuf, name) == 0)
        {
            return true;
        }
        else
        {
            cout << "[IGNORED] Неверный позывной: " << recvBuf << endl;
            return false;
        }
    }
}

// -- отправка ответа клиенту
bool PutAnswerToClient(char* name, sockaddr* to, int* tolen, SOCKET sS)
{
    int lb = sendto(sS, name, strlen(name) + 1, NULL, to, *tolen);
    if (lb == SOCKET_ERROR)
    {
        throw SetErrorMsgText("sendto:", WSAGetLastError());
    }
    return true;
}

vector<SOCKADDR_IN> FindOtherServers(char* call, short port, SOCKET sS)
{
    vector<SOCKADDR_IN> foundServers;

    // Разрешить широковещание
    BOOL optval = TRUE;
    setsockopt(sS, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));

    // Установить таймаут на прием
    int timeout = 2000;
    setsockopt(sS, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    // Широковещательный адрес
    SOCKADDR_IN to;
    to.sin_family = AF_INET;
    to.sin_port = htons(port);
    to.sin_addr.s_addr = inet_addr("172.20.10.15");

    // Отправка позывного
    sendto(sS, call, strlen(call) + 1, NULL, (sockaddr*)&to, sizeof(to));

    // Ожидание ответов
    char recvBuf[512];
    SOCKADDR_IN from;
    int fromLen = sizeof(from);

    while (true)
    {
        int lb = recvfrom(sS, recvBuf, sizeof(recvBuf), NULL, (sockaddr*)&from, &fromLen);
        if (lb == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
                break;
            else
                throw SetErrorMsgText("recvfrom:", WSAGetLastError());
        }

        recvBuf[lb] = '\0';
        if (strcmp(recvBuf, call) == 0)
        {
            foundServers.push_back(from);
        }
    }

    return foundServers;
}


int main()
{
    setlocale(LC_ALL, "ru");

    SOCKET sS;
    WSADATA wsaData;

    try
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());

        sS = socket(AF_INET, SOCK_DGRAM, NULL);
        if (sS == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());

        // Установка таймаута ожидания (например, 10 секунд)
        int timeout = 10000;
        setsockopt(sS, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(2000);
        serv.sin_addr.s_addr = INADDR_ANY;

        if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
            throw SetErrorMsgText("bind:", WSAGetLastError());

        vector<SOCKADDR_IN> others = FindOtherServers((char*)"Hello", 2000, sS);

        if (!others.empty())
        {
            cout << "[WARNING] Найдено " << others.size() << " других серверов с позывным \"Hello\":\n";
            for (auto& srv : others)
            {
                cout << "IP: " << inet_ntoa(srv.sin_addr)
                    << ", Port: " << ntohs(srv.sin_port) << endl;
            }
        }
        else
        {
            cout << "[INFO] Других серверов с позывным \"Hello\" не обнаружено.\n";
        }


        cout << "ServerB запущен. Ожидание позывного...\n";

        // Цикл обработки запросов
        while (true)
        {
            SOCKADDR_IN clnt;
            int lclnt = sizeof(clnt);

            if (GetRequestFromClient((char*)"Hello", 2000, (sockaddr*)&clnt, &lclnt, sS))
            {
                cout << "[MATCH] Получен правильный позывной от клиента: "
                    << inet_ntoa(clnt.sin_addr) << ":" << ntohs(clnt.sin_port) << endl;

                PutAnswerToClient((char*)"Hello", (sockaddr*)&clnt, &lclnt, sS);
                cout << "[SEND] Ответ отправлен клиенту\n";
            }
            else
            {
                cout << "[TIMEOUT] Время ожидания запроса истекло\n";
            }
        }

        if (closesocket(sS) == SOCKET_ERROR)
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
