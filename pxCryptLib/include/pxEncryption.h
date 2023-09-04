#pragma once


class Encryption {
public:
	virtual ~Encryption() = default;
	virtual const char* encode(const char*) = 0;
	virtual const char* decode(const char*)=0;

};
#include <stdlib.h>




class ceaserEncryption : public Encryption {
public:
	ceaserEncryption(int value):key(value) {
		
	}
	const char* encode(const char* msg) override {
		const size_t szmsg = strlen(msg);
		char* emsg = new char[szmsg + 1];
		for (size_t i = 0; i < szmsg; i++) {
			emsg[i] = encodeByte(msg[i]);
		}
		emsg[szmsg] = '\0';
		return emsg;

	}
	const char* decode(const char* msg) override {
		const size_t szmsg = strlen(msg);
		char* emsg = new char[szmsg + 1];
		for (size_t i = 0; i < szmsg; i++) {
			emsg[i] = decodeByte(msg[i]);
		}
		emsg[szmsg] = '\0';
		return emsg;

	}

private:
	static inline constexpr int uA = 'A';
	static inline constexpr int uZ = 'Z';
	static inline constexpr int lA = 'a';
	static inline constexpr int lZ = 'z';
	static inline constexpr int rmin = lA;
	static inline constexpr int rmax = lZ;
	static inline constexpr int range = rmax - rmin + 1;

	char encodeByte(int byte) {
		// check if x is in range [a-z or A-Z]
		if (!((byte >= lA && byte <= lZ) || (byte >= uA && byte <= uZ))) return byte;


		const char offset = 32 * (byte >= uA && byte <= uZ);
		byte += offset;
		if (!((byte + key) >= rmin && (byte + key) <= rmax)) {
			if ((byte + key) >= rmin) {
				byte -= range;
			}
			else {
				byte += range;
			}
		}
		return (char)((byte + key % range) - offset);
		// convert to lowercase ( if is uppercase )
		if(byte >= uA && byte <= uZ) {
			byte += 32;
		}
		bool isUpper = false;
		byte = (isUpper = byte >= uA && byte <= uZ) == 1 ? byte + 32 : byte;
		byte = byte + [&,y = byte + key] {return (char)(y >= rmin && y <= rmax ? 0 : y > rmax ? -range : range); }();
		return [&](auto x, auto n)->char {return (char)(x + n % range); }(byte, key) + (isUpper ? -32 : 0);
	}


	char decodeByte(int byte) {
		// check if x is in range [a-z or A-Z]
		if (!((byte >= lA && byte <= lZ) || (byte >= uA && byte <= uZ))) return byte;

		const char offset = 32 * (byte >= uA && byte <= uZ);
		byte += offset;
		if (!((byte - key) >= rmin && (byte - key) <= rmax)) {
			if ((byte - key) >= rmin) {
				byte -= range;
			}
			else {
				byte += range;
			}
		}
		return (char)((byte - key % range) - offset);

		// convert to lowercase ( if is uppercase )
		bool isUpper = false;
		byte = (isUpper = byte >= uA && byte <= uZ) == 1 ? byte + 32 : byte;
		byte = byte + [&, y = byte - key] {return (char)(y >= rmin && y <= rmax ? 0 : y > rmax ? -range : range); }();
		return [&](auto x, auto n)->char {return (char)(x - n % range); }(byte, key) + (isUpper ? -32 : 0);

	}

	int key;
};

template<typename Encoder>
unsigned int Encode(const char* msg, void* metrics = 0) {
	Encoder encoder = Encoder();
	unsigned int result = encoder.encode(msg);
	return result;
}

template<typename Decoder>
unsigned int Decode(const char* msg, void* metrics = 0) {
	Decoder decoder = Decoder();
	unsigned int result = decoder.encode(msg);
	return result;
}
