// 01_push_box.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <variant>

using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

// sum type in C++ https://www.ida.liu.se/~TDDD38/lecture/slides/sum_types.pdf
enum class Object {
	SPACE,
	WALL,
	GOAL,
	BLOCK,
	BLOCK_ON_GOAL,
	MAN,
	MAN_ON_GOAL,
	UNKNOWN,
};

// 函数原型
void initialize(Object state[], int w, int h, const char stageData[]);
void draw(const Object state[], int w, int h);
void update(Object state[], char input, int w, int h);
bool checkClear(const Object state[], int w, int h);

// main function
int main() {
	Object *state = new Object[ gStageWidth * gStageHeight ];
	initialize(state, gStageWidth, gStageHeight, gStageData);
	
	while (true) {
		// draw first
		draw(state, gStageWidth, gStageHeight);

		// check if met with the game set condition
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break;
		}

		// get input
		cout << "a: left d: right w: up s: down. command?" << endl;
		char input;
		cin >> input;

		// update
		update(state, input, gStageWidth, gStageHeight);
	}

	cout << "Congratulation's you win。" << endl;
	delete[] state;
	state = nullptr;

	return 0;
}

void initialize(Object state[], int width, int height, const char stageData[]) {
	const char* d = stageData;
	int x = 0;
	int y = 0;
	while (*d != '\0') {
		Object t;
		switch ( *d )
		{
		case '#': t = Object::WALL; break;
		case ' ': t = Object::SPACE; break;
		case 'o': t = Object::BLOCK; break;
		case 'O': t = Object::BLOCK_ON_GOAL; break;
		case '.': t = Object::GOAL; break;
		case 'p': t = Object::MAN; break;
		case 'P': t = Object::MAN_ON_GOAL; break;
		case '\n':	// next line
			x = 0;
			++y;
			t = Object::UNKNOWN;					// no data, would be emitted
			break;
		default:
			t = Object::UNKNOWN; break;				// illegal data
			break;
		}
		++d;										// iterate to next stageData

		if (t != Object::UNKNOWN) {
			state[y * width + x] = t;				// write in
			++x;
		}
	}
}

void draw(const Object state[], int width, int height) {
	// the sequence of enum class Object
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Object o = state[y * width + x];
			cout << font[static_cast<int>(o)];
		}
		cout << endl;
	}

}

void update(Object state[], char input, int w, int h) {
	int dx = 0;
	int dy = 0;
	
	switch (input) {
		case 'a': dx = -1; break;
		case 'd': dx = 1; break;
		case 'w': dy = -1; break;
		case 's': dy = 1; break;
	}

	// search for player's position
	int i = -1;
	for (i = 0; i < w * h; ++i) {
		if (state[i] == Object::MAN || state[i] == Object::MAN_ON_GOAL) {
			break;
		}
	}

	int x = i % w;
	int y = i / w;

	// 移动后的坐标
	int tx = x + dx;
	int ty = y + dy;

	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}


	int p = y * w + x;							// player position
	int tp = ty + w + tx;						// target position
	// A. 要移动的地方是空白或者目的地，玩家移动
	if (state[tp] == Object::SPACE || state[tp] == Object::GOAL) {
		state[tp] = (state[tp] == Object::GOAL ? Object::MAN_ON_GOAL : Object::MAN);
		state[p] = (state[p] == Object::MAN_ON_GOAL ? Object::GOAL : Object::SPACE);
	} 
	// B. 要移动到的位置有箱子；如果沿该方向的下一个网格是空白、或者是目的地，那么就移动
	else {
		// 沿着该方向的第二个网格的位置，是否在允许的范围之内
		int tx2 = tx + dx;
		int ty2 = ty + dy;

		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
			return;			// cannot push
		}
		int tp2 = ty2 * w + tx2;
		if (state[tp2] == Object::SPACE || state[tp2] == Object::GOAL) {
			// 逐个替换
			state[tp2] = (state[tp2] == Object::GOAL ? Object::BLOCK_ON_GOAL : Object::BLOCK);
			state[tp] = (state[tp] == Object::BLOCK_ON_GOAL ? Object::MAN_ON_GOAL : Object::MAN);
			state[p] = (state[p] == Object::MAN_ON_GOAL ? Object::GOAL : Object::SPACE);
		}
	}


}

bool checkClear(const Object state[], int width, int height) {
	for (int i = 0; i < width * height; ++i) {
		if (state[i] == Object::BLOCK) {
			return false;
		}
	}
	return true;
}
