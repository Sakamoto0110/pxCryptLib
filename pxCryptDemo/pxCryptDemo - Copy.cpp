#include <iostream>
#include "pxCrypt.h"
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







#include <stack>
#include "../pxConsoleHelper.h"
#include <Windows.h>





void gotoxy(short a, short b) //Custom gotoxy() function
{
	COORD coordinates; //Data type of co-ordinates
	coordinates.X = a; //Assign value to X- Co-ordinate
	coordinates.Y = b; //Assign value to Y Co-ordinate

	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), coordinates);

}

//static constexpr char null_char = (char)0xB0;

#include "visualizer.h"

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

//
// 
// 
// start -> request_input
//				   |	
//				   +------> parse_input
// 
// mk_block <size> <-args> -- make a string block
//				      |
//					  +-> -e   makes an empty block
//					  +-> -nc  disable color mapping
//		 			  +-> -s   sets a string as content
//
// shr <index> <dir> -- shifts a row 
// shc <index> <dir> -- shifts a col
namespace exemples {
	constexpr const char* STRING_SAMPLE_10x10 = "T316LTQN6RG37UAIFZN31QJT3QIWTAM9CW8D77HODJEHMU7ESF0OMQP042OGHHO3WIH2NY62AUPXJDIVDR0GCLN0LEVLT8GV3TZ";
	constexpr const char* STRING_SAMPLE_5x5 = "QCBLQR9ZUMONVLMJGC13ARJCX";


