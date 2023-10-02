#pragma once
#define pxCryptAPI_EXPORTS
#include "pxCrypt_core.h"
#include <stdlib.h>
#include <cstdint>
#include "pxvt.h"
#include <vector>


constexpr uint32_t sbSINGULAR = 0b0001;
constexpr uint32_t sbSUB = 0b0010;
constexpr uint32_t sbROOT = 0b0100;
constexpr uint32_t sbDISPOSABLE = 0b1000;

constexpr uint32_t sbROW = 0b0001 << 4;
constexpr uint32_t sbCOLUMN = 0b0010 << 4;


struct StringBlockHandle_t {
	int16_t offset;
	int16_t size;
	uint32_t ex;
	struct StringBlockHandle_t* rows;
	struct StringBlockHandle_t* cols;

	struct SBHDATA{
		union {
			union {
				uint32_t value;
				struct {
					uint8_t a;
					uint8_t b;
					uint8_t g;
					uint8_t r;
				};
			}color;
			uint8_t ch;
		};
	}*data;

	/*struct SBHDATA {
		union {
			struct {
				uint8_t ch;
				uint8_t b;
				uint8_t g;
				uint8_t r;
				
			};
			uint32_t value;
			
		};
	}*data;*/

	decltype(*data) operator[](size_t index) {

		return data[index * offset];
	}
	explicit operator char() { return (*data).ch; }
};




//
//void _shift(StringBlockHandle_t* _handle, int32_t value) {
//	auto _a = [&_handle](size_t _t) {return &_handle->data[_t * _handle->offset]; };
//	auto _b = [&_handle,&value](size_t _t) {return &_handle->data[((_t + value) % _handle->size) * _handle->offset]; };
//	
//	/*auto _swap = [](decltype(_handle->data) __a, decltype(_handle->data) __b) {
//		unsigned char* buff[sizeof(decltype(_handle->data))];
//		memcpy(buff, __a, sizeof(decltype(_handle->data)));
//		memcpy(__a, __b, sizeof(decltype(_handle->data)));
//		memcpy(__b, buff, sizeof(decltype(_handle->data)));
//	};*/
//	auto _swap = [](StringBlockHandle_t::SBHDATA* __a, StringBlockHandle_t::SBHDATA* __b) {
//		StringBlockHandle_t::SBHDATA tmp = *__a;
//		*__a = *__b;
//		*__b = tmp;
//	};
//	if (value > 0)
//		for (int i = _handle->size - 1; i > 0; i--) {
//			auto a = i * _handle->offset;
//			auto b = ((i + value) % _handle->size) * _handle->offset;
//		//	printf("[(%i:%c): (%i:%c)] ", a, _handle->data[a].ch, b, _handle->data[b].ch);
//			_swap(&_handle->data[a], &_handle->data[b]);
//		}
//			
//	else
//		for (int i = 0; i < _handle->size-1; i++) {
//			auto a = i * _handle->offset;
//			auto b = ((i - value) % _handle->size) * _handle->offset;
//			//printf("[(%i:%c): (%i:%c)] ", a, _handle->data[a].ch, b, _handle->data[b].ch);
//			_swap(&_handle->data[a], &_handle->data[b]);
//		}
//	//printf("\n");
//}

// column 0b0001
// row    0b0010
// root   0b0011
// 
// 
// 

pxCrypt_API StringBlockHandle_t CreateStringBlockHandle(uint16_t offset, uint16_t size, void* data);
pxCrypt_API StringBlockHandle_t CreateStringBlockHandle(uint16_t width, uint16_t height);

pxCrypt_API void Dispose(StringBlockHandle_t& handle);
pxCrypt_API void _shift(StringBlockHandle_t* _handle, int32_t value);

pxCrypt_API bool IsColumn(const StringBlockHandle_t& _handle);
pxCrypt_API bool IsRow(const StringBlockHandle_t& _handle);
pxCrypt_API bool IsOwner(const StringBlockHandle_t& _handle);
pxCrypt_API bool IsSingular(const StringBlockHandle_t& _handle);
pxCrypt_API bool IsDisposable(const StringBlockHandle_t& _handle);

pxCrypt_API int16_t GetWidth(const StringBlockHandle_t& handle);
pxCrypt_API int16_t GetHeight(const StringBlockHandle_t& handle);
pxCrypt_API int16_t GetSize(const StringBlockHandle_t& handle);

pxCrypt_API void ShowInfo(const StringBlockHandle_t& handle);






//
//// Creates another handle and copies the data from src, this handler also must to be disposed
//[[nodiscard]] StringBlockHandle_t _copyStringBlock(const StringBlockHandle_t& src) {
//	StringBlockHandle_t dst = CreateStringBlockHandle(GetWidth(src), GetHeight(src));
//	for(size_t i = 0; i < dst.size; i++)
//		memcpy(dst.data, src.data, sizeof(decltype(src.data)));
//	return dst;
//}


class Iterator2D {
public :
	struct IterPoint {
		int32_t x;
		int32_t y;
		bool operator ==(const IterPoint& a) const {
			return a.x == x && a.y == y;
		}
	};
	IterPoint TOP{ 0, -1 };
	IterPoint BOTTOM{ 0, 1 };
	IterPoint LEFT{ -1, 0 };
	IterPoint RIGHT{ 1, 0 };
	IterPoint TOPLEFT{ -1, -1 };
	IterPoint TOPRIGHT{ 1, -1 };
	IterPoint BOTTOMLEFT{ -1, 1 };
	IterPoint BOTTOMRIGHT 			{  1, 1 };

