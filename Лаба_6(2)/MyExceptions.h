#include "includes.h"

/*
Instruction:

*/

class MyException
{
public:
	MyException(const char* msg, int value);
	MyException(const char* msg, string str, int value);
	int code();
	string what();

private:
	int _dataState;
	string _message;

	void GetString(const char* msg); // переводим char* в string
};