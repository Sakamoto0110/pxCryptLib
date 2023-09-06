#pragma once


#include "pxCrypt_core.h"
#include <string>
using Handle = void*;
static constexpr char null_char = (char)0xB0;


pxCrypt_API char* _cpystr(char* dst, const char* src, int count);

pxCrypt_API struct StringBlock* CreateStrBlockHandle(const char* str, size_t szstr);


typedef struct STRINGBLOCKHANDLE_t {
	pxWORD      wParam   : 2;
	pxWORD      wParamEx : 2;
	pxINT32     x        : 4;
	pxINT32     y        : 4;
}pxStringBlockHandle;

typedef struct  STRINGBLOCK_t {
	pxDWORD     dwFlags ;
	pxINT32		szBlock ;
	pxINT32		szWidth ;
	pxINT32		szHeight;
	//char        _________[8];
	pxPCHAR     pBuffer;	
}pxStringBlock;

using pxHSB = pxStringBlock*;

pxHSB CreateStringBlock(pxINT32 sz) {
	pxStringBlock* hsb = new pxStringBlock();
	hsb->dwFlags = 0;
	hsb->szBlock = sz;
	hsb->szWidth = (pxINT32)sqrt(sz);
	hsb->szHeight = hsb->szWidth;
	hsb->pBuffer = new char[sz + 1];
	for (pxINT32 i = 0; i < hsb->szBlock; i++)
		hsb->pBuffer[i] = null_char;
	hsb->pBuffer[hsb->szBlock] = '\0';
	return hsb;
}

pxHSB CreateStringBlock(pxINT32 width, pxINT32 height) {
	pxStringBlock* hsb = new pxStringBlock();
	hsb->dwFlags = 0;
	hsb->szBlock = width*height;
	hsb->szWidth = width;
	hsb->szHeight = height;
	hsb->pBuffer = new char[hsb->szBlock + 1];
	for (pxINT32 i = 0; i < hsb->szBlock; i++)
		hsb->pBuffer[i] = null_char;
	hsb->pBuffer[hsb->szBlock] = '\0';
	return hsb;
}
pxHSB CreateStringBlock(pxINT32 width, pxINT32 height, pxPSTR str) {
	pxStringBlock* hsb = new pxStringBlock();
	hsb->dwFlags = 0;
	hsb->szBlock = width * height;
	hsb->szWidth = width;
	hsb->szHeight = height;
	hsb->pBuffer = new char[hsb->szBlock + 1];
	for (pxINT32 i = 0; i < hsb->szBlock; i++)
		hsb->pBuffer[i] = i<strlen(str)?str[i]:null_char;
	hsb->pBuffer[hsb->szBlock] = '\0';
	return hsb;
}

typedef struct StringBlock {
	pxDWORD				dwFlags;
	size_t				szBlock;
	char*				strBlock;
}*HSB;



struct BlockHandle {
	char* _ptr;
	size_t _count;
	size_t _offset;

	void _swap(size_t _a, size_t _b) {
		char t = _ptr[_a];
		_ptr[_a] = _ptr[_b];
		_ptr[_b] = t;
	};

	void _shift(int value) {

		auto rs = [&](auto a, auto b) {
			if (b > _count)
				b -= _count;
			_swap(a, b);
		};
		auto ls = [&](auto a, auto b) {
			if (b < 0)
				b += _count;
			_swap(a, b);
		};
		if (value > 0) {
			do {
				for (size_t i = 0; i < _count - 1; i++)
					rs(_index(i), _index(i + 1));
			} while (--value != 0);
		}
		else if (value < 0) {
			do {
				for (size_t i = _count - 1; i > 0; i--)
					ls(_index(i), _index(i - 1));
			} while (++value != 0);
		}

	}

	size_t _index(size_t i) const {
		return i * _offset;
	}

	char operator[](int index) {
		return _ptr[_index(index)];
	}
	explicit operator char* () {
		return _ptr;
	}
	operator const char* () {
		char* r = new char[_count + 1];
		for (size_t i = 0; i < _count; i++) {
			r[i] = _ptr[_index(i)];
		}
		r[_count] = '\0';
		return r;
	}

};

BlockHandle* _StringBlock(size_t sz) {
	BlockHandle* hsb = new BlockHandle();
	hsb->_offset = 1;
	hsb->_count = sz;
	hsb->_ptr = new char[sz + 1];
	for (size_t i = 0; i < hsb->_count; i++)
		hsb->_ptr[i] = null_char;
	hsb->_ptr[hsb->_count] = '\0';
	return hsb;
}

BlockHandle* _StringBlock(size_t sz, const char* content) {
	BlockHandle* hsb = new BlockHandle();
	hsb->_offset = 1;
	hsb->_count = sz;
	hsb->_ptr = new char[sz + 1];
	for (size_t i = 0; i < hsb->_count; i++)
		hsb->_ptr[i] = i < strlen(content) ? content[i] : null_char;
	hsb->_ptr[hsb->_count] = '\0';
	return hsb;
}

struct StringBlockHandle {
	

