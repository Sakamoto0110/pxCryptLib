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


#include "pxEncryption.h"


struct ExprNode {
	ExprNode* a = 0;
	ExprNode* b = 0;
	char op=0;
	long long value=0;

	ExprNode(){}
	ExprNode(long long val):value(val){}

	long long Eval() {
		if (op == '+') {
			if (a != 0 && b != 0) {
				return a->Eval() + b->Eval();
			}

		}
		if (a != 0 && b == 0)
			return a->Eval();
		return value;
	}
};
template<size_t N>
struct StringLiteral {
	constexpr StringLiteral(const char(&str)[N]) {
		for (int i = 0; i < N; i++)
			value[i] = str[i];
	}

	char value[N];
};



template<StringLiteral signature>
class vtObject_t {
public:
	static constexpr const char* _signature = signature.value;
};
#define VTC_CURPOS "\x1b[%i;%iH"
struct vtPosition :  vtObject_t<VTC_CURPOS> {
	int x;
	int y;
	vtPosition(int x, int y) : x(x),y(y) {}
	friend std::ostream& operator <<(std::ostream& out, const vtPosition& p);

};

std::ostream& operator <<(std::ostream& out, const vtPosition& p) {
	printf(p._signature, p.x, p.y);
	return out;
}

#include <stack>
#include "../pxConsoleHelper.h"
#include <Windows.h>

template <typename T>
T lerp(const T& a, const T& b, float t) {
	return a + t * (b - a);
}
#include <array>



template<size_t sz>
constexpr std::array<int32_t,sz> MakeColorMap(auto min, auto max) {
	auto cmap= std::array<int32_t, sz>();
	for (size_t i = 0; i < sz; i++) {
		const float t = ((float)(1+i) * ((float)sz / 100));
		const float t1 = 1 - t;
		float r = lerp(min, max, t);
		float b = lerp(min, max, t1);
		cmap[i] = Color_RGB(r, 0, b);

	}
	return cmap;
}



void gotoxy(short a, short b) //Custom gotoxy() function
{
	COORD coordinates; //Data type of co-ordinates
	coordinates.X = a; //Assign value to X- Co-ordinate
	coordinates.Y = b; //Assign value to Y Co-ordinate

	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), coordinates);

}

//static constexpr char null_char = (char)0xB0;
#include "StringBlock.h"
class StringBlockHandleIterator {
public:
	StringBlockHandle_t& handle;
	size_t pos = 0;

	decltype(*handle.data)& Next() {
		return handle.data[(pos++%handle.size)];
	}

	decltype(handle.data) begin() {
		return &handle.data[0];
	}
	decltype(handle.data) end() {
		return &handle.data[handle.size];
	}
	StringBlockHandleIterator(StringBlockHandle_t& _handle) :handle(_handle) {
	}
};

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

// Function to interpolate between two colors
// r1, g1, b1: Color 1 (e.g., red)
// r2, g2, b2: Color 2 (e.g., blue)
// t: Interpolation parameter (0.0 to 1.0)
// Returns the interpolated color
int interpolateColor(int r1, int g1, int b1, int r2, int g2, int b2, double t) {
	int r = static_cast<int>((1.0 - t) * r1 + t * r2);
	int g = static_cast<int>((1.0 - t) * g1 + t * g2);
	int b = static_cast<int>((1.0 - t) * b1 + t * b2);
	return (r << 16) | (g << 8) | b;
}

#include <vector>
// Function provided by chat gpt 3.5
std::vector<int32_t> _ColorMap(auto width, auto height) {
	auto r = std::vector<int32_t>();
	// Define the minimum and maximum brightness values
	const double MIN_BRIGHTNESS = 0.5;
	const double MAX_BRIGHTNESS = 1.0;

	// Generate the color map
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			// Calculate the brightness (y-axis)
			double brightness = MIN_BRIGHTNESS + (MAX_BRIGHTNESS - MIN_BRIGHTNESS) * (static_cast<double>(y) / height);
			
			// Interpolate between red (255, 0, 0) and blue (0, 0, 255) for the x-axis
			int color = interpolateColor(255, 0, 0, 0, 0, 255, static_cast<double>(x) / width);

			// Adjust brightness
			int adjustedR = static_cast<int>(brightness * (color >> 16 & 255));
			int adjustedG = static_cast<int>(brightness * (color >> 8 & 255));
			int adjustedB = static_cast<int>(brightness * (color & 255));
			r.push_back(Color_RGB(adjustedR, adjustedG, adjustedB));
		}
	}
	
	return r;

}
namespace exemples {
	constexpr const char* STRING_SAMPLE_10x10 = "T316LTQN6RG37UAIFZN31QJT3QIWTAM9CW8D77HODJEHMU7ESF0OMQP042OGHHO3WIH2NY62AUPXJDIVDR0GCLN0LEVLT8GV3TZ";
	constexpr const char* STRING_SAMPLE_5x5 = "QCBLQR9ZUMONVLMJGC13ARJCX";


