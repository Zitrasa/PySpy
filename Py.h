#pragma once

#include <windows.h>
#include <optional>
#include <string>

#include "PyTypes.h"

namespace Py {

	typedef void (*REFProc)(void* obj);

	extern REFProc IncRef;
	extern REFProc DecRef;

	namespace String {
		typedef void* (*FromStringProc)(const char* str);
		typedef char* (*AsStringProc)(void* str);

		extern FromStringProc FromString;
		extern AsStringProc AsString;
	}

	namespace Import {
		typedef void* (*ImportProc)(void* name);

		extern ImportProc Import;
	}

	namespace Object {
		typedef void* (*GetAttrStringProc)(void* obj, const char* name);
		typedef void* (*CallObjectProc)(void* obj, void* args);
		typedef void* (*TypeProc)(void* obj);
		typedef int (*IsInstanceProc)(void* obj, void* type);
		typedef int (*TypeCheckProc)(void* obj, void* type);
		typedef void* (*StrProc)(void* obj);

		extern GetAttrStringProc GetAttrString;
		extern CallObjectProc CallObject;
		extern TypeProc Type;
		extern IsInstanceProc IsInstance;
		extern TypeCheckProc TypeCheck;
		extern StrProc Str;
	}

	namespace GIL {
		typedef GILState (*EnsureProc)();
		typedef void (*ReleaseProc)(GILState state);

		extern EnsureProc Ensure;
		extern ReleaseProc Release;
	}

	namespace Tuple {
		typedef void* (*NewProc)(int len);
		typedef int (*SetItemProc)(void* tuple, int pos, void* obj);

		extern NewProc New;
		extern SetItemProc SetItem;
	}

	namespace Dict {
		typedef int (*CheckProc)(void* obj);
		typedef void* (*GetItemStringProc)(void* dict, const char* key);
		typedef void* (*NewProc)();

		extern CheckProc Check;
		extern GetItemStringProc GetItemString;
		extern NewProc New;
	}

	namespace List {
		typedef int (*SizeProc)(void* list);
		typedef void* (*GetItemProc)(void* list, int index);

		extern SizeProc Size;
		extern GetItemProc GetItem;
	}

	bool init();
}