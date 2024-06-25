#include "pch.h"
#include "Log.h"


#include <Windows.h>

namespace Log 
{
	void Error(const std::string& s)
	{
		std::string err = "[ERROR]\t: ";
		err += s + "\n";

		OutputDebugStringA(err.c_str());		// vs

		cout << err;						// console
	}

	void Warning(const std::string& s)
	{
		std::string warn = "[WARNING]\t: ";
		warn += s + "\n";

		OutputDebugStringA(warn.c_str());

		cout << warn;
	}

	void Info(const std::string& s)
	{
		std::string info = "[INFO]\t: " + s + "\n";
		OutputDebugStringA(info.c_str());


		cout << info;
	}
}

