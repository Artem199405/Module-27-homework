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
    cout << "OS name: " << PLATFORM_NAME << endl;

    cout << "Welcome to the chat server!" << endl;

    Chat chat;

    // Establishing a connection to the database
    chat.connectToDatabase();

    // Restoring the chat
    chat.restoringChat();

    // Starting the server
    if (PLATFORM_NAME == "windows 32-bit" || PLATFORM_NAME == "windows 64-bit")
        chat.runServer();

    return 0;
}