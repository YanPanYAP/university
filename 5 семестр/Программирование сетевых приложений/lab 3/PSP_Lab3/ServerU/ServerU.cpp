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
	SOCKET  sS;
	WSADATA wsaData;
	try
	{

		// 1.
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());


		// 2.
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		//							
		serv.sin_addr.s_addr = INADDR_ANY;
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("bind:", WSAGetLastError());



		SOCKADDR_IN clnt;
		int lc = sizeof(clnt);
		char ibuf[512];
		int lb = 0;

		srand((unsigned)time(NULL));


		cout << "Server is running...\n";


		// =======================  TASK 7  =======================

		//int i = 0;
		while (true)
		{
			int sentCount = 0;
			int lostPacketIndex = rand() % 100;
			bool packetDropped = false;

			int lcInt = sizeof(clnt);
			int time = clock();
			while (true)
			{
				//Sleep(1000);
				if (lb = recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lc) == SOCKET_ERROR)
					throw SetErrorMsgText("recvfrom:", WSAGetLastError());

				

				cout << "[RECV] " << ibuf << " (packet #" << sentCount << ")\n";
				//i++;

				if (!strcmp(ibuf, "CLOSE"))
					break;

				Sleep(500);

				if (sentCount == lostPacketIndex && !packetDropped)
				{
					packetDropped = true;
					cout << "[DROP] Packet #" << sentCount << " intentionally lost\n";
				}
				else
				{
					if ((lb = sendto(sS, ibuf, strlen(ibuf) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt))) == SOCKET_ERROR)
						throw SetErrorMsgText("sendto:", WSAGetLastError());
				}
				sentCount++;

			}
			int timeEnd = clock();
			cout << "\nSession duration: " << (timeEnd - time) << " ticks or "
				<< ((float)(timeEnd - time)) / CLOCKS_PER_SEC << " seconds.\n\n";

			sentCount = 0;
			lostPacketIndex = rand() % 100;

		}





		// 5.
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
