#include <iostream>
#include "Chat.h"

using namespace std;

#if defined(_WIN32) 
#define PLATFORM_NAME "windows 32-bit" // Windows 32-bit
#elif defined(_WIN64)
#define PLATFORM_NAME "windows 64-bit" // Windows 64-bit
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android"   // Android 
#elif defined(__linux__)
#define PLATFORM_NAME "linux"     // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos и другие
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios"       // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx"       // Apple OSX
#else
#define PLATFORM_NAME  "OS not detected" // ОС не распознана
#endif

int main()
{
    cout << "Welcome to the chat!" << endl;

    Chat chat;

    // Establish a connection to the server
    if (PLATFORM_NAME == "windows 32-bit" || PLATFORM_NAME == "windows 64-bit")
        chat.connectServer();

    int action;
    do
    {
        cout << "Select an action (1 - register an account; 2 - log in to your account; 3 - exit the application): ";
        cin >> action;
        chat.sendServerInt(action);

        if (action == 1)
        {
            // Account Registration
            chat.registration();
        }
        else if (action == 2)
        {
            // Log in to your account
            chat.authorization();

            do
            {
                cout << "Select an action (1 - write a message; 2 - read the sent messages; 3 - log out of your account): ";
                cin >> action;
                chat.sendServerInt(action);

                if (action == 1)
                {
                    // Entering a message
                    chat.enteringMessage();
                }
                else if (action == 2)
                {
                    // Reading sent messages
                    chat.readMessages();
                }
                else if (action == 3)
                {
                }
                else
                {
                    cout << "Invalid operator, repeat the input" << endl;
                }
            }
            while (action != 3);

            action = 2;
        }
        else if (action == 3)
        {
        }
        else
        {
            cout << "Invalid operator, repeat the input" << endl;
        }
    }
    while (action != 3);

    return 0;
}