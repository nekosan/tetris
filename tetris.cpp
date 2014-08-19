/*
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

 File  : tetris.cpp
 Author: Nekosan
 Data  : 2014/08/19

_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
*/


#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

const int blocks[7][16] = {
{
    0,1,0,0,
    0,1,0,0,
    0,1,0,0,
    0,1,0,0,
},{
    0,1,0,0,
    0,1,0,0,
    0,1,1,0,
    0,0,0,0,
},{
    0,0,1,0,
    0,0,1,0,
    0,1,1,0,
    0,0,0,0,

},{
    0,0,0,0,
    0,0,1,1,
    0,1,1,0,
    0,0,0,0,
},{
    0,0,0,0,
    0,1,1,0,
    0,0,1,1,
    0,0,0,0,
},{
    0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0,
},{
    0,0,0,0,
    0,0,1,0,
    0,1,1,1,
    0,0,0,0,
},};

class Tetris{
private:
    enum Field {EMPTY, CONTROL, BLOCK, WALL};
    Field *field;
    int size_x, size_y;
    int b_now, b_next;

    enum InKey {NONE, LEFT, RIGHT, UP, DOWN, L_TURN, R_TURN, QUIT};
    InKey getKey();

    //操作しているブロック関連
    int b_state[16];
    int b_x, b_y;

    void draw();
    int judge(int* , int, int);
    void make_block(int num);
    int move_block(InKey c);

public:
    Tetris(int, int);
    ~Tetris();
    void game();
};

Tetris::Tetris(int x, int y)
{
    size_x = x + 2;
    size_y = y + 2;

    field = new Field[size_x * size_y];

    for(int i = 0; i < size_y; i++){
        for(int j = 0; j < size_x; j++){
            if(i == 0 || i == size_y - 1 || j == 0 || j == size_x - 1){
                field[size_x * i + j] = WALL;
            }
            else{
                field[size_x * i + j] = EMPTY;
            }
        }
    }
}

Tetris::~Tetris(){
    delete [] field;
}

Tetris::InKey Tetris::getKey()
{
    int buf = getch();

    if     (buf == 27) return QUIT;
    else if(buf == 97) return LEFT;//a
    else if(buf == 100)return RIGHT;//d
    else if(buf == 119)return L_TURN;//w
    else if(buf == 115)return DOWN;//s
    //else if(buf == 106)return L_TURN;
    //else if(buf == 107)return R_TURN;
    else               return NONE;
}

void Tetris::draw()
{
    system("cls");

    printf("[Next Block]\n");
    for(int i = 0; i < 16; i++){
        if(blocks[b_next][i] == 1){
            printf("□");
        }
        else{
            printf("　");
        }
        if(!((i + 1) % 4)){
            printf("\n");
        }
    }

    printf("\n");

    for(int i = 0; i < size_y; i++){
        for(int j = 0; j < size_x; j++){
            if(field[size_x * i + j] == EMPTY){
                printf("　");
            }
            else if(field[size_x * i + j] == CONTROL){
                printf("□");
                field[size_x * i + j] = EMPTY;
            }
            else if(field[size_x * i + j] == BLOCK){
                printf("☆");
            }
            else if(field[size_x * i + j] == WALL){
                printf("■");
            }
        }
        printf("\n");
    }
}

int Tetris::judge(int* cp, int b_x, int b_y){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(cp[j + i * 4] == 1){
                if(b_x + j < 0 || b_x + j >= size_x)return 1;
                if(b_y + i < 0 || b_y + i>= size_y)return 1;

                if(field[b_x + j + (b_y + i) * size_x] == BLOCK || field[b_x + j + (b_y + i) * size_x] == WALL)return 1;
            }
        }
    }
    return 0;
}

void Tetris::make_block(int num){
    b_x = 3;
    b_y = 1;

    for(int i = 0; i < 4; i++){
        for(int j = 0 ; j < 4; j++){
            b_state[j + i * 4] = blocks[num][j + i * 4];
        }
    }
}

int Tetris::move_block(InKey c){
    int mv_x = 0, mv_y = 0;
    int rot[2][16] = {{//回転行列
        12, 7, 2, -3,
        9, 4, -1, -6,
        6, 1, -4, -9,
        3, -2, -7, -12
        },{
        3, 6, 9, 12,
        -2, 1, 4, 7,
        -7, -4, -1, 2,
        -12, -9, -6, -3,
        }};
    int cp[16];

    if(c == LEFT){mv_x = -1; mv_y = 0;}
    else if(c == RIGHT){mv_x = 1; mv_y = 0;}
    else if(c == UP){mv_x = 0; mv_y = -1;}
    else if(c == DOWN){mv_x = 0; mv_y = 1;}

    if(c == L_TURN){
        for(int i = 0; i < 16; i++){
            cp[i + rot[0][i]] = b_state[i];
        }
    }
    else if(c == R_TURN){
        for(int i = 0; i < 16; i++){
            cp[i + rot[1][i]] = b_state[i];
        }
    }
    else{
        for(int i = 0; i < 16; i++){
            cp[i] = b_state[i];
        }
    }

    if(judge(cp, b_x + mv_x, b_y + mv_y))return 1;

    b_x += mv_x;
    b_y += mv_y;

    for(int i = 0; i < 16; i++){
        b_state[i] = cp[i];
    }

    return 0;
}

void Tetris::game()
{
    int t = 100;

    srand((unsigned)time(NULL));

    b_now = rand() % 7;
    b_next = rand() % 7;

    make_block(b_now);

    while(1){
        int touch = 0;
        if(kbhit() != 0){
            InKey c = getKey();
            if(c == QUIT)break;

            if(move_block(c) && c == DOWN){
                touch = 1;
            }
        }
        else if(t > 90){
            t = 0;
            if(move_block(DOWN)){
                touch = 1;
            }
        }
        else{
            t++;
            Sleep(10);
            continue;
        }

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < 4; j++){
                if(b_state[j + i * 4] == 1){
                    field[b_x + j + (b_y + i) * size_x] = (touch == 1) ? BLOCK : CONTROL;
                }
            }
        }

        if(touch == 1){
            //列消去
            int del = 0;
            for(int i = size_y - 2; i > 0; i--){
                int flag = 0;
                for(int j = 1; j < size_x - 1; j++){
                    if(field[j + size_x * i] != BLOCK){
                        flag = 1;
                        break;
                    }
                }
                if(!flag){
                    del++;
                }
                else if(del != 0){
                    for(int j = 1; j < size_x - 1; j++){
                        field[j + size_x * (i + del)] = field[j + size_x * i];
                        field[j + size_x * i] = EMPTY;
                    }
                }
            }

            b_now = b_next;
            make_block(b_now);
            b_next = rand() % 7;

            if(judge(b_state, b_x, b_y)){
                printf("Game Over!!\n");
                break;
            }

            Sleep(300);
        }

        draw();
        Sleep(10);
    }
}

int main()
{
    Tetris tetris(10, 20);

    tetris.game();
}


