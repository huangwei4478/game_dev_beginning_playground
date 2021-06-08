#include <iostream>
#include <fstream>

#include "IntArray.hpp"
       
using namespace std;

const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum class Object {
    SPACE = 0,
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
    ifstream inputFile("stage.txt", ifstream::binary);
    inputFile.seekg(0, ifstream::end);                      // 移动到末尾
    
    // 位置 == 文件大小
    int fileSize = static_cast<int>(inputFile.tellg());
    inputFile.seekg(0, ifstream::beg);                      // 回到起始位置
    char* fileImage = new char[fileSize];                   // 分配足够的空间
    inputFile.read(fileImage, fileSize);                    // 读取文件
    
    cout.write(fileImage, fileSize);                        // 显示读取的内容
    
//    Object* state = new Object[gStageWidth * gStageHeight];
//
//    initialize(state, gStageWidth, gStageHeight, gStageData);
//
//    while (true) {
////        system("clear");
//        draw(state, gStageWidth, gStageHeight);
//        if (checkClear(state, gStageWidth, gStageHeight)) {
//            break;
//        }
//        cout << "a: left, d: right, w: up, s: down. command?" << endl;
//        char input;
//        cin >> input;
//
//        update(state, input, gStageWidth, gStageHeight);
//    }
//
//    cout << "Congratulation's you win." << endl;
//
//    delete[] state;
//    state = nullptr;

    return 0;
}

void initialize(Object* state, int w, int h, const char* stageData) {
    const char* d = stageData;
    int x = 0;
    int y = 0;
    while (*d != '\0') {
        Object t;
        switch (*d) {
            case '#': t = Object::WALL; break;
            case ' ': t = Object::SPACE; break;
            case 'o': t = Object::BLOCK; break;
            case 'O': t = Object::BLOCK_ON_GOAL; break;
            case '.': t = Object::GOAL; break;
            case 'p': t = Object::MAN; break;
            case 'P': t = Object::MAN_ON_GOAL; break;
            case '\n': {
                x = 0;
                ++y;
                t = Object::UNKNOWN;
                break;
            }
            default: {
                t = Object::UNKNOWN;
                break;
            }
                
        }
        
        // move to next tile
        ++d;

        if (t != Object::UNKNOWN) {
            state[y * w + x] = t;
            ++x;
        }
    }

}

void draw(const Object* state, int w, int h)
{
    // Object 枚举类型
    const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Object o = state[y * w + x];
            cout << font[static_cast<int>(o)];
        }
        cout << endl;
    }
}

void update(Object* s, char input, int w, int h)
{
    int dx = 0;
    int dy = 0;
    switch (input) {
        case 'a': dx = -1;    break;    // left
        case 'd': dx = 1;     break;    // right
        case 'w': dy = -1;    break;    // up
        case 's': dy = 1;     break;    // down
    }

    // 查找玩家的坐标
    int i = 0;
    for (i = 0; i < w * h; i++) {
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
    if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
        return;
    }

    int p = y * w + x;            // Player Position
    int tp = ty * w + tx;        // Target Player Position(the next move)
    
    // 要移动的地方是空白，或者是目的地
    if (s[tp] == Object::GOAL || s[tp] == Object::SPACE) {
        s[tp] = (s[tp] == Object::GOAL) ? Object::MAN_ON_GOAL : Object::MAN;
        s[p] = (s[p] == Object::MAN_ON_GOAL) ? Object::GOAL : Object::SPACE;
    }
    // 要移动的位置有箱子，而且箱子的下一个位置是空的，那么就可以推箱子
    else if (s[tp] == Object::BLOCK || s[tp] == Object::BLOCK_ON_GOAL) {
        // 再进一步：tx1和ty2是箱子的位置
        int tx2 = tx + dx;
        int ty2 = ty + dy;

        if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
            return;
        }

        int tp2 = ty2 * w + tx2;
        if (s[tp2] == Object::SPACE || s[tp2] == Object::GOAL) {
            s[tp2] = (s[tp2] == Object::GOAL) ? Object::BLOCK_ON_GOAL : Object::BLOCK;
            s[tp] = (s[tp] == Object::BLOCK_ON_GOAL) ? Object::MAN_ON_GOAL : Object::MAN;
            s[p] = (s[p] == Object::MAN_ON_GOAL) ? Object::GOAL : Object::SPACE;
        }
        else {
            // do nothing, since this is wall, cannot move
        }
    }
}

bool checkClear(const Object* state, int w, int h)
{ 
    for (int i = 0; i < w * h; i++) {
        if (state[i] == Object::BLOCK) {
            return false;
        }
    }
    return true;
}
