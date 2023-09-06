#include <iostream>







#include "pxCrypt.h"




template<typename T, size_t sz, bool ShouldPreAllocate = true>
struct CallbackArray {
	CallbackArray() {
		for (size_t i = 0; i < sz; i++) {
			if (ShouldPreAllocate)
				_data[i] = new T;
			_data[i] = 0;
		}
	}

	constexpr size_t Count() {
		size_t size = 0;
		for (size_t i = 0; i < sz; i++)
			if (_data[(size = i)] == 0)
				break;
		return size;
	}

	void Remove(size_t index) {
		if (_tryToDealloc(index) && !_isContigous()) {
			_removeGaps();
		}
	}

	void Insert(size_t index, const T& dat) {
		if (_tryToAlloc(index))
			*_data[index] = dat;
	}

	void operator +=(const T& dat) {
		for (size_t i = 0; i < sz; i++) {
			if (!_isAllocated(i)) {
				this->Insert(i, dat);
				return;
			}
		}
	}

	T operator[](size_t index) {
		return *_data[index];
	}
private:
	constexpr bool _isAllocated(size_t index) {
		return _data[index] != 0;
	}
	constexpr bool _isContigous() {
		for (size_t i = 0; i < sz; i++) {
			if (_data[i] == 0) {
				for (size_t j = i; j < sz; j++) {
					if (_data[j] != 0)
						return false;
				}
			}
		}
		return true;
	}
	bool _tryToDealloc(size_t index) {
		if (_data[index] != 0) {
			delete	_data[index];
			return true;
		}
		return false;
	}
	bool _tryToAlloc(size_t index) {
		if (_data[index] == 0) {
			_data[index] = new T;
			return true;
		}
		return false;
	}

	void _removeGaps() {
		for (size_t i = 0; i < sz - 1; i++) {
			if (_data[i] == 0 && _data[i + 1] != 0) {
				_data[i] = _data[i + 1];
				_data[i + 1] = 0;
			}
		}
	}

	T* _data[sz];
};


struct Foo {
	Foo(const char* str) {
		std::cout << strlen(str) << '\n';
	}
};

template<typename T, typename...Tx>
class ICallable {
public:
	virtual T operator()(Tx...args) const = 0;
	virtual bool operator==(ICallable*) const = 0;
};

template<typename T, typename...Tx>
class Callable  {
public:
	static constexpr size_t ARGS_COUNT = sizeof...(Tx);
	inline static const char* RETURNTYPE = typeid(T).name();
	struct {
		const char* operator[](size_t index) const noexcept {
			return index < 0 || index >= ARGS_COUNT ? "" : _args[index];
		}
	private:
		const char* _args[sizeof...(Tx)] = { (typeid(Tx).name()) ... };
	}inline static ARGS;

	

	
	T Invoke(Tx...args) const {
		if constexpr (std::is_void<T>()) {
			BeforeInvokeHook();
			(*this)(args...);
			AfterInvokeHook();
			return;
		}
		else {
			BeforeInvokeHook();
			T t = (*this)(args...);
			AfterInvokeHook();
			return t;
		}
	}

	

protected:
	
	virtual void BeforeInvokeHook() {
		/* Override this if needed */
	}
	virtual void AfterInvokeHook() {
		/* Override this if needed */
	}
};


template<typename T, typename...Tx>
class Function : public Callable<T,Tx...>, ICallable<T, Tx...> {
public:
	Function(T(*_fn)(Tx...)) :fn(_fn) {
		
	}
	T operator()(Tx...args) const override {
		return fn(args...);
	}
	bool operator==(ICallable<T, Tx...>* other) const override {
		return fn == ((Function*)other)->fn;
	}

	void BeforeInvokeHook() override {
		
	}

private:
	T(*fn)(Tx...);
};

template<typename...Tx>
class Action : public ICallable<void, Tx...>,Callable<void, Tx...>  {
public:
	Action(void(*_fn)(Tx...)) :fn(_fn) {

	}
	void operator()(Tx...args) const override {
		fn(args...);
	}
	bool operator==(ICallable<void, Tx...>* other) const override {
		return fn == ((Action*)other)->fn;
	}

private:
	void(*fn)(Tx...);
};

void f(int x, int y) {
	
}

//struct Parameter {
//	const char* name = "unnamed";
//	unsigned int wFlags;
//	union {
//		int i = 0;
//		float f = 0;
//		const char* str=0;
//
//	}data;
//	explicit Parameter(int val) {
//		data.i = val;
//		wFlags = 2;
//	}
//	explicit Parameter(float val) {
//		data.f = val;
//		wFlags = 4;
//	}
//	explicit Parameter(const char* val) {
//		data.str = val;
//		wFlags = 8;
//	}
//
//	Parameter& operator =(int val) { data.i = val; return *this; }
//	Parameter& operator =(float val) { data.f = val; return *this; }
//	Parameter& operator =(const char* val) { data.str = val; return *this; }
//
//	operator int()const { return data.i; }
//	operator float()const { return data.f; }
//	operator const char*()const { return data.str; }
//
//};
// encode [inline commands[cmd [arg:type]]...] [msg:str]
//struct Command {
//	const char* ids;
//	const char* tags;
//	const char* name;
//	const char* description;
//	unsigned int wFlags;
//	struct ParameterInfo {
//		const char* name;
//		const char* description;
//		const char* type;
//		unsigned int wFlags;
//	}params[16];
//	pxDWORD HCALLBACK;
//	class _Callable* func;
//};
//
//inline static Command* cmd = new Command
//{
//	"t","none","cmd","A command", 0,
//	{
//		{
//			"arg0",
//					  "aaaa",
//					  "void",
//					  0
//		}
//	},
//	0
//};

