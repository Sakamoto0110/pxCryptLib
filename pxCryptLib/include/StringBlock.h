#pragma once
#include <stdlib.h>
#include <cstdint>

constexpr uint16_t _hword(int32_t _dword) { return (_dword & 0xFFFF0000) >> 16; }
constexpr uint16_t _lword(int32_t _dword) { return (_dword & 0x0000FFFF) >> 0; }
constexpr uint8_t _hhByte(int32_t _dword) { return (_dword & 0xFF000000) >> 24; }
constexpr uint8_t _hlByte(int32_t _dword) { return (_dword & 0x00FF0000) >> 16; }
constexpr uint8_t _lhByte(int32_t _dword) { return (_dword & 0x0000FF00) >> 8; }
constexpr uint8_t _llByte(int32_t _dword) { return (_dword & 0x000000FF) >> 0; }

// range: 0 - 65565
constexpr uint32_t _2wordDWORD(uint16_t hWord, uint16_t lWord) {
	return	((hWord & 0x0FFFF) << 16) |
		((lWord & 0x0FFFF) << 0);
}

constexpr uint32_t _4byteDWORD(uint8_t hhByte, uint8_t hlByte, uint8_t lhByte, uint8_t llByte) {
	return	((hhByte & 0x0FF) << 24) |
		((hlByte & 0x0FF) << 16) |
		((lhByte & 0x0FF) << 8) |
		((llByte & 0x0FF) << 0);
}






// return a dword containing 2 words ( Little endian convention )
constexpr uint32_t _dw4Words(uint16_t word1 = 0, uint16_t word2 = 0) {
	return	((word2 & 0x0FFFF) << 16) |
			((word1 & 0x0FFFF) <<  0) ;
}

// return a dword containing 4 bytes ( Little endian convention )
constexpr uint32_t _dw4Bytes(uint8_t byte1 = 0, uint8_t byte2 = 0, uint8_t byte3 = 0, uint8_t byte4 = 0) {
	return	((byte4 & 0x0FF) << 24) |
			((byte3 & 0x0FF) << 16) |
			((byte2 & 0x0FF) << 8)  |
			((byte1 & 0x0FF) << 0)  ;
}



// C Ready code
#if 0
// return max value of a word
uint16_t _dwWord(size_t little_endian_byte) {
	return (0xFFFF << ((little_endian_byte - 1) * 16));
}

// sets the value of the word in the dword ( and returns the dword )
uint32_t _dwWord(size_t little_endian_byte, uint32_t dword, uint16_t word) {
	return ((word & 0xFFFF) << ((little_endian_byte - 1) * 16)) | dword;
}

// returns the desired word contained in the dword
uint16_t _dwWord(size_t little_endian_byte, uint32_t dword) {
	return (dword & _dwWord(little_endian_byte)) >> ((little_endian_byte - 1) * 16);
}

// return max value of a byte
uint32_t _dwByte(size_t little_endian_byte) {
	return (0xFF << ((little_endian_byte - 1) * 8));
}

// sets the value of the byte in the dword ( and returns the dword )
uint32_t _dwByte(size_t little_endian_byte, uint32_t dword, uint8_t byte) {
	return ((byte & 0xFF) << ((little_endian_byte - 1) * 8)) | dword;
}

// returns the desired byte contained in the dword
uint8_t _dwByte(size_t little_endian_byte, uint32_t dword) {
	return (dword & _dwByte(little_endian_byte)) >> ((little_endian_byte - 1) * 8);
}
	

#endif











// return max value of a word
template<size_t little_endian_byte>
constexpr uint32_t _dwWord() {
	static_assert(little_endian_byte >= 0 && little_endian_byte <= 2,
		"byte must be between 0 and 2, following little endian convention.");
	return (0xFFFF << ((little_endian_byte - 1) * 16));
}

// sets the value of the word in the dword ( and returns the dword )
template<size_t little_endian_byte>
uint32_t _dwWord(uint32_t& dword, uint16_t word) {
	static_assert(little_endian_byte >= 0 && little_endian_byte <= 2,
		"byte must be between 0 and 2, following little endian convention.");
	return dword = (((word & 0xFFFF) << ((little_endian_byte - 1) * 16)) | dword);
}

// returns the desired word contained in the dword
template<size_t little_endian_byte>
constexpr uint16_t _dwWord(uint32_t dword) {
	static_assert(little_endian_byte >= 0 && little_endian_byte <= 2,
		"byte must be between 0 and 2, following little endian convention.");
	return (dword & _dwWord<little_endian_byte>()) >> ((little_endian_byte - 1) * 16);
}




// return max value of a byte
template<size_t little_endian_byte>
constexpr uint32_t _dwByte() {
	static_assert(little_endian_byte >= 0 && little_endian_byte <= 4,
		"byte must be between 0 and 4, following little endian convention.");
	return (0xFF << ((little_endian_byte - 1) * 8));
}

