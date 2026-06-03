#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

// dimensioni campo
const int width = 30;
const int height = 20;

bool gameOver;
int x, y, foodX, foodY, score;
int tailX[200], tailY[200];
int nTail;

enum Dir { STOP = 0, LEFT, RIGHT, UP, DOWN };
Dir dir;

// colori
void color(int c) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void setup() {
    gameOver = false;
    dir = STOP;

    x = width / 2;
    y = height / 2;

    srand(time(0));
    foodX = rand() % width;
    foodY = rand() % height;

    score = 0;
    nTail = 0;
}

// UI più pulita
void draw() {
    system("cls");

    // header UI
    color(11);
    cout << "================ SNAKE ARCADE ================\n";
    color(7);
    cout << "Score: " << score << "   |   Lunghezza: " << nTail << "\n";
    cout << "Controlli: WASD o Frecce | X = Esci\n\n";

    // bordo superiore
    color(8);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";

    for (int i = 0; i < height; i++) {
        color(8);
        cout << "#";

        for (int j = 0; j < width; j++) {

            if (i == y && j == x) {
                color(10);
                cout << "O";
            }
            else if (i == foodY && j == foodX) {
                color(12);
                cout << "@";
            }
            else {
                bool printed = false;

                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        color(2);
                        cout << "o";
                        printed = true;
                        break;
                    }
                }

                if (!printed) cout << " ";
            }
        }

        color(8);
        cout << "#\n";
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << "\n";

    color(7);
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': case 75: dir = LEFT; break;
        case 'd': case 77: dir = RIGHT; break;
        case 'w': case 72: dir = UP; break;
        case 's': case 80: dir = DOWN; break;
        case 'x': gameOver = true; break;
        }
    }
}

// LOGICA: muri mortali
void logic() {
    int prevX = x;
    int prevY = y;

    for (int i = 0; i < nTail; i++) {
        int tempX = tailX[i];
        int tempY = tailY[i];

        tailX[i] = prevX;
        tailY[i] = prevY;

        prevX = tempX;
        prevY = tempY;
    }

    switch (dir) {
    case LEFT: x--; break;
    case RIGHT: x++; break;
    case UP: y--; break;
    case DOWN: y++; break;
    default: break;
    }

    // 💀 MURI MORTALI
    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;

    // collisione corpo
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // cibo
    if (x == foodX && y == foodY) {
        score += 10;
        foodX = rand() % width;
        foodY = rand() % height;
        nTail++;
    }
}

int main() {
    // nasconde cursore
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(80); // più fluido e veloce
    }

    system("cls");
    color(12);
    cout << "\n\n******** GAME OVER ********\n";
    color(7);
    cout << "Score finale: " << score << "\n\n";

    system("pause");
    return 0;
}