constexpr char encodeByte(int b, int k) {
	constexpr int uA = 'A';
	constexpr int uZ = 'Z';
	constexpr int lA = 'a';
	constexpr int lZ = 'z';
	constexpr int rmin = lA;
	constexpr int rmax = lZ;
	constexpr int range = rmax - rmin + 1;
	// check if x is in range [a-z or A-Z]
	if (!((b >= lA && b <= lZ) || (b >= uA && b <= uZ))) return b;

	/*int offset = 32 * (b >= uA && b <= uZ);
	b += offset;
	int t = b + k;
	b += t >= rmin && t <= rmax ? 0 : t > rmax ? -range : range;
	return (b + k % range) - offset;*/
	const char offset = 32 * (b >= uA && b <= uZ);
	b += offset;
	if (!((b + k) >= rmin && (b + k) <= rmax)) {
		if ((b + k) >= rmin) {
			b -= range;
		}
		else {
			b += range;
		}
	}
	return (char)((b + k % range) - offset);
	// convert to lowercase ( if is uppercase )
	bool isUpper = false;
	b = (isUpper = b >= uA && b <= uZ )==1? b + 32 : b;
	b = b + [y = b + k] {return (char)(y >= rmin && y <= rmax ? 0 : y > rmax ? -range : range); }();
	return [](auto x, auto n)->char {return (char)(x + n % range); }(b, k)+(isUpper?-32:0);
}


constexpr char decodeByte(int b, int k) {
	constexpr char uA = 'A';
	constexpr char uZ = 'Z';
	constexpr char lA = 'a';
	constexpr char lZ = 'z';
	constexpr char rmin = lA;
	constexpr char rmax = lZ;
	constexpr char range = rmax - rmin + 1;
	// check if x is in range [a-z or A-Z]
	if (!((b >= lA && b <= lZ) || (b >= uA && b <= uZ))) return b;

	const char offset = 32 * (b >= uA && b <= uZ);
	b += offset;
	if(!((b - k) >= rmin && (b - k) <= rmax)) {
		if((b - k) >= rmin) {
			b -= range;
		}else {
			b += range;
		}
	}
	return (char)((b-k % range) - offset);
	// convert to lowercase ( if is uppercase )
	bool isUpper = false;
	b = (isUpper = b >= uA && b <= uZ) == 1 ? b + 32 : b;
	b = b + [y = b - k] {return (char)(y >= rmin && y <= rmax ? 0 : y > rmax ? -range : range); }();
	return [](auto x, auto n)->char {return (char)(x - n % range); }(b, k) + (isUpper ? -32 : 0);
	
}

const char* encodeString(const char* msg, int key) {
	const size_t szmsg = strlen(msg);
	char* emsg = new char[szmsg+1];
	for(size_t i = 0; i < szmsg; i++) {
		emsg[i] = encodeByte(msg[i], key);
	}
	emsg[szmsg] = '\0';
	return emsg;

}

const char* decodeString(const char* msg, int key) {
	const size_t szmsg = strlen(msg);
	char* emsg = new char[szmsg + 1];
	for (size_t i = 0; i < szmsg; i++) {
		emsg[i] = decodeByte(msg[i], key);
	}
	emsg[szmsg] = '\0';
	return emsg;
}

constexpr pxDWORD GetRValue(pxDWORD dw) {
	return  dw ^ ( (0b1111111111111111111111111111111100000000000000000000000000000000));
}
constexpr pxDWORD GetLValue(pxDWORD dw) {
	return dw & (dw >> 32);
}

constexpr pxDWORD SetRValue(pxDWORD dw, pxWORD value) {
	return dw | value;
}
constexpr pxDWORD SetLValue(pxDWORD dw, pxDWORD value) {
	return dw | (value << 32);
}

//pxDWORD SetRValue(pxDWORD& dw, pxWORD value) {
//	return dw |= value;
//}
//pxDWORD SetLValue(pxDWORD& dw, pxWORD value) {
//	return dw |= (value << 15);
//}

#include "pxEncryption.h"
//#include <Windows.h>
//void gotoxy(int x, int y) {
//	COORD c;
//	c.X = x;
//	c.Y = y;
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
//}
int main()
{
	StringBlockHandle block(3, 3, "abcd");
	/*for (pxI32 y = 0; y < block.szHeight; y++) {
		for (pxI32 x = 0; x < block.szWidth; x++) {

			pxI32 index = x + y * block.szWidth;
			printf("%c", block.pBuffer[index]);
		}
		printf("\n");
	}*/
	block.Print2D();
	std::cout << block << ".\n";
	/*sb16 block("abcde");
	std::cout << block << ".\n";

	block.Shift(block.hsb->strBlock, 2, 2, 5, block.hsb->szContent, block.hsb->szBlock, 0);
	std::cout << block << ".\n";*/

	//std::cout << (char*)((void*)&block) << '\n';
    return 0;
}

