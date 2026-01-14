#pragma once
#include "Global.h"

//прием первой команды от клиента после подключения, содержащей идентификатор, запрашиваемого сервиса
//получает информацию (сокет и его параметры) о новом подключении через список ListContact(элементы списка создаются и
//заполняются в потоке AcceptServer).

//должен отследить интервал времени(с помощью ожидающего таймера) от момента подключения до получения первой команды, проанализировать команду на
//правильность, а также запустить поток соответствующий запрошенному сервису(и) или оправить диагностирующее сообщение.
DWORD WINAPI DispathServer(LPVOID pPrm) {
	cout << "DispathServer working\n";
	DWORD rc = 0;
	try {
		while (*((TalkersCommand*)pPrm) != EXIT) {//Условие выхода из цикла
			try {
				if (WaitForSingleObject(Event, 300) == WAIT_OBJECT_0) {//перешел в сигнальное состояние

					if (&ClientServiceNumber > (volatile LONG*)0) {
						Contact* client = NULL;
						int libuf = 1;
						char CallBuf[50] = "", SendError[50] = "ErrorInquiry";
						EnterCriticalSection(&scListContact);

						//должен последовательно просматривать список ListContact  (в котором хранятся все сведения о подключившихся клиентах)
						//и считывать запрос, подключившегося клиента
						for (ListContact::iterator p = Contacts.begin(); p != Contacts.end(); p++) {
							if (p->type == Contact::ACCEPT && p->sthread != Contact::ABORT)
							{
								client = &(*p);

								//При обнаружении в списке подключившегося, но не обслуженного клиента (это можно определить  по специальным признакам, 
								//записанным в элемент списка) следует прочитать первое сообщение клиента (которое должно быть кодом запроса) с помощью
								//функции recv (раздел 3.11).
								bool flag = false;
								while (flag == false) {
									if ((libuf = recv(client->s, CallBuf, sizeof(CallBuf), NULL)) == SOCKET_ERROR) {
										switch (WSAGetLastError()) {
											case WSAEWOULDBLOCK: Sleep(100); break;
											default: throw  SetErrorMsgText("Recv:", WSAGetLastError());
										}
									}
									else flag = true;
								}

								// Пытаемся найти сервис в загруженных библиотеках (поддержка любых сервисов)
								if (flag == true && strlen(CallBuf) > 0) {
									client->type = Contact::CONTACT;
									strcpy_s(client->srvname, CallBuf);

									//ожидающий таймер будет создаваться потоком AcceptServer
									//для каждого запущенного потока EchoSever.Сразу же после перехода
									//одного из ожидающих таймеров, в очередь асинхронных процедур потока
									//AcceptServer будет поставлена соответствующая процедура завершения.
									// 
									//переходит в сигнальное состояние через три минуты после запуска обслуживающего 
									//сервера (т.е. три минуты – это максимально допустимое время работы обслуживающего сервера).
									client->htimer = CreateWaitableTimer(NULL, false, NULL); // true - ручной сброс, false - автоматический
									// Время в 100-наносекундных интервалах: 3 минуты = 180 секунд = 180 * 10^7 интервалов = 1,800,000,000
									_int64 time = -1800000000LL; // 180 секунд * 10^7 интервалов в секунде = 1,800,000,000 интервалов
									SetWaitableTimer(client->htimer, (LARGE_INTEGER*)&time, 0, ASWTimer, client, false); // 0 - не периодическая
									
									// Ищем сервис во всех загруженных библиотеках
									client->hthread = FindServiceInLibraries(CallBuf, client);
									
									if (client->hthread != NULL) {
										// Сервис найден
										cout << CallBuf << " server called" << endl;
										if ((libuf = send(client->s, CallBuf, sizeof(CallBuf), NULL)) == SOCKET_ERROR) throw SetErrorMsgText("Send:", WSAGetLastError());
									}
									else {
										// Сервис не найден (все библиотеки выгружены или сервис не существует)
										char errorMsg[50] = "ErrorInquiry";
										if ((libuf = send(client->s, errorMsg, sizeof(errorMsg) + 1, NULL)) == SOCKET_ERROR) throw SetErrorMsgText("Send:", WSAGetLastError());
										closesocket(client->s);
										client->sthread = Contact::ABORT;
										CancelWaitableTimer(client->htimer);
										InterlockedIncrement(&Fail);
										cout << "Service " << CallBuf << " not found or all libraries unloaded" << endl;
									}
								}
								else {
								//обслуживающий сервер, завершает свою работу до истечения  
								//установленного максимального интервала его работы
									if ((libuf = send(client->s, SendError, sizeof(SendError) + 1, NULL)) == SOCKET_ERROR) throw SetErrorMsgText("Send:", WSAGetLastError());
									closesocket(client->s);
									client->sthread = Contact::ABORT;
									CancelWaitableTimer(client->htimer);
									InterlockedIncrement(&Fail);
								}
							}
						}
						LeaveCriticalSection(&scListContact);
					}
					SleepEx(0, true);
				}
				SleepEx(0, true);
			}
			catch (string errorMsgText) {
				std::cout << errorMsgText << endl;
			}
		}
	}
	catch (string errorMsgText) {
		std::cout << errorMsgText << endl;
	}
	catch (...) {
		std::cout << "Error DispathServer" << std::endl;
	}
	cout << "DispathServer stopped\n" << endl;
	ExitThread(rc);
}