#pragma once

#define pxCryptAPI_EXPORTS
#include "include/pxCrypt_core.h"

#include <stdio.h>
namespace pxConsoleHelper {
	pxCryptAPI_EXPORTS bool EnableVTMode();


	template<typename...Tx>
	void _printf(int x, int y, const char* str, Tx...args) {
		printf("\x1b[%i;%iH", x, y);
		printf(str, args...);
	}

}