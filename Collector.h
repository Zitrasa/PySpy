#pragma once

#include <stdexcept>
#include <thread>
#include <chrono>
#include <vector>

#include "Py.h"

class Collector {
protected:

	static DWORD WINAPI dumpWorker(LPVOID params);

	void* gcModule = nullptr;
	void* gcGetObjects = nullptr;
	void* gcReferrers = nullptr;

	void* dumpList = nullptr;

public:

	Collector();
	~Collector();

	void* dump();
	std::vector<void*> filter(void* dump);
	std::vector<std::pair<void*, std::string>> search(std::vector<void*>& dicts, const char* key);
};