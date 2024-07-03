#include "Chat.h"

Chat::Chat()
{
    _loginSource.insert("Log1"); _loginSource.insert("Log2"); _loginSource.insert("Log3");
    _loginSource.insert("Login1"); _loginSource.insert("Login2"); _loginSource.insert("Login3");
    _loginSource.insert("log1"); _loginSource.insert("log2"); _loginSource.insert("log3");
    _loginSource.insert("login1"); _loginSource.insert("login2"); _loginSource.insert("login3");

    log.writeLogFile("Welcome to the chat server!");
}

Chat::~Chat()
{
    closesocket(_listenSocket);
}

void Chat::connectToDatabase()
{
    // Get a connection descriptor
    mysql_init(&mysql);

    // If the descriptor is not received, we output an error message
    if (&mysql == nullptr)
        cout << "Error: can't create MySQL-descriptor" << endl;

    // Connecting to the server (if it is not possible to establish
    //  a connection with the database, we display an error message)
    if (!mysql_real_connect(&mysql, "localhost", "root", "1234", "chatdb", NULL, NULL, 0))
    {
        cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
        log.writeLogFile("Error: can't connect to database!");
    }
    else
    {
        cout << "The connection to the database has been successfully established!" << endl;
        log.writeLogFile("The connection to the database has been successfully established!");
    }

    // Creating a database chat
    mysql_query(&mysql, "CREATE DATABASE IF NOT EXISTS chat");
    mysql_query(&mysql, "USE chat");

    // Creating a table with the data of chat participants
    mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS participants("
                        "id INT AUTO_INCREMENT PRIMARY KEY,"
                        "login VARCHAR(255),"
                        "password VARCHAR(255),"
                        "name VARCHAR(255))");

    // Creating a table with a message history
    mysql_query(&mysql, "CREATE TABLE IF NOT EXISTS messages("
                        "id INT AUTO_INCREMENT PRIMARY KEY,"
                        "id_recipient INT,"
                        "id_sender INT,"
                        "text VARCHAR(255))");
}

void Chat::restoringChat()
{
    MYSQL_RES* res;
    MYSQL_ROW row;

    mysql_query(&mysql, "SELECT * FROM participants");
    
    if ( res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            int id = atoi(row[0]);
            string login = row[1], password = row[2], name = row[3];

            if (login != "" && password != "" && name != "")
                addParticipant(id, login, password, name);
        }
    }
    else
        cout << "MySQL error number " << mysql_error(&mysql);

    for (size_t i = 0; i < _participants.size(); ++i)
    {
        int idRecipient = _participants[i].getId();

        string str;
            
        str.append("SELECT * FROM messages WHERE id_recipient = ")
            .append(to_string(idRecipient));
        mysql_query(&mysql, str.c_str());

        if (res = mysql_store_result(&mysql))
        {
            while (row = mysql_fetch_row(res))
            {
                int idSender = atoi(row[2]);
                string text = row[3];
                string nameSender;
                
                for (size_t j = 0; j < _participants.size(); ++j)
                    if (idSender == _participants[j].getId())
                        nameSender = _participants[j].getName();

                _participants[i].recordMessage(nameSender, text);
            }
        }
        else
            cout << "MySQL error number " << mysql_error(&mysql);
    }

    int action;

    log.writeLogFile("The chat was restored successfully!");
    do
    {
        cout << "Do you want to read log.txt file? (1 - Yes; 2 - Not): ";
        cin >> action;

        if (action == 1)
        {
            // Reading log file
            log.readLogFile();
            action = 2;
        }
        else if (action == 2)
        {
        }
        else
        {
            cout << "Invalid operator, repeat the input" << endl;
        }
    } while (action != 2);
}

void Chat::runServer()
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
        cout << ("WSAStartup failed with error: ", iResult);
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

    // Creating a socket for the server that will listen for client connections
    _listenSocket = socket(AF_INET, SOCK_STREAM, NULL);
    if (_listenSocket == INVALID_SOCKET)
    {
        cout << ("Socket failed with error: ", WSAGetLastError());
        system("pause");
    }
    // Configuring a TCP listening socket
    iResult = bind(_listenSocket, (SOCKADDR*)&address, sizeAddress);
    if (iResult == SOCKET_ERROR)
    {
        cout << ("Bind failed with error: ", WSAGetLastError());
        system("pause");
    }
    // Let's set the server to receive data
    iResult = listen(_listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        cout << ("Listen failed with error: ", WSAGetLastError());
        system("pause");
    }

    SOCKET connection;
    while (true)
    {
        // Accept the client socket
        connection = accept(_listenSocket, (SOCKADDR*)&address, &sizeAddress);

        if (connection == INVALID_SOCKET)
        {
            std::cout << ("accept failed with error: ", WSAGetLastError());
            system("pause");
        }
        else
        {
            std::cout << "Client Connected!" << endl;
            log.writeLogFile("Client Connected!");

            _participantSocket.push_back(connection);

            participantHandler(_participantSocket.size() - 1);
            //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)participantHandler,
            //    (LPVOID)(participantSocket.size() - 1), NULL, NULL);
        }
    }
}

