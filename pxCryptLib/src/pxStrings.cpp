#include <pch.h>
#include "../include/pxStrings.h"



char* _cpystr(char* dst, const char* src, int count) {

	for (int i = 0; i < count; i++) {
		dst[i] = src[i];
	}
	return dst;
}

