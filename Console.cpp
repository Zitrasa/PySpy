#include "Console.h"

Console::Console() {
	FILE* stream;
	
	AllocConsole();

	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);
}

Console::~Console() {
	FreeConsole();
}