	bool CanMove(IterPoint p) {
		if (p == TOP)     return ptr.y + p.y > 0;
		if (p == BOTTOM)  return ptr.y + p.y < _handle->cols[0].size;
		if (p == LEFT)    return ptr.x + p.x > 0;
		if (p == RIGHT)   return ptr.x + p.x < _handle->rows[0].size;

		/*if (p == _dirs[TOPLEFT]);
		if (p == _dirs[TOPRIGHT]);
		if (p == _dirs[BOTTOMLEFT]);
		if (p == _dirs[BOTTOMLEFT]);*/
	}
	void move(IterPoint dst) {
		size_t szRows = _handle->rows[0].size;
		size_t szCols = _handle->cols[0].size;
		int16_t offset = _handle->cols[0].offset;
		if (CanMove(dst)) {
			ptr.x += dst.x;
			ptr.y += dst.y;
		}
		else {
			int i = 0;
		}
	}

	Iterator2D& operator +=(IterPoint dst) {
		move(dst);
		return *this;
	}
	IterPoint ptr;
	StringBlockHandle_t* _handle;
	operator decltype(*_handle->data)(){
		return _handle->rows[ptr.y % _handle->cols[0].size][ptr.x % _handle->rows[0].size];
	}

private:
	
};
#include "visualizer.h"
using sbh = StringBlockHandle_t;
class StringBlock {
public:
	
	using Iter = Iterator2D;

	Iter GetRowIterator(size_t idxRow) {
		Iter it;
		it._handle = &_handle;
		return it;
	}


	explicit StringBlock(size_t sz) : _handle(CreateStringBlockHandle(sz, 1)) {
		
	}
	explicit StringBlock(size_t width, size_t height) : _handle(CreateStringBlockHandle(width, height)) {
		
	}

	


	void SetColorMap(uint32_t* color_map) {
		for (int i = 0; i < _handle.size; i++) {
			uint8_t ch = _handle.data[i].ch;
			_handle.data[i].color = { color_map[i] };
			_handle.data[i].ch = ch;
		}
	}

	void SetContent(const char* str) {
		for (int i = 0; i < strlen(str); i++) {
			_handle.data[i].ch = str[i];
		}
	}

	void ShiftRow(size_t idxRow, int32_t non_zero_value) {
		auto value = non_zero_value;
		if (value == 0) return;
		_shift(&_handle.rows[idxRow], value);

	}

	void ShiftCol(size_t idxCol, int32_t non_zero_value) {
		auto value = non_zero_value;
		if (value == 0) return;

		if (value < 0) {
			while (value++ < 0) {
				_shift(&_handle.cols[idxCol], -1);
			}
		}
		else if (value > 0) {
			while (value-- > 0) {
				_shift(&_handle.cols[idxCol], 1);
			}
		}

		/*ifnon_zero_value = non_zero_value * -1 + this->Height()-1;
		} (non_zero_value < 0) {
			*/
		

	}

	explicit operator StringBlockHandle_t& () {
		return _handle;
	}
	
	StringBlockHandle_t& GetHandle() {
		return _handle;
	}
	

	void for_each(StringBlockHandle_t h, void(*f)(decltype(*h.data)&)) {
		for (size_t i = 0; i < h.size; i++) {
			f(h.data[i]);
		}
	}

	void FillWith(char ch) {
		for (size_t i = 0; i < _handle.size; i++) {
			_handle.data[i].ch = ch;
		}
	}

	void Fill_null_spaces(char _null_char) {
		for (size_t i = 0; i < _handle.size; i++) {
			if (_handle.data[i].ch == 0)
				_handle.data[i].ch = _null_char;
		}
	}

	int16_t Size() const { return GetSize(_handle); }
	int16_t Width() const { return GetWidth(_handle); }
	int16_t Height() const { return GetHeight(_handle); }
	
	const char* ToString() {
		char* str = new char[_handle.size+1];
		for (int i = 0; i < _handle.size; i++) {
			str[i] = _handle.data[i].ch;
		}
		str[_handle.size] = '\0';
		return str;
	}
	
	void Print(int x, int y, uint32_t printFlags = 0) {
		vtCursor<move>(x, y);
		Print(printFlags);
	}

	void Print(uint32_t printFlags = 0) {
		auto w = Width();
		auto h = Height();
		for (int y = 0; y < h; y++) {
			vtCursor<save>();
			for (int x = 0; x < w; x++) {
				auto color = _handle.rows[y][x].color;
				char spacing = printFlags == 1 ? ' ' : 0;
				if (color.value != 0)
					std::cout << vtBackgroundColor(color.value) << spacing << _handle.rows[y][x].ch;
				else
					std::cout << _handle.rows[y][x].ch;
			}
			vtCursor<load>();
			vtCursor<moveDown>();
		
		}
	}

	~StringBlock() {
		Dispose(_handle);
	}
private:
	using _shiftFunc_t = void(*)(StringBlockHandle_t*, size_t);
	//_shiftFunc_t _GetShiftFunc(int32_t val) { return val < 0 ? _rshift : _lshift; }
	StringBlockHandle_t _handle;
};
