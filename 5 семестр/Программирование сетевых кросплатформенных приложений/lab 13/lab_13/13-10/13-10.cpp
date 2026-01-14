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

    setlocale(LC_ALL, "ru");

    SOCKET cC;
    WSADATA wsaData;

    try
    {

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw SetErrorMsgText("Startup:", WSAGetLastError());

        if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket:", WSAGetLastError());


        SOCKADDR_IN serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(3000);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");
        int lc = sizeof(serv);


        const char* obuf = "Client: Hello world";


        if (sendto(cC, obuf, strlen(obuf) + 1, 0, (sockaddr*)&serv, lc) == SOCKET_ERROR)
            throw SetErrorMsgText("sendto: ", WSAGetLastError());
        else
            cout << "Client: сообщение отправлено серверу" << endl;


        char ibuf[512];
        int lb = recvfrom(cC, ibuf, sizeof(ibuf) - 1, 0, (sockaddr*)&serv, &lc);
        if (lb == SOCKET_ERROR)
            throw SetErrorMsgText("recvfrom: ", WSAGetLastError());

        ibuf[lb] = '\0';
        cout << "Client: от сервера получено: " << ibuf << endl;
        cout << "Client: получено " << lb << " байтов от "
            << inet_ntoa(serv.sin_addr) << ":" << ntohs(serv.sin_port) << endl;

        if (closesocket(cC) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());
        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
    }
    catch (string errorMsgText)
    {
        cout << "\nWSAGetLastError: " << errorMsgText << endl;
    }

    return 0;
}
