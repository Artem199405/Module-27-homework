#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <shared_mutex>

using namespace std;

class Logger
{
public:
	/**
	 Constructor
	*/
	Logger();

	/**
	 Destructor
	*/
	~Logger();

	/**
	 * Writing logs to the file
	 *
	 * @param text
	*/
	void writeLogFile(string text);

	/**
	* Reading log file
	*/
	void readLogFile();

private:
	fstream log_file;

	shared_mutex log_shared_mutex;
};