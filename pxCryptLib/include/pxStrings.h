#pragma once

#define pxCryptAPI_EXPORTS
#include "pxCrypt_core.h"
#include <string>
using Handle = void*;
static constexpr char null_char = (char)0xB0;


pxCrypt_API char* _cpystr(char* dst, const char* src, int count);

//pxCrypt_API struct StringBlock* CreateStrBlockHandle(const char* str, size_t szstr);
//


//pxCrypt_API void _swap(struct StringCell* buff, size_t a, size_t b);
//pxCrypt_API void _rshift(struct StringBlock*, int shift_ammount);
//pxCrypt_API void _lshift(struct StringBlock*, int shift_ammount);
//
//pxCrypt_API struct StringBlock CreateStringBlockHandle(size_t szBlock);
//pxCrypt_API struct StringBlock CreateStringBlockHandle(size_t szBlock,const char* content);
//

pxCrypt_API void Print2D(size_t nRows, size_t nCols, char* str);

using byte = unsigned char;   // 1 bytes 




constexpr int32_t Color_RGBA(byte red, byte green, byte blue, byte alpha) {
	return ((red & 0x0ff) << 24) | ((green & 0x0ff) << 16) | ((blue & 0x0ff) << 8) | ((alpha & 0x0ff) << 0);
}

// Alpha will be 255 by default
constexpr int32_t Color_RGB(byte red, byte green, byte blue) {
	
	return (((int32_t)red & 0x0ff) << 24) | (((int32_t)green & 0x0ff) << 16) | (((int32_t)blue & 0x0ff) << 8) | ((255 & 0x0ff) << 0);
}

constexpr byte GetRed(int32_t color) { return (color & 0xFF000000) >> 24; }
constexpr byte GetGreen(int32_t color) { return (color & 0x00FF0000) >> 16; }
constexpr byte GetBlue(int32_t color) { return (color & 0x0000FF00) >> 8; }
constexpr byte GetAlpha(int32_t color) { return (color & 0x000000FF) >> 0; }

//
//
//struct StringCell {
//	char value = 0;
//	int32_t color = 0;
//
//	StringCell() {
//
//	}
//	explicit StringCell(char ch) {
//		value = ch;
//	}
//
//	
//
//	StringCell(const StringCell& o) {
//		value = o.value;
//		color = o.color;
//	}
//};
//
//#include <tuple>
//struct StringBlock {
//	//char*  pcBlock;
//	StringCell* pcBlock;
//	size_t szBlock;
//	size_t cOffset;
//	
//};
//
//
//
//struct StringBlockHandler {
//	StringBlock  handle;
//	StringBlock* hRows;
//	StringBlock* hCols;
//	size_t       szBlock;
//	size_t       nRows = 1;
//	size_t       nCols =0;
//
//	StringBlockHandler(size_t _szBlock) {
//		szBlock = _szBlock;
//		nCols = handle.szBlock;
//		_alloc();
//	}
//	StringBlockHandler(size_t _width, size_t _height) {
//		szBlock = _width * _height;
//		nCols = _width;
//		nRows = _height;
//		_alloc();
//		
//	}
//
//	void SetContent(const char* str) {
//		for (size_t i = 0; i < strlen(str); i++)
//			handle.pcBlock[i].value = str[i];
//	}
//
//	void _alloc() {
//		handle = CreateStringBlockHandle(szBlock);
//		hRows = new StringBlock[nRows];
//		for (size_t i = 0; i < nRows; i++) {
//			hRows[i] = { &handle.pcBlock[i * nCols],nCols,1 };
//		}
//		hCols = new StringBlock[nCols];
//		for (size_t i = 0; i < nCols; i++) {
//			hCols[i] = { &handle.pcBlock[i], nCols, nRows };
//		}
//	}
//
//	/*StringBlockHandler operator[](size_t i) {
//		if (nRows == 1 && nCols == 1)
//			return *this;
//		size_t c = nCols;
//		if (nRows == 1) {
//			c = 1;
//		}
//		return StringBlockHandler(c, 1, hRows[i].pcBlock);
//	}
//
//
//
//	char GetChar(size_t x, size_t y) {
//		return handle.pcBlock[x + y * nCols];
//	}*/
//
//	void Print2D() {
//		for (size_t y = 0; y < nRows; y++) {
//			for (size_t x = 0; x < nCols; x++) {
//			
//				size_t index = x + y * nCols;
//				byte r = GetRed(handle.pcBlock[index].color);
//				byte g = GetGreen(handle.pcBlock[index].color);
//				byte b = GetBlue(handle.pcBlock[index].color);
//				printf("\x1b[48;2;%i;%i;%im", r, g,b);
//				printf("%c",  handle.pcBlock[index].value);
//			}
//			printf("\n");
//		}
//	}
//
//	operator StringBlock () const { return handle; }
//	operator const char* () const { 
//		char* r = new char[szBlock + 1];
//		for (size_t i = 0; i < szBlock; i++) {
//			r[i] = handle.pcBlock[i * handle.cOffset].value;
//		}
//		r[szBlock] = '\0';
//		
//		return r; 
//	}
//	static constexpr char null_char = (char)0xB0;
//};
//
//
//	
//	













