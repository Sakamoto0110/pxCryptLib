#pragma once


#ifdef pxCryptAPI_EXPORTS
#define pxCrypt_API extern "C" __declspec(dllexport)
#else
#define pxCrypt_API extern "C" __declspec(dllimport)
#endif


using pxBYTE = unsigned char;
using pxWORD = unsigned long;
using pxDWORD = unsigned long long;