void Chat::participantHandler(int i)
{
    int action;
    
    do
    {
        action = getParticipantInt(i);
        if (action == 1)
            // Account Registration
            registration(i);
        else if (action == 2)
        {
            // Log in to your account
            authorization(i);

            do
            {
                action = getParticipantInt(i);
                if (action == 1)
                    // Entering a message
                    enteringMessage(i);
                else if (action == 2)
                    // Reading the message
                    readMessages(i);
                else if (action == 3)
                {

                }
            }
            while (action != 3);

            action = 2;
        }
        else if (action == 3)
        {

        }
    }
    while (action != 3);
}

void Chat::registration(int i)
{
    string login, password, name;
    bool key = true;

    do
    {
        int action;

        recv(_participantSocket[i], (char*)&action, sizeof(int), NULL);
        if (action == 1)
        {
            string keyword = getParticipantString(i);
            
            findFreeLogin(i, keyword);
        }
        login = getParticipantString(i);
        key = compareLogin(login);
        if (key == true)
        {
            sendParticipantInt(i, 1);
            sendParticipantString(i, "A user with this login already exists. Come up with a new login.");
        }
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "Good login.");
        }
    }
    while (key == true);

    password = getParticipantString(i);

    do
    {
        name = getParticipantString(i);
        key = compareName(name);
        if (key == true)
        {
            sendParticipantInt(i, 1);
            sendParticipantString(i, "A user with this username already exists. Come up with a new username.");
        }
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "Good username.");
        }
    }
    while (key == true);

    int id = 0;

    addParticipantToDatabase(id, login, password, name);
}

void Chat::findFreeLogin(int i, string keyword)
{
    string res;

    _loginSource.findPrefixWords(keyword, &res);

    if (res.at(0) == ' ')
    {
        sendParticipantString(i, "No words with matching keyword found ");
    }
    else
    {
        string newLogin, logins;

        logins = "Words with keyword: ";

        for (size_t i = 0; i < res.length(); i++)
        {

            if (res.at(i) == ' ')
            {
                bool key = compareLogin(newLogin);
                if (!key)
                    logins.append(newLogin + " ");

                newLogin.clear();
            }
            else
                newLogin += res.at(i);
        }

        sendParticipantString(i, logins);
    }

    cout << endl;
}

void Chat::addParticipant(int id, string login, string password, string name)
{
    Participants participant;

    participant.setId(id);
    participant.setLogin(login);
    participant.setPassword(password);
    participant.setName(name);

    _participants.push_back(participant);
    log.writeLogFile("Client " + name + " was added to the chat successfully!");
}

void Chat::addParticipantToDatabase(int id, string login, string password, string name)
{
    string str;
    
    str.append("INSERT INTO participants(id, login, password, name) values(default, '")
        .append(login).append("', '").append(password).append("', '").append(name).append("')");
    mysql_query(&mysql, str.c_str());
    
    str.clear();
    
    str.append("SELECT * FROM participants WHERE login = '").append(login).append("'");
    mysql_query(&mysql, str.c_str());

    if (MYSQL_RES* res = mysql_store_result(&mysql))
    {
        MYSQL_ROW row = mysql_fetch_row(res);

        id = atoi(row[0]);
    }

    addParticipant(id, login, password, name);
}

void Chat::authorization(int i)
{
    string login, password;
    bool key = true;

    do
    {
        login = getParticipantString(i);
        password = getParticipantString(i);
        key = signIn(login, password);
        if (key == true)
        {
            sendParticipantInt(i, 1);
            sendParticipantString(i, "You are logged in to your account.");
            log.writeLogFile("The client logged in to the chat. Login: " + login);
        }
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "You have entered an incorrect login or password. Try again.");
        }
    }
    while (key == false);

    sendParticipantString(i, getParticipantName(login));
}

