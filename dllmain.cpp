#include <windows.h>
#include <thread>
#include <iostream>

#include "Console.h"
#include "Collector.h"
#include "Logger.h"

DWORD WINAPI browser(LPVOID params) {
	std::string searchStr;
	std::string format;

	Console console;
	Logger logger(std::cout);

	logger.log("OK", "Console created");

	if (Py::init()) {
		logger.log("OK", "Python27.dll linked");
	} else {
		logger.log("ERR", "Python27.dll link fault");
		return 0;
	}

	while (true) {

		std::cin >> searchStr;

		if (searchStr == "exit") {
			break;
		}

		Collector collector;
		logger.log("OK", "Collector created");

		void* dump = collector.dump();

		if (dump) {
			logger.log("OK", "GC objects dumped");
		} else {
			logger.log("ERR", "GC dump failed");
			return 0;
		}

		std::vector<void*> filtered = collector.filter(dump);
		logger.log("INFO", "Tracked dictionaries count: " + std::to_string(filtered.size()));

		std::vector<std::pair<void*, std::string>> results = collector.search(filtered, searchStr.c_str());
		logger.logSection("INFO", "SEARCH " + searchStr);
		logger.log("INFO", "Dictionaries with key <" + searchStr + ">: " + std::to_string(results.size()));

		for (int i = 0; i < results.size(); i++) {
			std::cout << std::hex << results[i].first << " | " << results[i].second << "\n";
		}

		logger.empty();
		logger.empty();
	}
	
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH: {
			CreateThread(NULL, 0, browser, NULL, 0, NULL);
			break;
		}

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;

		case DLL_PROCESS_DETACH:
			break;
    }
	
    return TRUE;
}

