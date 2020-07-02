#include "Collector.h"
using namespace std::chrono_literals;

Collector::Collector() {
	GILState state = Py::GIL::Ensure();

	void* gcModuleName = Py::String::FromString("gc");
	gcModule = Py::Import::Import(gcModuleName);

	if (!gcModule) {
		throw std::runtime_error("[ERR]: Failed to load GC Module.");
	}

	gcGetObjects = Py::Object::GetAttrString(gcModule, "get_objects");

	if (!gcGetObjects) {
		throw std::runtime_error("[ERR]: Failed to load gc.get_objects() function.");
	}
	
	gcReferrers = Py::Object::GetAttrString(gcModule, "get_referrers");
	
	if (!gcReferrers) {
		throw std::runtime_error("[ERR]: Failed to load gc.get_referrers() function.");
	}

	Py::DecRef(gcModuleName);
	Py::GIL::Release(state);
}

Collector::~Collector() {
	GILState state = Py::GIL::Ensure();
	Py::DecRef(gcModule);
	Py::DecRef(gcGetObjects);
	Py::DecRef(gcReferrers);
	Py::GIL::Release(state);
}

DWORD WINAPI Collector::dumpWorker(LPVOID params) {
	HANDLE curThreadHandle = GetCurrentThread();
	SetThreadPriority(curThreadHandle, THREAD_PRIORITY_LOWEST);

	Collector* This = (Collector*)params;

	GILState state = Py::GIL::Ensure();
	This->dumpList = Py::Object::CallObject(This->gcGetObjects, NULL);
	Py::GIL::Release(state);

	return 0;
}

void* Collector::dump() {
	if (!gcModule || !gcGetObjects || !gcReferrers) {
		throw std::runtime_error("[ERR]: Collector is not correctly initiated to collect().");
	}
	
	HANDLE worker = CreateThread(NULL, 0, dumpWorker, (void*)this, CREATE_SUSPENDED, NULL);

	while (worker && WaitForSingleObject(worker, 0) == WAIT_TIMEOUT) {
		ResumeThread(worker);
		std::this_thread::sleep_for(0.025s);
		SuspendThread(worker);
		std::this_thread::yield();
	}

	if (worker) {
		CloseHandle(worker);
	}

	return dumpList;
}

std::vector<void*> Collector::filter(void* dump) {
	GILState state = Py::GIL::Ensure();

	void* item;
	std::vector<void*> filtered;
	int len = Py::List::Size(dump);
	void* dict = Py::Dict::New();
	void* type = Py::Object::Type(dict);

	for (int i = 0; i < len; i++) {
		item = Py::List::GetItem(dump, i);

		if (Py::Object::IsInstance(item, type) == 1) {
			filtered.push_back(item);
		}
	}

	Py::DecRef(dict);
	Py::DecRef(type);

	Py::GIL::Release(state);

	return filtered;
}

std::vector<std::pair<void*, std::string>> Collector::search(std::vector<void*>& dicts, const char* key) {
	std::vector<std::pair<void*, std::string>> results;
	
	GILState state = Py::GIL::Ensure();
	for (int i = 0; i < dicts.size(); i++) {
		void* item = Py::Dict::GetItemString(dicts[i], key);

		if (item) {
			results.push_back({ dicts[i], 
				std::string(Py::String::AsString(Py::Object::Str(item))) });
		}
	}

	Py::GIL::Release(state);
	return results;
}