// sets the value of the byte in the dword ( and returns the dword )
template<size_t little_endian_byte>
uint32_t _dwByte(uint32_t& dword, uint8_t byte) {
	static_assert(little_endian_byte >= 0 && little_endian_byte <= 4,
		"byte must be between 0 and 4, following little endian convention.");
	return dword = (((byte & 0xFF) << ((little_endian_byte - 1) * 8)) | dword);
}

// returns the desired byte contained in the dword
template<size_t little_endian_byte>
constexpr uint8_t _dwByte(uint32_t dword) {
	static_assert(little_endian_byte >= 0 && little_endian_byte <= 4,
		"byte must be between 0 and 4, following little endian convention.");
	return (dword & _dwByte<little_endian_byte>()) >> ((little_endian_byte - 1) * 8);
	
}





//
//constexpr uint32_t _SetB1(uint8_t byte, uint32_t dst) {
//	return (byte) & 0xFF << 24 | dst;
//}
//constexpr uint32_t _SetB2(uint8_t byte, uint32_t dst) {
//	return (byte) & 0xFF << 24 | dst;
//	uint8_t b1 = _hhByte(dst);
//	uint8_t b2 = _hhByte(dst);
//	uint8_t b3 = _hhByte(dst);
//}
//constexpr uint32_t _SetB3(uint8_t byte, uint32_t dst) {
//	return (byte) & 0xFF << 24 | dst;
//	uint8_t b1 = _hhByte(dst);
//	uint8_t b2 = _hhByte(dst);
//	uint8_t b3 = _hhByte(dst);
//}
//constexpr uint32_t _SetB4(uint8_t byte, uint32_t dst) {
//	uint8_t b1 = _hhByte(dst);
//	uint8_t b2 = _hhByte(dst);
//	uint8_t b3 = _hhByte(dst);
//}

struct StringBlockHandle_t {
	int16_t offset;
	int16_t size;
	uint32_t ex;
	struct StringBlockHandle_t* rows;
	struct StringBlockHandle_t* cols;
	typedef struct _charEx_t{
		char ch;
		int32_t color;
		operator char() { return ch; }
		
	};
	_charEx_t* data;
	decltype(*data) operator[](size_t index) {

		return data[index*offset];
	}
	explicit operator char() { return (*data).ch; }
};



void _swap(void* a, void* b) {
	unsigned char* buff[sizeof(StringBlockHandle_t::_charEx_t)];
	memcpy(buff, a, sizeof(StringBlockHandle_t::_charEx_t));
	memcpy(a, b, sizeof(StringBlockHandle_t::_charEx_t));
	memcpy(b, buff, sizeof(StringBlockHandle_t::_charEx_t));
}


void _shift(StringBlockHandle_t* _handle, int32_t value) {
	auto _a = [&_handle](size_t _t) {return &_handle->data[_t * _handle->offset]; };
	auto _b = [&_handle,&value](size_t _t) {return &_handle->data[((_t + value) % _handle->size) * _handle->offset]; };
	if (value > 0)
		for (size_t i = _handle->size - 1; i > 0; i--) 
			_swap(_a(i), _b(i));
	else
		for (size_t i = 0; i < _handle->size; i++) 
			_swap(_a(i), _b(i));
}
bool IsColumn(const StringBlockHandle_t& _handle) { return _dwByte<1>(_handle.ex) == 0xF0; }
bool IsRow(const StringBlockHandle_t& _handle) { return _dwByte<1>(_handle.ex) == 0x0F; }
bool IsOwner(const StringBlockHandle_t& _handle) { return _dwWord<1>(_handle.ex) == 0xFFFF; }

int16_t GetWidth(const StringBlockHandle_t& handle) {
	if (IsOwner(handle)) {
		return handle.size / handle.cols[0].size;
	}
	return handle.cols == 0 ? 0 : handle.cols[0].size; 
}
int16_t GetHeight(const StringBlockHandle_t& handle) { 
	if (IsOwner(handle)) {
		return handle.size / handle.rows[0].size;
	}
	return handle.rows == 0 ? 0 : handle.rows[0].size; 
}
int16_t GetSize(const StringBlockHandle_t& handle) { return handle.size; }

void Dispose(StringBlockHandle_t& handle) {
	int16_t w = GetWidth(handle);
	int16_t h = GetHeight(handle);
	for (int16_t i = 0; i < w; i++) {
		Dispose(handle.cols[i]);
	}
	for (int16_t i = 0; i < h; i++) {
		Dispose(handle.rows[i]);
	}
	delete handle.cols;
	delete handle.rows;
	if (_hword(handle.ex) != 0xffff) {
		delete handle.data;
	}

}