	void exemple_1_10x10() {
		StringBlock sb(10, 10);
		auto colors = Visualizer::ColorMap(10, 10);
		
		sb.SetColorMap(colors);
		sb.SetContent(STRING_SAMPLE_10x10);

		sb.Print();
		auto s = 10;
		gotoxy(0, s + 2);
		sb.Print();
		gotoxy(0, s * 2 + 2);
		printf("\x1b[0m");
		for (int i = 0; i < 1000; i++) {
		
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


#define printl std::cout << '\n'
constexpr const char* STRING_SAMPLE_4x4 = "ABGHTKOLIMJNHGL";
constexpr const char* STRING_SAMPLE_5x5 = "QCBLPR9ZUMONVLMJGC13ARJCX";
constexpr const char* STRING_SAMPLE_3x3 = "123456789";
constexpr const char* STRING_SAMPLE_10x10 = "T316LTQN6RG37UAIFZN31QJT3QIWTAM9CW8D77HODJEHMU7ESF0OMQP042OGHHO3WIH2NY62AUPXJDIVDR0GCLN0LEVLT8GV3TZ";





template<int sz>
void Encrypt(const char* plaintext, const char* key) {
	int szBlock = sz * sz;
	int szStr = strlen(plaintext);

	int nBlocks = szStr > szBlock ? (szStr / szBlock)+1 : 1;

	printf("plaintext: %s\n",plaintext);
	printf("plaintext size: %i\n",szStr);
	printf("block size: %i\n",szBlock);
	printf("number of blocks: %i\n",nBlocks);
	std::string str = std::string(plaintext);
	auto chf = [](const StringBlockHandle_t& dat, auto idx) {return dat.data[idx].ch; };
	auto cf = [](const StringBlockHandle_t& dat, auto idx) {return dat.data[idx].color; };
	// Create blocks
	StringBlock** blocks = new StringBlock*[nBlocks];
	auto colors = Visualizer::ColorMap(10,10);
	
	for (int i = 0; i < nBlocks; i++) {
		blocks[i] = new StringBlock(sz, sz);
		
		blocks[i]->SetColorMap(Visualizer::ColorMap(sz,sz, Visualizer::DEFAULT_GRAD_2D));
		blocks[i]->SetContent(str.substr(i * szBlock, szBlock).c_str());
		blocks[i]->Print(3 + (sz + 1) * i, 6, 1);
	}
	

	// Apply key
	for (int i = 0; i < nBlocks; i++) {
		for (int j = 0; j < blocks[i]->Width(); j++) {
			blocks[i]->ShiftCol(j, 'a'-key[j]);
		}
		blocks[i]->Print(3 + (sz + 1) * i, 12,1);
	}

	for (int i = 0; i < nBlocks; i++) {
		for (int j = 0; j < blocks[i]->Width(); j++) {
			blocks[i]->ShiftCol(j, ('a' - key[nBlocks - 1 - j])*-1);
		}
		blocks[i]->Print(3 + (sz + 1) * i, 18, 1);
	}

}

template<int sz>
StringBlock MakeColoredStringBlock() {
	const char* str = "";
	switch (sz){
	case 3:
		str = STRING_SAMPLE_3x3;
		break;
	case 4:
		str = STRING_SAMPLE_4x4;
		break;
	case 5:
		str = STRING_SAMPLE_5x5;
		break;
	case 10:
		str = STRING_SAMPLE_10x10;
		break;
	}
	StringBlock sb(sz,sz);
	auto colors = Visualizer::ColorMap(sz, sz, Visualizer::DEFAULT_GRAD_2D);
	//auto colors = _ColorMap(sz,sz);
	sb.SetColorMap(colors);
	sb.SetContent(str);
	if(str == "")sb.FillWith(' ');
	//
	return sb;
}

union uColor {
	uint32_t value;
	struct {
		uint8_t a;
		uint8_t r ;
		uint8_t g ;
		uint8_t b ;
		
	};

};

std::string* SplitStrings(const char* _str, int n, int sz) {
	std::string str = std::string(_str);
	std::string* result = new std::string[n];
	for (int i = 0; i < n; i++) {
		result[i] = std::string(str.substr(i * sz, sz));
	}
	return result;
}


template<int width,int height>
void Encrypt2(const char* plaintext, const char* key) {	
	// setup
	int szBlock = width * height;
	int szStr = strlen(plaintext);
	int nBlocks = szStr > szBlock ? (szStr / szBlock) + 1 : 1;
	printf("plaintext: %s\n", plaintext);
	printf("plaintext size: %i\n", szStr);
	printf("block size: %i\n", szBlock);
	printf("number of blocks: %i\n", nBlocks);

	// * * * * * * * * * * * * * * * * * * * * *
	// Create blocks
	std::string* str = SplitStrings(plaintext, nBlocks, szBlock);
	StringBlock** blocks = new StringBlock * [nBlocks];
	for (int i = 0; i < nBlocks; i++) {
		blocks[i] = new StringBlock(width, height);
		blocks[i]->SetContent(str[i].c_str());
		blocks[i]->SetColorMap(Visualizer::ColorMap(width,height, Visualizer::DEFAULT_GRAD_2D));
		
	}
	
	auto fn_showgrid = [](int y, int n, int w, int h, StringBlock** _blocks) {
		for (int i = 0; i < n; i++) {
			int _x = (w * (i * 2) + 1 + (i * w));
			int _y = y;
			_blocks[i]->Print(_x, _y, 1);
			vtCursor<move>(1, _y + h + 1);
		}
	};
	// show grid
	fn_showgrid(5, nBlocks, width, height, blocks);

	
	
	// apply key
	for (int i = 0; i < nBlocks; i++) {
		for (int j = 0; j < strlen(key); j++) {
			int ikey = key[j];
			blocks[i]->ShiftCol(j, ikey);
		}
		
	}
	fn_showgrid(9, nBlocks, width, height, blocks);
	
	
	// reverse key
	for (int i = 0; i < nBlocks; i++) {
		for (int j = 0; j < strlen(key); j++) {
			int ikey = key[j];
			blocks[i]->ShiftCol(j, -ikey);
		}
	}
	fn_showgrid(13, nBlocks, width, height, blocks);
	printf("\n");
	for (int i = 0; i < nBlocks; i++) {
		std::cout << blocks[i]->ToString();
	}
	printf("\n");
	/*delete(str);
	delete(blocks);*/
}

int main()
{
	
	ShowConsoleCursor(false);
	Encrypt2<2,2>("somedatalongtwo", "cgra");













	//system("pause");
	//exemples::exemple_1_10x10();
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


