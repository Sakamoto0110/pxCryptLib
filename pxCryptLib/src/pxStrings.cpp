#include "pch.h"
#include "../include/pxStrings.h"


char* _cpystr(char* dst, const char* src, int count) {

	for (int i = 0; i < count; i++) {
		dst[i] = src[i];
	}
	return dst;
}








//
//pxCrypt_API struct StringBlock CreateStringBlockHandle(size_t szBlock) {
//	StringBlock handle;
//	handle.cOffset = 1;
//	handle.szBlock = szBlock;
//	handle.pcBlock = new StringCell[handle.szBlock];
//	for (size_t i = 0; i < handle.szBlock; i++)
//		handle.pcBlock[i].value = null_char;
//	//handle.pcBlock[handle.szBlock].value = '\0';
//	return handle;
//}
//
//pxCrypt_API struct StringBlock CreateStringBlockHandle(size_t szBlock, const char* content) {
//	StringBlock handle;
//	handle.cOffset = 1;
//	handle.szBlock = szBlock;
//	handle.pcBlock = new StringCell[handle.szBlock + 1];
//	for (size_t i = 0; i < handle.szBlock; i++)
//		handle.pcBlock[i].value = i < strlen(content) ? content[i] : null_char;
//	handle.pcBlock[handle.szBlock].value = '\0';
//	return handle;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//pxCrypt_API void _swap(struct StringCell* buff, size_t a, size_t b) {
//	StringCell t = buff[a];
//	buff[a] = buff[b];
//	buff[b] = t;
//}
//
//pxCrypt_API size_t _index(size_t i, size_t offset) {
//	return i * offset;
//}
//
//pxCrypt_API void _rshift(struct StringBlock* handle, int shift_ammount) {
//	StringCell*  buff   = handle->pcBlock;
//	size_t szBuff = handle->szBlock;
//	size_t offset = handle->cOffset;
//	for (size_t i = szBuff - 1; i > 0; i--) {
//		size_t a = _index(i,offset);
//		size_t b = _index((i + shift_ammount) >= szBuff ? 0 : i + shift_ammount, offset);
//		//printf("[%llu] swaping a[%llu:%c] with b[%llu:%c]\n", i, a, buff[a], b, buff[b]);
//		//		size_t b = _index((i + value) % szBlock);
//		_swap(buff,a, b);
//	}
//}
//
//pxCrypt_API void _lshift(struct StringBlock* handle, int shift_ammount) {
//	StringCell*  buff   = handle->pcBlock;
//	size_t szBuff = handle->szBlock;
//	size_t offset = handle->cOffset;
//	for (size_t i = 0; i < szBuff; i++) {
//		size_t a = _index(i, offset);
//		size_t b = _index((i + shift_ammount) >= szBuff ? 0 : i + shift_ammount, offset);
//		//printf("[%llu] swaping a[%llu:%c] with b[%llu:%c]\n", i, a, buff[a], b, buff[b]);
//		//		size_t b = _index((i + value) % szBlock);
//		_swap(buff, a, b);
//	}
//}

pxCrypt_API void Print2D(size_t nRows, size_t nCols, char* str) {
	for (size_t y = 0; y < nRows; y++) {
		for (size_t x = 0; x < nCols; x++) {

			size_t index = x + y * nCols;
			printf("%c", str[index]);
		}
		printf("\n");
	}
}