StringBlockHandle_t CreateStringBlockHandle(uint16_t offset, uint16_t size, void* data) {
	StringBlockHandle_t sb;
	sb.offset = offset;
	sb.size = size;
	sb.data = (decltype(sb.data))data;
	sb.rows = 0;
	sb.cols = 0;
	sb.ex = 0;
	sb.ex = _dwWord<2>(sb.ex, 0xffff);
	if (size > 1) {
		sb.cols = new StringBlockHandle_t[sb.size];
		sb.rows = new StringBlockHandle_t[sb.size];
		for (uint16_t i = 0; i < sb.size; i++) {
			sb.cols[i] = CreateStringBlockHandle(1, 1, &((decltype(sb.data))data)[i]);
			sb.rows[i] = CreateStringBlockHandle(1, 1, &((decltype(sb.data))data)[i]);
		}
	}

	return sb;
}


StringBlockHandle_t CreateStringBlockHandle(uint16_t width, uint16_t height) {
	StringBlockHandle_t sb;
	sb.offset = 1;
	sb.size = width * height;
	sb.data = (decltype(sb.data))calloc(sb.size, sizeof(decltype(sb.data)));
	for (uint16_t i = 0; i < sb.size; i++) 
		sb.data[i].ch = 0;
	sb.cols = new StringBlockHandle_t[width];
	for (uint16_t i = 0; i < width; i++) {
		sb.cols[i] = CreateStringBlockHandle(width, height, &sb.data[i]);
		sb.cols[i].ex = _dwByte<1>(sb.cols[i].ex, 0xF0); // 0x00000001 0b0001
	}

	sb.rows = new StringBlockHandle_t[height];
	for (uint16_t i = 0; i < height; i++) {
		sb.rows[i] = CreateStringBlockHandle(1, width, &sb.data[i * width]);
		sb.rows[i].ex = _dwByte<1>(sb.rows[i].ex, 0x0F); // 0x00000002 0b0010
	}
	sb.ex = _dwWord<1>(sb.ex, 0xFFFF); // 0x00000003 0b0011
	return sb;
}

// Creates another handle and copies the data from src, this handler also must to be disposed
[[nodiscard]] StringBlockHandle_t _copyStringBlock(const StringBlockHandle_t& src) {
	StringBlockHandle_t dst = CreateStringBlockHandle(GetWidth(src), GetHeight(src));
	for(size_t i = 0; i < dst.size; i++)
		memcpy(dst.data, src.data, sizeof(StringBlockHandle_t::_charEx_t));
	return dst;
}

void PopulateStringBlock(StringBlockHandle_t& sb, const char* str) {
	for (int i = 0; i < strlen(str); i++) {
		sb.data[i].ch = str[i];
	}
}

size_t _indexToCoord(size_t i, const StringBlockHandle_t& _handle) {
	//size_t result = i / _handle.
	return 0;
}

void printBlock(StringBlockHandle_t& handle) {
	int sz = handle.size;
	auto w = GetWidth(handle);
	auto h = GetHeight(handle);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			printf("%c", handle.rows[y][x]);
		}
		printf("\n");
	}
	for (int i = 0; i < sz; i++) {
		//printf("%c", handle.data[i*handle.offset].ch);
	}
}


//class SimpleIterator  {
//public:
//	SimpleIterator(StringBlockHandle_t* ptr) :  _activeHandle(ptr) {
//	}
//
//	//void SwitchToRow(size_t idxRow) { _activeHandle = &_owner.rows[idxRow]; }
//	//void SwitchToCol(size_t idxCol) { _activeHandle = &_owner.cols[idxCol]; }
//
//
//	//void RestorePosition() { pos = 0; }
//	//void Restore() { _activeHandle = &_owner; pos = 0; }
//
//	
//	StringBlockHandle_t::_charEx_t* begin() {
//		return &_activeHandle->data[0];
//	}
//	StringBlockHandle_t::_charEx_t* end() {
//		return &_activeHandle->data[(_activeHandle->offset * _activeHandle->size)];
//	}
//	StringBlockHandle_t::_charEx_t* Actual() {
//		return *this;
//	}
//	operator StringBlockHandle_t::_charEx_t* () {
//		return &_activeHandle->data[(_activeHandle->offset * (pos)) % _activeHandle->size];
//		
//	}
//
//	StringBlockHandle_t::_charEx_t* Next() {
//		pos++;
//		return *this;
//		auto* a = Actual();
//		++pos;
//		return a;
//	}
//	SimpleIterator& operator ++() {
//		Next();
//		return *this;
//	}
//protected:
//	size_t pos = 0;
//	StringBlockHandle_t* _activeHandle;
//private:
//	
//
//};

#define _ auto 

