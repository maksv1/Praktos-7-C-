#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // Инициализация библиотеки Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "Ошибка при инициализации библиотеки Winsock" << endl;
        return 1;
    }

    // Получение информации о сетевом адресе и протоколе
    ADDRINFO* addresult = nullptr;
    ADDRINFO hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    result = getaddrinfo(nullptr, "666", &hints, &addresult);
    if (result != 0) {
        cout << "Ошибка при получении информации о сетевом адресе и протоколе" << endl;
        WSACleanup();
        return 1;
    }

    // Создание сокета для подключения к серверу
    SOCKET ConnectSocket = INVALID_SOCKET;
    ConnectSocket = socket(addresult->ai_family, addresult->ai_socktype, addresult->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Ошибка при создании сокета для подключения к серверу" << endl;
        freeaddrinfo(addresult);
        WSACleanup();
        return 1;
    }

    // Подключение к серверу
    result = connect(ConnectSocket, addresult->ai_addr, (int)addresult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "Ошибка при подключении к серверу" << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addresult);
        WSACleanup();
        return 1;
    }

    // Отправка сообщения на сервер
    const char* sendBuffer = "Hello from Client";
    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);
    if (result == SOCKET_ERROR) {
        cout << "Ошибка при отправке сообщения на сервер" << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addresult);
        WSACleanup();
        return 1;
    }

    // Получение сообщения от сервера
    char recvBuffer[512];
    ZeroMemory(recvBuffer, 512);
    result = recv(ConnectSocket, recvBuffer, 512, 0);
    if (result > 0) {
        cout << "Получено сообщение от сервера: " << recvBuffer << endl;
    }
    else if (result == 0) {
        cout << "Сервер завершил соединение" << endl;
    }
    else {
        cout << "Ошибка при получении сообщения от сервера" << endl;
    }

    // Закрытие сокета и очистка ресурсов
    closesocket(ConnectSocket);
    freeaddrinfo(addresult);
    WSACleanup();
    return 0;
}