#pragma once

#define pxCryptAPI_EXPORTS
#include "pxCrypt_core.h"
#include <string>
using Handle = void*;
static constexpr char null_char = (char)0xB0;


pxCrypt_API char* _cpystr(char* dst, const char* src, int count);

//pxCrypt_API struct StringBlock* CreateStrBlockHandle(const char* str, size_t szstr);
//


pxCrypt_API void _swap(char* buff, size_t a, size_t b);
pxCrypt_API void _rshift(struct StringBlock*, int shift_ammount);
pxCrypt_API void _lshift(struct StringBlock*, int shift_ammount);

pxCrypt_API struct StringBlock CreateStringBlockHandle(size_t szBlock);
pxCrypt_API struct StringBlock CreateStringBlockHandle(size_t szBlock,const char* content);


pxCrypt_API void Print2D(size_t nRows, size_t nCols, char* str);

struct StringBlock {
	char*  pcBlock;
	size_t szBlock;
	size_t cOffset;
	
};



struct StringBlockHandler {
	StringBlock  handle;
	StringBlock* hRows;
	StringBlock* hCols;
	size_t       szBlock;
	size_t       nRows = 1;
	size_t       nCols =0;

	StringBlockHandler(size_t _szBlock) {
		szBlock = _szBlock;
		handle = CreateStringBlockHandle(_szBlock);
		nCols = handle.szBlock;
		hRows = new StringBlock[nRows];
		hCols = new StringBlock[nCols];
	}
	StringBlockHandler(size_t _szBlock, const char* content) {
		szBlock = _szBlock;
		handle = CreateStringBlockHandle(szBlock,content);
		nCols = handle.szBlock;
		hRows = new StringBlock[nRows];
		hCols = new StringBlock[nCols];
	}
	StringBlockHandler(size_t _width, size_t _height, const char* content) {
		szBlock = _width * _height;
		handle = CreateStringBlockHandle(_width * _height, content);
		nCols = _width;
		nRows = _height;
		
		hRows = new StringBlock[nRows];
		for (size_t i = 0; i < nRows; i++) {
			hRows[i] = { &handle.pcBlock[i * nCols],nCols,1 };
		}
		hCols = new StringBlock[nCols];
		for (size_t i = 0; i < nCols; i++) {
			hCols[i] = { &handle.pcBlock[i], nCols, nRows };
		}
		
	}

	StringBlockHandler operator[](size_t i) {
		if (nRows == 1 && nCols == 1)
			return *this;
		size_t c = nCols;
		if (nRows == 1) {
			c = 1;
		}
		return StringBlockHandler(c, 1, hRows[i].pcBlock);
	}



	char GetChar(size_t x, size_t y) {
		return handle.pcBlock[x + y * nCols];
	}

	void Print2D() {
		for (size_t y = 0; y < nRows; y++) {
			for (size_t x = 0; x < nCols; x++) {
			
				size_t index = x + y * nCols;
				printf("%c",handle.pcBlock[index]);
			}
			printf("\n");
		}
	}

	operator StringBlock () const { return handle; }
	operator const char* () const { 
		char* r = new char[szBlock + 1];
		for (size_t i = 0; i < szBlock; i++) {
			r[i] = handle.pcBlock[i * handle.cOffset];
		}
		r[szBlock] = '\0';
		
		return handle.pcBlock; 
	}
	static constexpr char null_char = (char)0xB0;
};


	
	













