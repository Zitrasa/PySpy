# PySpy
DLL, intended for injection in processes using Python 2.7 C-Api.

## Main goal
This DLL can be used to find Python dictionaries with given keyword (one-word string) in process, that uses Python 2.7 C-Api.

## Principle of operation
1. Allocating new console and redirecting stdin\out\err to it, so you can use iostream freely.
2. Using Winapi, locating **python27.dll**, getting it's handle and handle to functions, listed in **Py.h** file. When I was writing a code, I included only the features I need (mainly some Object, List, Dict methods), but you can easily add methods you need, following the pattern used by me (**Py.h\Py.cpp**).
3. Calling **gc.get_objects( )** method, getting all gc tracked items.
4. With Python classinfo functions, extract all dictionaries out of gc tracked items.
5. Request your keyword to find, and output list of results.

## Output
After inputing your keyword, this code write a list of results, which consist of several lines of following structure:
  1. Dictionary adress in process memory.
  2. Output of python **str( )** function, that was used on a value, assigned for requested key.
