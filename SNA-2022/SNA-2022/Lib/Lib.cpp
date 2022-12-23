// Lib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include <iostream>

extern "C"
{

	int __stdcall strln(char* source)
	{
		int result = 0;
		while (source[result] != '\0')
		{
			result++;
		}
		return result;
	}

	int __stdcall strtonum(char* source)
	{
		int n = 0;
		while (*source >= '0' && *source <= '9') {
			n *= 10;
			n += *source++;
			n -= '0';
		}
		return n;
	}

	int __stdcall readw(char* value)
	{
		setlocale(0, "");
		std::cout << value << std::endl;
		return 0;
	}

	int __stdcall readr(int value)
	{
		std::cout << value << std::endl;
		return 0;
	}
}