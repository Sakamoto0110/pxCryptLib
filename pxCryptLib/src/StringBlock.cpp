#include "pch.h"

#include "../include/StringBlock.h"



pxCrypt_API StringBlockHandle_t CreateStringBlockHandle(uint16_t offset, uint16_t size, void* data) {
	StringBlockHandle_t sb;
	sb.offset = offset;
	sb.size = size;
	sb.data = (decltype(sb.data))data;
	sb.rows = 0;
	sb.cols = 0;
	sb.ex = sbSUB;
	if (size > 1) {
		sb.cols = new StringBlockHandle_t[sb.size];
		sb.rows = new StringBlockHandle_t[sb.size];
		for (uint16_t i = 0; i < sb.size; i++) {
			sb.cols[i] = CreateStringBlockHandle(1, 1, &((decltype(sb.data))data)[i]);
			sb.cols[i].ex = sb.cols[i].ex | sbCOLUMN | sbSINGULAR;
			sb.rows[i] = CreateStringBlockHandle(1, 1, &((decltype(sb.data))data)[i]);
			sb.rows[i].ex = sb.rows[i].ex | sbROW | sbSINGULAR;
		}
	}

	return sb;
}


pxCrypt_API StringBlockHandle_t CreateStringBlockHandle(uint16_t width, uint16_t height) {
	StringBlockHandle_t sb;
	sb.offset = 1;
	sb.size = width * height;
	sb.data = (decltype(sb.data))calloc(sb.size, sizeof(decltype(sb.data)));
	for (uint16_t i = 0; i < sb.size; i++)
		sb.data[i].ch = 0;
	sb.cols = new StringBlockHandle_t[width];
	for (uint16_t i = 0; i < width; i++) {
		sb.cols[i] = CreateStringBlockHandle(width, height, &sb.data[i]);
		sb.cols[i].ex = sb.cols[i].ex | sbCOLUMN | sbSUB;
	}

	sb.rows = new StringBlockHandle_t[height];
	for (uint16_t i = 0; i < height; i++) {
		sb.rows[i] = CreateStringBlockHandle(1, width, &sb.data[i * width]);
		sb.rows[i].ex = sb.rows[i].ex | sbROW | sbSUB;
	}
	sb.ex = sbROOT | sbDISPOSABLE;
	return sb;
}

pxCrypt_API void Dispose(StringBlockHandle_t& handle) {
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
	if ((handle.ex & sbDISPOSABLE) == sbDISPOSABLE) {
		delete handle.data;
	}
}


pxCrypt_API void _shift(StringBlockHandle_t* _handle, int32_t value) {
	auto _swap = [](StringBlockHandle_t::SBHDATA* __a, StringBlockHandle_t::SBHDATA* __b) {
		StringBlockHandle_t::SBHDATA tmp = *__a;
		*__a = *__b;
		*__b = tmp;
	};
	if (value > 0)
		for (int i = _handle->size - 1; i > 0; i--) {
			auto a = i * _handle->offset;
			auto b = ((i + value) % _handle->size) * _handle->offset;
			_swap(&_handle->data[a], &_handle->data[b]);
		}
	else
		for (int i = 0; i < _handle->size - 1; i++) {
			auto a = i * _handle->offset;
			auto b = ((i - value) % _handle->size) * _handle->offset;
			_swap(&_handle->data[a], &_handle->data[b]);
		}
}

pxCrypt_API bool IsColumn(const StringBlockHandle_t& _handle) { return (_handle.ex & sbCOLUMN) == sbCOLUMN; }
pxCrypt_API bool IsRow(const StringBlockHandle_t& _handle) { return (_handle.ex & sbROW) == sbROW; }
pxCrypt_API bool IsOwner(const StringBlockHandle_t& _handle) { return (_handle.ex & sbROOT) == sbROOT; }
pxCrypt_API bool IsSingular(const StringBlockHandle_t& _handle) { return (_handle.ex & sbSINGULAR) == sbSINGULAR; }
pxCrypt_API bool IsDisposable(const StringBlockHandle_t& _handle) { return (_handle.ex & sbDISPOSABLE) == sbDISPOSABLE; }






pxCrypt_API int16_t GetWidth(const StringBlockHandle_t& handle) {
	if (IsOwner(handle)) {
		return handle.size / handle.cols[0].size;
	}
	return handle.cols == 0 ? 0 : handle.cols[0].size;
}

pxCrypt_API int16_t GetHeight(const StringBlockHandle_t& handle) {
	if (IsOwner(handle)) {
		return handle.size / handle.rows[0].size;
	}
	return handle.rows == 0 ? 0 : handle.rows[0].size;
}

pxCrypt_API int16_t GetSize(const StringBlockHandle_t& handle) { return handle.size; }





struct BoolString {
	bool value;
	BoolString(bool b) : value(b) {}
	constexpr operator const char* () { return value ? "true" : "false"; }
};


pxCrypt_API void ShowInfo(const StringBlockHandle_t& handle) {
	printf("Handle size: %i\n", handle.size);
	printf("Handle flags: %i\n", handle.ex);
	printf("Is column: %s\n", (const char*)BoolString(IsColumn(handle)));
	printf("Is row: %s\n", (const char*)BoolString(IsRow(handle)));
	printf("Is root: %s\n", (const char*)BoolString(IsOwner(handle)));
	printf("Is singular: %s\n", (const char*)BoolString(IsSingular(handle)));
	printf("Is disposable: %s\n", (const char*)BoolString(IsDisposable(handle)));

	printf("column count: %i\n", GetWidth(handle));
	printf("row count: %i\n\n", GetHeight(handle));

	if (!IsSingular(handle)) {
		for (int i = 0; i < GetHeight(handle); i++) {
			ShowInfo(handle.rows[i]);
		}

		for (int i = 0; i < GetHeight(handle); i++) {
			ShowInfo(handle.cols[i]);
		}
	}
}