	void exemple_1_10x10() {
		StringBlock sb(10, 10, STRING_SAMPLE_10x10);
		sb.Fill_null_spaces(' ');
		auto colors = _ColorMap(10, 10);
		
		for (int i = 0; i < 100; i++) {
			sb.GetHandle()[i].color = colors[(99 - i)%100];
		}


		sb.Print();
		auto s = 10;
		gotoxy(0, s + 2);
		sb.Print();
		gotoxy(0, s * 2 + 2);
		printf("\x1b[0m");
		for (int i = 0; i < 1000; i++) {
			/*for (int j = 0; j < 10; j++) {
				sb.ShiftRow(j, 1);
				sb.ShiftCol((j ), 1);
			}*/
			
			
			auto i1 = (rand()) % s;
			if (rand() % 2 == 0) {
				sb.ShiftRow(i1, 1);
			}
			else {
				sb.ShiftCol(i1, 1);
			}


			gotoxy(0, 0);
			sb.Print();
			Sleep(100);
			
		}



	}
}

//void ____DWORDTESTING() {
//	constexpr auto _byte = 0xFF;
//	constexpr auto _word = 0xFFFF;
//	{
//		constexpr auto b1 = _dwByte<1>();
//		constexpr auto b2 = _dwByte<2>();
//		constexpr auto b3 = _dwByte<3>();
//		constexpr auto b4 = _dwByte<4>();
//
//		constexpr auto dword1 = 0;
//		constexpr auto dword2 = 0;
//		constexpr auto dword3 = 0;
//		constexpr auto dword4 = 0;
//
//		constexpr auto dword_ = _4byteDWORD(0,0, _byte, _byte);
//
//		constexpr auto dword = _4byteDWORD(_byte, _byte, 0, 0);
//
//		constexpr auto _dword1 = _dwByte<3>(dword, 0xFF);
//
//		constexpr auto _b1 = _dwByte<1>(_dword1);
//		constexpr auto _b2 = _dwByte<2>(_dword1);
//		constexpr auto _b3 = _dwByte<3>(_dword1);
//		constexpr auto _b4 = _dwByte<4>(_dword1);
//
//
//
//
//		constexpr auto _dword = _dw4Words(0xFFFF, 0xFFFF);
//
//
//	}
//	
//
//	{
//		constexpr auto w1 = _dwWord<1>();
//		constexpr auto w2 = _dwWord<2>();
//
//		constexpr auto dword1 = 0;
//
//		constexpr auto dword2 = _dwWord<1>(dword1, _word);
//		constexpr auto dword3 = _dwWord<2>(dword2, _word);
//
//		constexpr auto _d = _dwWord<1>(dword3);
//		constexpr auto _d1 = _dwWord<2>(dword3);
//
//		
//	}
//
//}
#define printl std::cout << '\n'
constexpr const char* STRING_SAMPLE_5x5 = "QCBLPR9ZUMONVLMJGC13ARJCX";
constexpr const char* STRING_SAMPLE_3x3 = "123456789";
int main()
{
	ShowConsoleCursor(false);
	exemples::exemple_1_10x10();
	StringBlock sb(5, 5, STRING_SAMPLE_5x5);
	sb.Print();
	printl;
	size_t path[9] = { 5,4,4,3,3,2,2,1,1 };
	
	StringBlock::Iter it = sb.GetRowIterator(0);
	
	std::cout << ">" << it.operator StringBlockHandle_t::_charEx_t & ().ch << " \n";
	it += {0, 1};
	std::cout << ">" << it.operator StringBlockHandle_t::_charEx_t &().ch << " \n";
	it += {0, 1};
	std::cout << ">" << it.operator StringBlockHandle_t::_charEx_t & ().ch << " \n";
	it += {0, 1};
	std::cout << ">" << it.operator StringBlockHandle_t::_charEx_t & ().ch << " \n";
	it += {0, 1};
	std::cout << ">" << it.operator StringBlockHandle_t::_charEx_t & ().ch << " \n";
	it += {0, 1};
	std::cout << ">" << it.operator StringBlockHandle_t::_charEx_t & ().ch << " \n";
	printl;
	/*for (auto v = it.begin(); v != it.end(); v = it.Next()) {
		printf("v[%c] end[%c]\n", (it.Actual()).ch, (*it.end()).ch);
	}
	printf("\n\nv[%c] \n", (it.Actual()).ch);*/
	
	printf("\n");
	printf("\x1b[0m");
	system("pause");
	/*PopulateStringBlock(handle, "123456789");
	std::cout << "size: " << GetSize(handle.cols[0].cols[0]) << '\n';
	std::cout << "width: " << GetWidth(handle.cols[0].cols[0]) << '\n';
	std::cout << "height: " << GetHeight(handle.cols[0].cols[0]) << '\n';
	printf("\n");*/
	
//	sb.Print2D();

	
	//auto b = MakeColorMap<10>(0, 125);
	//for (size_t y = 0; y < 10; y++) {
	//	std::array < int32_t, 10> colors = MakeColorMap<10>(125, 255);
	//	for (size_t x = 0; x < 10; x++) {
	//		
	//		//std::cout << colors[y][x] << ',';
	//		printf("\x1b[48;2;%i;%i;%im ", GetRed(colors[x]- b[y]), 0, GetBlue(colors[x]-b[y]));
	//	}
	//	printf("\n");
	//}
	//
	//
	//
	//StringBlockHandler sb(3, 3);
	//
	//

	//sb.SetContent("123456789");
	//sb.hRows[0].pcBlock[0].color = Color_RGB(125,0,0);
	//sb.hRows[1].pcBlock[0].color = Color_RGB(125, 0, 0);
	//sb.hRows[2].pcBlock[0].color = Color_RGB(125, 0, 0);

	//sb.hRows[0].pcBlock[1].color = Color_RGB(0, 0, 125);
	//sb.hRows[1].pcBlock[1].color = Color_RGB(0, 0, 125);
	//sb.hRows[2].pcBlock[1].color = Color_RGB(0, 0, 125);

	//sb.hRows[0].pcBlock[2].color = Color_RGB(0, 125, 0);
	//sb.hRows[1].pcBlock[2].color = Color_RGB(0, 125, 0);
	//sb.hRows[2].pcBlock[2].color = Color_RGB(0, 125, 0);
	////Sleep(3000);
	//sb.Print2D();
	//
	//printf("\x1b[0m");
	return 0;
	constexpr int size = 7;
	char str[8] = "1+3+6+6";
	std::stack<ExprNode*> stk{};
	for (int i = 0; i < size-1; i++) {
		if (str[i] == '+') {
			ExprNode* op = new ExprNode();
			if (stk.size() > 0) {
				ExprNode* last = stk.top();
				if (last != 0) {
					
					op->a = last;
				}
			}
			else {
				if (str[i - 1] == '1') {
					op->a = new ExprNode(1);
				}
				if (str[i - 1] == '3') {
					op->a = new ExprNode(3);
				}
				if (str[i - 1] == '6') {
					op->a = new ExprNode(6);
				}
			}
			if (str[i + 1] == '1') {
				op->b = new ExprNode(1);
			}
			if (str[i + 1] == '3') {
				op->b = new ExprNode(3);
			}
			if (str[i+1] == '6') {
				op->b = new ExprNode(6);
			}
			op->op = '+';
			stk.push(op);
		}
		
		
		
	}
	for (int i = 0; i < stk.size(); i++) {
		auto expr = stk.top();
		std::cout << expr->Eval() << std::endl;
		stk.pop();
	}
	/*
	ExprNode a = 10;
	ExprNode b = 5;

	ExprNode e;
	e.a = &a;
	e.b = &b;
	e.op = '+';*/
	
    return 0;
}


