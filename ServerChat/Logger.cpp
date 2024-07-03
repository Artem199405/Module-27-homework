#include "Logger.h"

Logger::Logger()
{
    log_file = fstream("log.txt", ios::in | ios::out);

    filesystem::permissions("log.txt",
        filesystem::perms::group_all | filesystem::perms::others_all,
        filesystem::perm_options::remove);

    if (!log_file)
        log_file = fstream("log.txt", ios::in | ios::out | ios::trunc);
}

Logger::~Logger()
{
    log_file.close();
}

void Logger::writeLogFile(string text)
{
    log_shared_mutex.lock();
    
    if (log_file)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(now);
        //std::cout << "Current Time and Date: " << ctime(&end_time) << std::endl;

        log_file.seekp(0, ios_base::end);
        log_file << text << endl;
    }
    else
    {
        cout << "Could not open file log.txt !" << endl;
    }

    log_shared_mutex.unlock();
}

void Logger::readLogFile()
{
    log_shared_mutex.lock();
    
    if (log_file)
    {
        log_file.seekp(0, ios_base::beg);
        while (!log_file.eof())
        {
            string text;

            getline(log_file, text);
            cout << text << endl;
        }
    }

    log_shared_mutex.unlock();
}