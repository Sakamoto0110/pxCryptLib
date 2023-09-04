#include "pch.h"
#include "../include/pUtils.h"



bool IsPrime(pxDWORD n) {
	for (size_t i = n - 1; i > 1; i--) {
		if (n % i == 0) return false;
	}
	return true;
}




void GetNPrimes(pxDWORD startValue, pxDWORD* buffer, size_t szBuffer) {
	size_t n = 0;

	for (size_t i = n + startValue; n < szBuffer; i++) {
		if (IsPrime(i)) {
			buffer[n++] = i;
		}
	}
}

void GenerateTable(pxDWORD* buffer, size_t szBuffer, size_t partitionCount) {
	const size_t elementsPerTable = szBuffer / partitionCount;
	size_t starting_value = 2;
	for (size_t i = 0; i < partitionCount; i++) {
		const size_t index = i * elementsPerTable;
		GetNPrimes(starting_value, buffer + index, elementsPerTable);
		starting_value = buffer[index + elementsPerTable - 1] + 1;
	}
}
void LoadTable(const char* filename, pxDWORD* buffer, size_t szBuffer) {
	_ReadTable(filename, buffer, szBuffer);
}
void PreComputeTable(const char* filename, size_t szBuffer) {
	pxDWORD* buffer = new pxDWORD[szBuffer];
	GenerateTable(buffer, szBuffer, 1);
	_WriteTable(filename, buffer, szBuffer);
	delete[] buffer;
}

#include <fstream>

void _ReadTable(const char* filename, pxDWORD* dst, size_t sz) {
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	file.read((char*)dst, sz * sizeof(pxDWORD));
	file.close();
}
void _WriteTable(const char* filename, pxDWORD* src, size_t sz) {
	std::ofstream file(filename, std::ios::out | std::ios::binary);
	file.write((char*)src, sz * sizeof(pxDWORD));
	file.close();
}