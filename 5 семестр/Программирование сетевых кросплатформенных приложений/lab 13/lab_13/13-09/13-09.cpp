#include <iostream>
#include <clocale>
#include <ctime>
#include "Winsock2.h"
#include <Windows.h>
#pragma comment (lib, "WS2_32.lib")
#pragma warning(disable:4996)
using namespace std;




string  GetErrorMsgText(int code)
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
};



string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}






int main()
{

	setlocale(LC_ALL, "ru");

	SOCKET  sS;
	WSADATA wsaData;
	try
	{

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());


		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(3000);
		serv.sin_addr.s_addr = INADDR_ANY;

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());


		cout << "Server is running...\n";


		SOCKADDR_IN clnt;
		int lc = sizeof(clnt);
		char ibuf[512];
		int lb = 0;


		while (true)
		{
			lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc);
				if (lb == SOCKET_ERROR)
					throw SetErrorMsgText("recvfrom:", WSAGetLastError());

				if (lb > 0) {
					ibuf[lb] = '\0'; 
					cout << "Server: получено \"" << ibuf << "\" от "
						<< inet_ntoa(clnt.sin_addr) << ":" << ntohs(clnt.sin_port) << endl;

					string echoMsg = "ECHO: " + string(ibuf);

					if (sendto(sS, echoMsg.c_str(), echoMsg.size(), 0, (sockaddr*)&clnt, lc) == SOCKET_ERROR)
						throw SetErrorMsgText("sendto: ", WSAGetLastError());

					cout << "Server: отправлено \"" << echoMsg << "\" клиенту\n";
				}

		}


		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
}