using sbh = StringBlockHandle_t;
class StringBlock {
public:
	class sbhAcessor {
	public:
		explicit sbhAcessor(sbh& _handle):_sbh(_handle){}

		auto operator[](size_t index) -> auto {
			return _sbh.data[index * _sbh.offset];
		}
		
	private:
		const sbh& _sbh;
	};
	
	class Iterator {
		friend class StringBlock;		
		using szpt = int32_t;
		StringBlockHandle_t* _handle;
		struct {
			szpt x = 0;
			szpt y = 0;
			bool operator ==(auto a ) {
				return a.x == x && a.y == y;
			}
		}ptr;

	public:
		
		decltype(ptr) TOP {  0, -1 };
		decltype(ptr) BOTTOM 			{  0, 1 };
		decltype(ptr) LEFT 			{ -1, 0 };
		decltype(ptr) RIGHT 			{  1, 0 };
		decltype(ptr) TOPLEFT 			{ -1, -1 };
		decltype(ptr) TOPRIGHT 			{  1, -1 };
		decltype(ptr) BOTTOMLEFT 			{ -1, 1 };
		decltype(ptr) BOTTOMRIGHT 			{  1, 1 };


		bool CanMove(decltype(ptr) p) {
			if (p == TOP)     return ptr.y+p.y > 0;
			if (p == BOTTOM)  return ptr.y+p.y < _handle->cols[0].size;
			if (p == LEFT)    return ptr.x+p.x > 0;
			if (p == RIGHT)   return ptr.x+p.x < _handle->rows[0].size;

			/*if (p == _dirs[TOPLEFT]);
			if (p == _dirs[TOPRIGHT]);
			if (p == _dirs[BOTTOMLEFT]);
			if (p == _dirs[BOTTOMLEFT]);*/
		}
	

		void move(decltype(ptr) dst) {
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

		Iterator& operator +=(decltype(ptr) dst) {
			move(dst);
			return *this;
		}

		operator decltype(*_handle->data)(){
			return _handle->rows[ptr.y%_handle->cols[0].size][ptr.x % _handle->rows[0].size];
		}
		
		
	};
	using Iter = Iterator;

	Iter GetRowIterator(size_t idxRow) {
		Iter it;
		it._handle = &_handle;
		return it;
	}

	explicit StringBlock(const char* str) : _handle(CreateStringBlockHandle(strlen(str), 1)) {
		PopulateStringBlock(_handle, str);
	}
	explicit StringBlock(size_t sz, const char* str) : _handle(CreateStringBlockHandle(sz, 1)) {
		PopulateStringBlock(_handle, str);
	}
	explicit StringBlock(size_t width, size_t height, const char* str) : _handle(CreateStringBlockHandle(width, height)) {
		PopulateStringBlock(_handle, str);
	}

	void ShiftRow(size_t idxRow, int32_t non_zero_value) {
		auto value = non_zero_value;
		if (value == 0) return;
		_shift(&_handle.rows[idxRow], value);

	}

	void ShiftCol(size_t idxCol, int32_t non_zero_value) {
		auto value = non_zero_value;
		if (value == 0) return;
		_shift(&_handle.cols[idxCol], value);

	}

	explicit operator StringBlockHandle_t& () {
		return _handle;
	}
	
	StringBlockHandle_t& GetHandle() {
		return _handle;
	}
	StringBlockHandle_t& GetRowHandle(size_t idxRow) {
		return _handle.rows[idxRow];
	}
	StringBlockHandle_t& GetColHandle(size_t idxCol) {
		return _handle.cols[idxCol];
	}


	sbhAcessor GetRow(size_t idxRow) {
		return sbhAcessor(_handle.rows[idxRow]);
	}
	sbhAcessor GetCol(size_t idxCol) {
		return sbhAcessor(_handle.cols[idxCol]);
	}

	void for_each(StringBlockHandle_t h, void(*f)(decltype(*h.data)&)) {
		for (size_t i = 0; i < h.size; i++) {
			f(h.data[i]);
		}
	}

	void Fill_null_spaces(char _null_char) {
		for (size_t i = 0; i < _handle.size; i++) {
			if (_handle.data[i] == 0)
				_handle.data[i].ch = _null_char;
		}
	}

	int16_t Size() const { return GetSize(_handle); }
	int16_t Width() const { return GetWidth(_handle); }
	int16_t Height() const { return GetHeight(_handle); }
	
	
	void Print() {
		auto w = Width();
		auto h = Height();
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if (_handle.rows[y][x].color != 0) {
					auto r = GetRed(_handle.rows[y][x].color);
					auto g = GetGreen(_handle.rows[y][x].color);
					auto b = GetBlue(_handle.rows[y][x].color);
					printf("\x1b[48;2;%i;%i;%im ", r, g, b);
				}
				printf("%c", _handle.rows[y][x].ch);
			}
			printf("\n");
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