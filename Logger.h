#pragma once
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>

class Logger {
private:

	std::ostream* out;
	__time64_t getTime();

public:

	Logger(std::ostream& outStream);

	void log(std::string reason, std::string msg);
	void logSection(std::string reason, std::string msg);
	void logHex(std::string reason, __int64 value);
	void empty();
};

