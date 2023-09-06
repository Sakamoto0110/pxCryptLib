#pragma once


#ifdef pxCryptAPI_EXPORTS
#define pxCrypt_API __declspec(dllexport)
#else
#define pxCrypt_API __declspec(dllimport)
#endif


using pxBYTE   = unsigned char;  // 1 bytes
using pxWORD   = unsigned short; // 2 bytes
using pxDWORD  = unsigned long;  // 4 bytes

using pxCHAR   = char;       	 // 1 byte
using pxPCHAR  = char*;          // 8 bytes
using pxPSTR   = const char*;    // 8 bytes
								 
using pxINT16  = int;            // 2 bytes
using pxINT32  = long;      	 // 4 bytes
using pxINT64  = long long; 	 // 8 bytes

using pxI16    = pxINT16;		 // 2 bytes
using pxI32    = pxINT32; 		 // 4 bytes
using pxI64    = pxINT64; 		 // 8 bytes

using pxINT    = pxINT32;      	 // 4 bytes

using pxFLOAT  = float;     	 // 4 bytes
using pxDOUBLE = double;     	 // 8 bytes

using pxPVOID  = void*;	 		 // 8 bytes