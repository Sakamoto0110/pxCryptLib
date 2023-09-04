#pragma once
#include "pxCrypt_core.h"

pxCrypt_API bool IsPrime(pxDWORD value);
pxCrypt_API void GetNPrimes(pxDWORD startValue, pxDWORD* buffer, size_t szBuffer);
pxCrypt_API void _ReadTable(const char* filename, pxDWORD* dst, size_t sz);
pxCrypt_API void _WriteTable(const char* filename, pxDWORD* src, size_t sz);
pxCrypt_API void GenerateTable(pxDWORD* buffer, size_t szBuffer, size_t partitionCount = 1);
pxCrypt_API void LoadTable(const char* filename, pxDWORD* buffer, size_t szBuffer);
pxCrypt_API void PreComputeTable(const char* filename, size_t szBuffer);