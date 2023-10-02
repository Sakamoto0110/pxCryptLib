#pragma once
#include <iostream>

#define _CPP_2020

#if _MSVC_LANG >= 202002L

template<size_t N>
struct StringLiteral {
	constexpr StringLiteral(const char(&str)[N]) {
		for (int i = 0; i < N; i++)
			value[i] = str[i];
	}

	char value[N];
};

template<StringLiteral signature>
struct vtObject  {
public:
	static constexpr const char* _signature = signature.value;
	

};
#define _vtObject(name, code)  name = vtObject<code> 



#else

#define _vtObject(name,code)					\
name = struct name##_t {									\
static constexpr const char* _signature = code; \
};											    \

#endif


// Aliases and signatures definitions

namespace vt {

	using _vtObject(vtPositionA, "\x1b[%i;%iH");
	using _vtObject(vtBackgroundColorA, "\x1b[48;2;%i;%i;%im");
	using _vtObject(vtForegroundColorA, "\x1b[38;2;%i;%i;%im");

	using _vtObject(vtCurMOVEA, "\x1b[%i;%iH");
	using _vtObject(vtCurUPA, "\x1b[%iA");
	using _vtObject(vtCurDOWNA, "\x1b[%iB");
	using _vtObject(vtCurRIGHTA, "\x1b[%iC");
	using _vtObject(vtCurLEFTA, "\x1b[%iD");
	using _vtObject(vtCurLDOWNA, "\x1b[%iE");
	using _vtObject(vtCurLUPA, "\x1b[%iF");
	using _vtObject(vtCurHABSA, "\x1b[%iG");
	using _vtObject(vtCurVABSA, "\x1b[%id");
	using _vtObject(vtCurSVA, "\x1b""7");
	using _vtObject(vtCurLDA, "\x1b""8");

}
// Type definitions
struct vtPosition : vt::vtPositionA {
	int x;
	int y;
	vtPosition(int x, int y) : x(x),y(y) {}
	friend std::ostream& operator <<(std::ostream& out, const vtPosition& p) ;
	
};
std::ostream& operator <<(std::ostream& out, const vtPosition& p) {
	printf(p._signature, p.x, p.y);
	return out;
}



struct vtBackgroundColor : vt::vtBackgroundColorA {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	
	vtBackgroundColor(unsigned int RRGGBBAA) :
		r((RRGGBBAA & 0xFF000000) >> 24),
		g((RRGGBBAA & 0x00FF0000) >> 16),
		b((RRGGBBAA & 0x0000FF00) >> 8) {}

	vtBackgroundColor(unsigned char r,unsigned char g, unsigned char b) :
		r(r),
		g(g),
		b(b) {}

	~vtBackgroundColor() {
		printf("\x1b[0m");
	}

	friend std::ostream& operator <<(std::ostream& out, const vtBackgroundColor& p);
};
std::ostream& operator <<(std::ostream& out, const vtBackgroundColor& p) {
	printf(p._signature, p.r, p.g, p.b);
	return out;
};



struct vtForegroundColor : vt::vtForegroundColorA {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	vtForegroundColor(unsigned int RRGGBBAA) :
		r((RRGGBBAA & 0xFF000000) >> 24),
		g((RRGGBBAA & 0x00FF0000) >> 16),
		b((RRGGBBAA & 0x0000FF00) >> 8) {}

	vtForegroundColor(unsigned char r, unsigned char g, unsigned char b) :
		r(r),
		g(g),
		b(b) {}

	~vtForegroundColor() {
		printf("\x1b[0m");
	}

	friend std::ostream& operator <<(std::ostream& out, const vtForegroundColor& p);
};
std::ostream& operator <<(std::ostream& out, const vtForegroundColor& p) {
	printf(p._signature, p.r, p.g, p.b);
	return out;
};


using move = vt::vtCurMOVEA;
using moveUp = vt::vtCurUPA;
using moveDown = vt::vtCurDOWNA;
using moveRight = vt::vtCurRIGHTA;
using moveLeft = vt::vtCurLEFTA;
using moveLineDown = vt::vtCurLDOWNA;
using moveLineUp = vt::vtCurLUPA;
using moveHabs = vt::vtCurHABSA;
using moveVabs = vt::vtCurVABSA;
using save = vt::vtCurSVA;
using load = vt::vtCurLDA;

template<typename T >
struct vtCursor {
	
	
	vtCursor(int v1 = 1) {
		if constexpr (T::_signature == vt::vtCurSVA::_signature || T::_signature == vt::vtCurLDA::_signature)
			printf(T::_signature);
		else
			printf(T::_signature, v1);
	}
	vtCursor(int v1, int v2) {
		if constexpr  (T::_signature == vt::vtCurMOVEA::_signature)
			printf(T::_signature, v2,v1);
			
	}
	
};

template<typename T >
struct vtLazyCursor {
	int v1 = 1;
	int v2 = 0;

	vtLazyCursor() {}
	vtLazyCursor(int val) :
	v1(val) {}
	vtLazyCursor(int v1, int v2):
	v1(v1),
	v2(v2) {}

	template<typename K>
	friend std::ostream& operator <<(std::ostream& out, const vtLazyCursor<K>& p);
};

template<typename T>
std::ostream& operator <<(std::ostream& out, const vtLazyCursor<T>& p) {
	if constexpr (T::_signature == vt::vtCurSVA::_signature || T::_signature == vt::vtCurLDA::_signature) 
		printf(T::_signature);
	else if constexpr (T::_signature != vt::vtCurMOVEA)
		printf(T::_signature, p.v1);
	else
		printf(T::_signature, p.v1,p.v2);
	return out;
}









namespace vtHelper {

	template<typename...T, typename...K>
	void format(K...args) {
		((std::cout << T(args)), ...);
	}

	template<typename...T, typename...K>
	void print(const char* str, K...args) {
		((std::cout << T()), ...);
		printf(str, args...);

	}

};




