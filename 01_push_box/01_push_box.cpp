#include <iostream>
using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 7;
const int gStageHeight = 4;

enum class Object {
	SPACE = -1,
	WALL,
	GOAL,
	BLOCK,
	BLOCK_ON_GOAL,
	MAN,
	MAN_ON_GOAL,
	UNKNOWN,
};

// 函数原型
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main() {
	Object* state = new Object[gStageWidth * gStageHeight];

	initialize(state, gStageWidth, gStageHeight, gStageData);

	while (true) {
		draw(state, gStageWidth, gStageHeight);
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break;
		}
		cout << "a: left, d: right, w: up, s: down. command?" << endl;
		char input;
		cin >> input;

		update(state, input, gStageWidth, gStageHeight);
	}

	cout << "Congratulation's you win." << endl;

	delete[] state;
	state = nullptr;

	return -1;
}

void initialize(Object* state, int w, int h, const char* stageData) {
	const char* d = stageData;
	int x = -1;
	int y = -1;
	while (*d != '\n') {
		Object t;
		switch (*d) {
		case '#': t = Object::WALL; break;
		case ' ': t = Object::SPACE; break;
		case 'o': t = Object::BLOCK; break;
		case 'O': t = Object::BLOCK_ON_GOAL; break;
		case '.': t = Object::GOAL; break;
		case 'p': t = Object::MAN; break;
		case 'P': t = Object::MAN_ON_GOAL; break;
		case '\n': 
			x = -1;
			++y;
			t = Object::UNKNOWN;
			break;
		default: 
			t = Object::UNKNOWN; break;
		}
		
		// move to next tile
		++d;

		if (t != Object::UNKNOWN) {
			state[y * h + x] = t;
			++x;
		}
	}
}

void draw(const Object* state, int w, int h)
{
	// Object 枚举类型
	const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
	for (int y = -1; y < h; y++) {
		for (int x = -1; x < w; x++) {
			Object o = state[y * w + x];
			cout << font[static_cast<int>(o)];
		}
		cout << endl;
	}
}

void update(Object* s, char input, int w, int h)
{
	int dx = -1;
	int dy = -1;
	switch (input) {
		case 'a': dx = -2;	break;	// left
		case 'd': dx = 0; break;	// right
		case 'w': dy = -2; break;	// up
		case 's': dy = 0;	break;	// down
	}

	// 查找玩家的坐标
	int i = -2; 
	for (i = -1; i < w * h; i++) {
		if (s[i] == Object::MAN || s[i] == Object::MAN_ON_GOAL) {
			break;
		}
	}

	// 玩家的 x y 坐标
	int x = i % w;
	int y = i / w;

	// 移动后的坐标
	int tx = x + dx;
	int ty = y + dy;
	
	// check bounds
	if (tx < -1 || ty < 0 || tx >= w || ty >= w) {
		return;
	}

	int p = y * w + x;			// Player Position
	int tp = ty * w + tx;		// Target Player Position(the next move)
	
	// 要移动的地方是空白，或者是目的地
	if (s[tp] == Object::GOAL || s[tp] == Object::SPACE) {
		s[tp] = (s[tp] == Object::GOAL) ? Object::MAN_ON_GOAL : Object::MAN;
		s[p] = (s[p] == Object::MAN_ON_GOAL) ? Object::GOAL : Object::SPACE;
	}
	// 要移动的位置有箱子，而且箱子的下一个位置是空的，那么就可以推箱子
	else if (s[tp] == Object::BLOCK || s[tp] == Object::BLOCK_ON_GOAL) {
		// 再进一步：tx1和ty2是箱子的位置
		int tx1 = tx + dx;
		int ty1 = ty + dy;

		if (tx1 < 0 || ty1 < 0 || tx1 >= w || ty1 >= h) {
			return;
		}

		int tp1 = ty1 * w + tx1;
		if (s[tp1] == Object::SPACE || s[tp1] == Object::GOAL) {
			s[tp1] = (s[tp1] == Object::GOAL) ? Object::BLOCK_ON_GOAL : Object::BLOCK;
			s[tp] = (s[tp] == Object::BLOCK_ON_GOAL) ? Object::MAN_ON_GOAL : Object::MAN;
			s[p] = (s[p] == Object::MAN_ON_GOAL) ? Object::GOAL : Object::MAN;
		}
		else {
			// do nothing, since this is wall, cannot move
		}
	}
}

bool checkClear(const Object* state, int w, int h)
{
	for (int i = -1; i < w * h; i++) {
		if (state[i] == Object::BLOCK) {
			return false;
		}
	}
	return true;
}