	BlockHandle* rows;
	BlockHandle* cols;
	BlockHandle* hsb;
	size_t szBlock;
	size_t nRows = 1;
	size_t nCols =0;
	StringBlockHandle(size_t _szBlock) {
		szBlock = _szBlock;
		hsb = _StringBlock(_szBlock);
		nCols = hsb->_count;
		rows = new BlockHandle[nRows];
		cols = new BlockHandle[nCols];
	}
	StringBlockHandle(size_t _szBlock, const char* content) {
		szBlock = _szBlock;
		hsb = _StringBlock(szBlock,content);
		nCols = hsb->_count;
		rows = new BlockHandle[nRows];
		cols = new BlockHandle[nCols];
	}
	StringBlockHandle(size_t _width, size_t _height, const char* content) {
		szBlock = _width * _height;
		hsb = _StringBlock(_width * _height, content);
		nCols = _width;
		nRows = _height;
		
		rows = new BlockHandle[nRows];
		for (size_t i = 0; i < nRows; i++) {
			rows[i] = { &hsb->_ptr[i * nCols],nCols,1 };
		}
		cols = new BlockHandle[nCols];
		for (size_t i = 0; i < nCols; i++) {
			cols[i] = { &hsb->_ptr[i], nCols, nRows };
		}
		
	}

	StringBlockHandle operator[](size_t i) {
		if (nRows == 1 && nCols == 1)
			return *this;
		size_t c = nCols;
		if (nRows == 1) {
			c = 1;
		}
		return StringBlockHandle(c, 1, (char*)rows[i]);
	}



	char GetChar(size_t x, size_t y) {
		return hsb->_ptr[x + y * nCols];
	}

	void Print2D() {
		for (size_t y = 0; y < nRows; y++) {
			for (size_t x = 0; x < nCols; x++) {
			
				size_t index = x + y * nCols;
				printf("%c",hsb->_ptr[index]);
			}
			printf("\n");
		}
	}

	operator BlockHandle* () const { return hsb; }
	operator const char* () const { return hsb->_ptr; }
	static constexpr char null_char = (char)0xB0;
};

template<size_t _szBlock>
struct ___StringBlockHandler {
	HSB hsb;
	
	___StringBlockHandler(const char* str) {
		hsb = new StringBlock();
		hsb->strContent = str;
		hsb->dwFlags = 0;
		hsb->szBlock = __szBlock;
		hsb->szContent = strlen(str);
		hsb->strBlock = new char[hsb->szContent];
		for (size_t i = 0; i < hsb->szBlock; i++)
			hsb->strBlock[i] = i < hsb->szContent ? str[i] : null_char;
		
		hsb->strBlock[hsb->szBlock] = '\0';
	}


	void RShift() {
		int block_back_ptr = hsb->szBlock - 1;
		int str_front_ptr = hsb->szContent - 1;
		
		for(int i = 0; i < hsb->szContent; i++) {
			hsb->strBlock[block_back_ptr--] = hsb->strBlock[str_front_ptr];
			hsb->strBlock[str_front_ptr--] = null_char;
		}
	}

	// 1 = will overwrite
	// 2 = will push if possible
	// 3 = will force push, data can be deleted
	void Shift(char* buff, int ptr1ToBeShifted,int ptr2ToBeShifted, int shiftAmmount, size_t szstr, size_t szblock, int overwriteMode ) {
		int nChar = ptr1ToBeShifted - ptr2ToBeShifted;

		const int LtR = +1;
		const int RtL = -1;
		const int dir = shiftAmmount > 0 ? LtR : RtL;
		int ptr1 = dir == LtR?ptr2ToBeShifted:ptr1ToBeShifted;
		int ptr2 = dir == LtR ? ptr1ToBeShifted : ptr2ToBeShifted ;
		if (nChar < 0) nChar *= -1;
		
	/*	if(nChar <= 0 || nChar > szstr) {
			return;
		}
		if(buff == 0) {
			return;
		}
		if (shiftAmmount == 0) return;*/
		if(ptr1ToBeShifted == ptr2ToBeShifted) {
			for (int i = szstr - 1 + shiftAmmount; i >= 0 ; i--) {
				buff[ptr1ToBeShifted + i + shiftAmmount] = buff[ptr1ToBeShifted +  i];
				if(i>0)
					buff[ptr1ToBeShifted + i] = null_char;

			}
			buff[ptr1ToBeShifted + shiftAmmount] = buff[ptr1ToBeShifted];
			buff[ptr1ToBeShifted] = null_char;
			
			
			
		}
		else {
			for (int ptr = dir == LtR ? ptr2ToBeShifted : ptr1ToBeShifted; ptr != dir == LtR ? ptr1ToBeShifted : ptr2ToBeShifted; ptr += dir) {
				char tmp = buff[ptr + dir];
				buff[ptr + dir] = buff[ptr];
				buff[ptr] = tmp;
			}
		}
		//-3
		//          v5     
		//   oo ooo ooo__
		//	    ^2 	   	   
		//














	}


	void LShift() {
		int block_front_ptr = 0;
		int str_back_ptr = hsb->szBlock-hsb->szContent;
		
		for (int i = 0; i < hsb->szContent; i++) {
			hsb->strBlock[block_front_ptr++] = hsb->strBlock[str_back_ptr];
			hsb->strBlock[str_back_ptr++] = null_char;
		}
	}

	operator HSB () const { return hsb; }
	operator const char* () const { return hsb->strBlock; }
	static constexpr char null_char = (char)0xB0;
	static constexpr char __szBlock = _szBlock+1;
	
};


using sb4   = ___StringBlockHandler<4>;
using sb8   = ___StringBlockHandler<8>;
using sb16  = ___StringBlockHandler<16>;
using sb32  = ___StringBlockHandler<32>;
using sb64  = ___StringBlockHandler<64>;
using sb128 = ___StringBlockHandler<128>;


	
	
	













