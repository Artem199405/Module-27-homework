#include "Chat.h"

Chat::Chat()
{
}

Chat::~Chat()
{
    closesocket(connectSocket);
}

void Chat::connectServer()
{
    // WSAStartup
    WSAData wsaData;
    // The requested version of the Winsock library
    WORD DLLVersion = MAKEWORD(2, 1);

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(DLLVersion, &wsaData);
    if (iResult != 0)
    {
        cout << ("WSAStartup failed with error: %d\n", iResult);
        system("pause");
    }

    // Socket address
    SOCKADDR_IN address;
    int sizeAddress = sizeof(address);
    // Set the IP number for communication
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Set the port number for communication
    address.sin_port = htons(port);
    // Using IPv4
    address.sin_family = AF_INET;

    // Creating a socket to connect to the server
    connectSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (connectSocket == INVALID_SOCKET)
    {
        cout << ("Socket failed with error: ", WSAGetLastError());
        system("pause");
    }

    // Connecting to the server
    if (connect(connectSocket, (SOCKADDR*)&address, sizeAddress) != 0)
    {
        cout << "Error: failed connect to server.";
        system("pause");
    }
    else
        cout << "The connection to the server is established." << endl;

}

void Chat::registration()
{
    string login, password, name;
    bool key = true;

    do
    {
        int action;

        cout << "Suggest free logins? (1 - yes; 2 - no): ";
        cin >> action;
        sendServerInt(action);
        if (action == 1)
        {
            string keyword;

            cout << "Enter the keyword: ";
            cin >>  keyword;
            sendServerString(keyword);
            cout << getServerString() << endl;
        }
        cout << "Enter your login: ";
        cin >> login;
        sendServerString(login);
        key = getServerInt();
        cout << getServerString() << endl;
    } while (key == true);

    cout << "Enter the password: ";
    cin >> password;
    sendServerString(password);

    do
    {
        cout << "Enter your username: ";
        cin >> name;
        sendServerString(name);
        key = getServerInt();
        cout << getServerString() << endl;
    } while (key == true);
}

void Chat::authorization()
{
    string login, password;
    bool key = true;

    do
    {
        cout << "Enter your login: ";
        cin >> login;
        sendServerString(login);
        cout << "Enter the password: ";
        cin >> password;
        sendServerString(password);
        key = getServerInt();
        cout << getServerString() << endl;
    } while (key == false);

    nameParticipant = getServerString();
}

void Chat::enteringMessage()
{
    string recipientName, text;
    bool key = false;

    do
    {
        cout << "Enter the name of the user to send the message to, or 'all' - all chat participants: ";
        cin >> recipientName;
        sendServerString(recipientName);
        key = getServerInt();
        if (key == false)
        {
            cout << getServerString() << endl;
        }
    }
    while (key == false);

    sendServerString(nameParticipant);
    cout << "Enter the text of the message: ";
    cin >> text;
    sendServerString(text);
}

void Chat::readMessages()
{
    sendServerString(nameParticipant);
    cout << getServerString() << endl;
    
    int count = getServerInt();
    for (size_t i = 0; i < count; ++i)
    {
        cout << getServerString() << endl;
        cout << getServerString() << endl;
    }

}

int Chat::getServerInt()
{
    int i;

    recv(connectSocket, (char*)&i, sizeof(int), NULL);

    return i;
}

string Chat::getServerString()
{
    int sizeWord;

    recv(connectSocket, (char*)&sizeWord, sizeof(int), NULL);

    char* word = new char[sizeWord + 1];
    word[sizeWord] = '\0';

    recv(connectSocket, word, sizeWord, NULL);

    return word;
}

void Chat::sendServerInt(int action)
{
    send(connectSocket, (char*)&action, sizeof(int), NULL);
}

void Chat::sendServerString(string message)
{
    int sizeMessage = message.size();

    send(connectSocket, (char*)&sizeMessage, sizeof(int), NULL);
    send(connectSocket, message.c_str(), sizeMessage, NULL);
}