#pragma once

#define pxCryptAPI_EXPORTS
#include "include/pxCrypt_core.h"

#include <stdio.h>
#include <ostream>

namespace vtsc {
#define ESC "\x1b"
#define CSI "\x1b["
	static const char* svcur = ESC"7";
	static const char* ldcur = ESC"8";

	// Cursor move
	static constexpr const char* mvcur_up = CSI"%iA";
	static constexpr const char* mvcur_down  = CSI"%iB"; 
	static constexpr const char* mvcur_right = CSI"%iC";
	static constexpr const char* mvcur_left  = CSI"%iD";
	static constexpr const char* mvcur_ldown = CSI"%iE";
	static constexpr const char* mvcur_lup   = CSI"%iF";
	static constexpr const char* mvcur_habs  = CSI"%iG";
	static constexpr const char* mvcur_vabs  = CSI"%id";
	static constexpr const char* mvcur       = CSI"%i;%iH";
}

namespace pxConsoleHelper {
	pxCryptAPI_EXPORTS bool EnableVTMode();

	


	template<typename...Tx>
	void _printf(int x, int y, const char* str, Tx...args) {
		printf("\x1b[%i;%iH", x, y);
		printf(str, args...);
	}

}