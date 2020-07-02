#include "Py.h"
#include <iostream>

namespace Py {

	namespace {

		HANDLE pyModuleHandle;

		template<typename F>
		F GetPyFunc(const char* name) {
			return (F)GetProcAddress(
				(HMODULE)pyModuleHandle,
				name);
		}
	}

	REFProc IncRef;
	REFProc DecRef;

	namespace String {
		FromStringProc FromString;
		AsStringProc AsString;
	}

	namespace Import {
		ImportProc Import;
	}

	namespace Object {
		GetAttrStringProc GetAttrString;
		CallObjectProc CallObject;
		TypeProc Type;
		IsInstanceProc IsInstance;
		TypeCheckProc TypeCheck;
		StrProc Str;
	}

	namespace GIL {
		EnsureProc Ensure;
		ReleaseProc Release;
	}

	namespace Tuple {
		NewProc New;
		SetItemProc SetItem;
	}

	namespace Dict {
		CheckProc Check;
		GetItemStringProc GetItemString;
		NewProc New;
	}

	namespace List {
		SizeProc Size;
		GetItemProc GetItem;
	}

	bool init() {
		
		if (!(pyModuleHandle = GetModuleHandleA("python27.dll"))) {
			return false;
		}

		IncRef =
			GetPyFunc<REFProc>("Py_IncRef");
		DecRef =
			GetPyFunc<REFProc>("Py_DecRef");
		String::FromString =
			GetPyFunc<String::FromStringProc>("PyString_FromString");	
		String::AsString =
			GetPyFunc<String::AsStringProc>("PyString_AsString");
		Import::Import =
			GetPyFunc<Import::ImportProc>("PyImport_Import");
		Tuple::New =
			GetPyFunc<Tuple::NewProc>("PyTuple_New");
		Tuple::SetItem =
			GetPyFunc<Tuple::SetItemProc>("PyTuple_SetItem");
		Object::GetAttrString =
			GetPyFunc<Object::GetAttrStringProc>("PyObject_GetAttrString");
		Object::Type =
			GetPyFunc<Object::TypeProc>("PyObject_Type");
		Object::CallObject =
			GetPyFunc<Object::CallObjectProc>("PyObject_CallObject");
		Object::IsInstance =
			GetPyFunc<Object::IsInstanceProc>("PyObject_IsInstance");
		Object::TypeCheck =
			GetPyFunc<Object::TypeCheckProc>("PyObject_TypeCheck");
		Object::Str =
			GetPyFunc<Object::StrProc>("PyObject_Str");
		GIL::Ensure =
			GetPyFunc<GIL::EnsureProc>("PyGILState_Ensure");
		GIL::Release =
			GetPyFunc<GIL::ReleaseProc>("PyGILState_Release");
		Dict::Check =
			GetPyFunc<Dict::CheckProc>("PyDict_Check");		
		Dict::GetItemString =
			GetPyFunc<Dict::GetItemStringProc>("PyDict_GetItemString");
		Dict::New =
			GetPyFunc<Dict::NewProc>("PyDict_New");
		List::Size = 
			GetPyFunc<List::SizeProc>("PyList_Size");
		List::GetItem = 
			GetPyFunc<List::GetItemProc>("PyList_GetItem");

		return true;
	}
}