bool Chat::signIn(string login, string password)
{
    uint hash = findHash(password);

    for (size_t i = 0; i < _participants.size(); ++i)
    {
        if (login == _participants[i].getLogin())
        {
            // The first option
            /*if (password == _participants[i].getPassword())
            {
                return true;
            }
            else
            {
                return false;
            }*/

            // The second option
            if (hash == _participants[i].getPasswordHash())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

uint Chat::findHash(string password)
{
    const char* charPassword = password.c_str();
    int length = strlen(charPassword);

    uint* hash = sha1(charPassword, (uint)length);

    return *hash;
}

void Chat::enteringMessage(int i)
{
    string recipientName, nameSender, text;
    bool key = false;

    do
    {
        recipientName = getParticipantString(i);
        key = compareName(recipientName);
        if (key == true)
            sendParticipantInt(i, 1);
        else
        {
            sendParticipantInt(i, 0);
            sendParticipantString(i, "A user with that name was not found. Try again.");
        }
    }
    while (key == false);

    nameSender = getParticipantString(i);
    text = getParticipantString(i);

    sendMessage(nameSender, recipientName, text); 
}

void Chat::sendMessage(string nameSender, string nameRecipient, string text)
{
    int iSender = getParticipantIndex(nameSender);
    int idSender = _participants[iSender].getId();
    int idRecipient;
    string str;

    if (nameRecipient == "all")
    {
        for (size_t i = 0; i < _participants.size(); ++i)
            if (i != iSender)
            {
                _participants[i].recordMessage(_participants[iSender].getName(), text);

                idRecipient = _participants[i].getId();
                
                str.append("INSERT INTO messages(id, id_recipient, id_sender, text) values(default, ")
                    .append(to_string(idRecipient)).append(", ").append(to_string(idSender)).append(", '")
                    .append(text).append("')");
                mysql_query(&mysql, str.c_str());

                str.clear();
            }
    }
    else
    {
        for (size_t i = 0; i < _participants.size(); ++i)
            if (nameRecipient == _participants[i].getName())
            {
                _participants[i].recordMessage(_participants[iSender].getName(), text);

                idRecipient = _participants[i].getId();

                str.append("INSERT INTO messages(id, id_recipient, id_sender, text) values(default, ")
                    .append(to_string(idRecipient)).append(", ").append(to_string(idSender)).append(", '")
                    .append(text).append("')");
                mysql_query(&mysql, str.c_str());

                break;
            }
    }
    log.writeLogFile("The message was sent. Sender: " + nameSender + "; Recipient: " + nameRecipient);
}

void Chat::readMessages(int index)
{
    string recipientName = getParticipantString(index);
    int iRecipient = getParticipantIndex(recipientName);
    auto& recipientParticipant = _participants[iRecipient];
    int messageCount = recipientParticipant.showMessageCount();
    int idRecipient = recipientParticipant.getId();
    string str;

    if (messageCount == 0)
        sendParticipantString(index, "You don't have any unread messages.");
    else if (messageCount == 1)
        sendParticipantString(index, "You have one unread message.");
    else
        sendParticipantString(index, "You have " + to_string(messageCount) + " unread messages.");

    sendParticipantInt(index, messageCount);
    for (size_t i = 0; i < messageCount; ++i)
    {
        sendParticipantString(index, "Sender: " + recipientParticipant.showMessageSender(i));
        sendParticipantString(index, "Text: " + recipientParticipant.showMessageText(i));
    }

    recipientParticipant.clearMessage();

    str.append("DELETE FROM messages WHERE id_recipient = ").append(to_string(idRecipient));
    mysql_query(&mysql, str.c_str());
}

bool Chat::compareLogin(string login)
{
    for (size_t i = 0; i < _participants.size(); ++i)
    {
        if (login == _participants[i].getLogin())
            return true;
    }

    return false;
}

bool Chat::compareName(string name)
{
    if (name == "all")
        return true;

    for (size_t i = 0; i < _participants.size(); ++i)
    {
        if (name == _participants[i].getName())
            return true;
    }

    return false;
}

string Chat::getParticipantName(string login)
{
    for (size_t i = 0; i < _participants.size(); ++i)
        if (login == _participants[i].getLogin())
            return _participants[i].getName();
}

int Chat::getParticipantIndex(string name)
{
    for (size_t i = 0; i < _participants.size(); ++i)
        if (name == _participants[i].getName())
            return i;
}

int Chat::getParticipantInt(int i)
{
    int j;
    
    recv(_participantSocket[i], (char*)&j, sizeof(int), NULL);

    return j;
}

string Chat::getParticipantString(int i)
{
    int sizeWord;

    recv(_participantSocket[i], (char*)&sizeWord, sizeof(int), NULL);

    char* word = new char[sizeWord + 1];
    word[sizeWord] = '\0';

    recv(_participantSocket[i], word, sizeWord, NULL);

    return word;
}

void Chat::sendParticipantInt(int i, int j)
{
    send(_participantSocket[i], (char*)&j, sizeof(int), NULL);
}

void Chat::sendParticipantString(int i, string word)
{
    int sizeWord = word.size();

    send(_participantSocket[i], (char*)&sizeWord, sizeof(int), NULL);
    send(_participantSocket[i], word.c_str(), sizeWord, NULL);
}