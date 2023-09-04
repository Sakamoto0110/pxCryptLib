#include "pch.h"
#include "../include/pxStrings.h"



char* _cpystr(char* dst, const char* src, int count) {

	for (int i = 0; i < count; i++) {
		dst[i] = src[i];
	}
	return dst;
}


StringBlock* CreateStrBlockHandle(const char* str, size_t szstr) {
	StringBlock* (*a[6])(const char*)
	{
		[](const char* str)->HSB {return(HSB) new sb4(str);  },
			[](const char* str)->HSB {return(HSB) new sb8(str);  },
			[](const char* str)->HSB {return(HSB) new sb16(str); },
			[](const char* str)->HSB {return(HSB) new sb32(str); },
			[](const char* str)->HSB {return(HSB) new sb64(str); },
			[](const char* str)->HSB {return(HSB) new sb128(str); },
	};
	return a[szstr / 4 - 1](str);
}