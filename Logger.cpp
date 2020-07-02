#include "Logger.h"

__time64_t Logger::getTime() {
	return std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::now());
}

Logger::Logger(std::ostream& outStream) {
	out = &outStream;
}

void Logger::log(std::string reason, std::string msg) {
	auto timenow = getTime();

	*out << ctime(&timenow) << " [" << reason << "]: " << msg << "\n";
}

void Logger::logSection(std::string reason, std::string header) {
	auto timenow = getTime();

	*out << "---------------------------------------------------\n";
	*out << ctime(&timenow) << " [" << reason << "]: " << header << "\n";
	*out << "---------------------------------------------------\n";
}

void Logger::logHex(std::string reason, __int64 value) {
	auto timenow = getTime();
	std::stringstream sstream;
	
	sstream << std::hex << value;
	*out << ctime(&timenow) << " [" << reason << "]: " << sstream.str() << "\n";
}

void Logger::empty() {
	*out << "\n";
}