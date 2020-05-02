#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <ctime>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace std;
bool gameOver;
const int width = 30;
const int height = 20;
int x, y, fx, fy, score;
int tailx[1000], taily[1000];
int ntail;
int score_text[100]; string name_text[100];
int text_nr = 1;
enum eDirection { STOP = 0, PAUSE, LEFT, RIGHT, UP, DOWN };
eDirection dir;
eDirection lastdir;
HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

void gameMenu();
void Setup();
void Draw();
void Input();
void Logic();
void Difficulty();
void scoreIN();
void scoreOUT();
void scoreREG();
void scoreSORT();
void cls();
void setFontSize(int);
void setConsoleSize(int, int);
int random(int, int);

int main()
{
    setFontSize(16);
    setConsoleSize(400, 500);
    scoreIN();
    PlaySound(TEXT("sounds/Inferno.wav"), NULL, SND_LOOP | SND_ASYNC);
    gameMenu();
}

void gameMenu()
{
    system("cls");
    cout << "1. START NEW GAME" << endl;
    cout << "2. SCORE BOARD" << endl;
    cout << "3. CONTROLS" << endl;
    cout << "4. EXIT" << endl;

    switch (_getch())
    {
    case '1':
        system("cls");
        Setup();
        while (!gameOver)
        {
            Input();
            Draw();
            Logic();
            Difficulty();
        }
        scoreREG();
        gameMenu();
        break;
    case '2':
        system("cls");
        for (int i = 1; i <= text_nr; i++)
        {
            if (i < 10)
                cout << " ";
            cout << i << ". " << name_text[i];
            if (name_text[i].length() < 4)
                cout << "\t\t";
            else
                cout << "\t";
            cout << score_text[i] << endl;
        }
        cout << "Press ANY key to get back." << endl;
        _getch();
        gameMenu();
    case '3':
        system("cls");
        cout << "CONTROLS:" << endl;
        cout << GREEN;
        cout << "W - move UP" << endl;
        cout << "S - move DOWN" << endl;
        cout << "A - move LEFT" << endl;
        cout << "D - move RIGHT" << endl;
        cout << "P - pause" << endl;
        cout << RESET;
        cout << "Press ANY key to get back." << endl;
        _getch();
        gameMenu();
        break;
    case '4':
        exit(0);
        break;
    default:
        system("cls");
        cout << "Unavaible option. Press ANY key to get back." << endl;
        _getch();
        gameMenu();
        break;
    }
}
void Setup()
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    for (int i = 0; i <= ntail; i++)
    {
        tailx[i] = 0;
        taily[i] = 0;
    }
    ntail = 0;
    fx = random(1, width);
    fy = random(1, height);
    while (fx == x && fy == y)
    {
        fx = random(1, width);
        fy = random(1, height);
    }
    score = 0;
}
void Draw()
{
    cls();
    int l = ((width + 2) - 10) / 2; //centered
    if (dir != PAUSE)
    {
        for (int i = 1; i <= l; i++)
            cout << " ";
        cout << "SNAKE GAME ";
    }
    else
    {
        for (int i = 1; i <= l; i++)
            cout << " ";
        cout << "GAME PAUSED";
    }
    cout << endl;

    while (dir == PAUSE)
    {
        if (_getch() == 'p')
        {
            dir = lastdir;
            break;
        }
    }
    //first line
    for (int i = 0; i <= width+1; i++)
    {
        cout << CYAN;
        cout << "#";
    }
    cout << endl;

    for (int i = 1; i <= height; i++)
    {
        for (int j = 0; j <= width+1; j++)
        { 
            // left and right walls
            if (j == 0 || j == width+1)
            {
                cout << CYAN;
                cout << "#";
            }
            //head
            else if (i == y && j == x)
            {
                cout << GREEN;
                if (dir == LEFT)
                    cout << "\x003C";
                else if (dir == RIGHT)
                    cout << "\x003E";
                else if (dir == UP)
                    cout << "\x005E";
                else if (dir == DOWN)
                    cout << "v";
                else if (dir == STOP)
                    cout << "o";
            }
            //fruit
            else if (i == fy && j == fx)
            {
                cout << RED;
                cout << "+";
            }
            //tail
            else
            {
                bool print = false;
                for (int k = 0; k < ntail; k++)
                {
                    if (tailx[k] == j && taily[k] == i)
                    {
                        cout << GREEN;
                        cout << "o";
                        print = true;
                    }
                }
                if (!print)
                   cout << " ";
            }
        }
        cout << endl;
    }
    //last line
    for (int i = 0; i <= width+1; i++)
    {
        cout << CYAN;
        cout << "#";
    }
    cout << endl;
    //Score
    cout << RESET;
    cout << "Score: " << score << "\tHigh score: " << score_text[1] << endl;
}
void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if (dir != RIGHT)
            {
                dir = LEFT;
                lastdir = LEFT;
            }
            break;
        case 'd':
            if (dir != LEFT)
            {
                dir = RIGHT;
                lastdir = RIGHT;
            }
            break;
        case 's':
            if (dir != UP)
            {
                dir = DOWN;
                lastdir = DOWN;
            }
            break;
        case 'w':
            if (dir != DOWN)
            {
                dir = UP;
                lastdir = UP;
            }
            break;
        case 'p':
            dir = PAUSE;
            break;
        }
    }
}
void Logic()
{
    if (dir != PAUSE )
    {
        for (int i = ntail - 1; i > 0; i--)
        {
            tailx[i] = tailx[i - 1];
            taily[i] = taily[i - 1];
        }
        tailx[0] = x;
        taily[0] = y;

        switch (dir)
        {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
        }
        //hit wall
        if (x == width + 1 || x == 0 || y == height + 1 || y == 0)
        {
            gameOver = true;
            cout << "You hit the wall!" << endl;
            cout << "GAME OVER" << endl;
        }
        //hit tail
        for (int i = 0; i < ntail; i++)
        {
            if (tailx[i] == x && taily[i] == y)
            {
                gameOver = true;
                cout << "You hit your tail!" << endl;
                cout << "GAME OVER" << endl;
            }
        }
        //get fruit
        if (x == fx && y == fy)
        {
            if (score != (width * height - 1) * 10 )
            {
                ntail++;
                score = score + 10;
                while (true)
                {
                    int ok = 0;
                    fx = random(1, width);
                    fy = random(1, height);
                    for (int i = 0; i <= ntail; i++)
                    {
                        if ((fx == tailx[i] && fy == taily[i]) || (fx == x && fy == y))
                        {
                            ok = 1;
                            break;
                        }
                    }
                    if (ok == 0)
                        break;
                }
            }
            else
            {
                cout << "CONGRATS. YOU WON THE GAME!" << endl;
                gameOver = true;
            }
        }
    }
}
void Difficulty()
{
    int time;
    time = 70 - score / 100 * 5;
    if (time > 0)
        Sleep(time);
}
void scoreIN()
{
    ifstream fin("score.txt");
    string str;
    while (fin >> str)
    {
        if (text_nr % 2 == 1)
        {
            try
            { score_text[(text_nr + 1) / 2] = stoi(str); }
            catch(...)
            { break; }
        }
        else
            name_text[text_nr / 2] = str;
        text_nr++;
    }
    text_nr = text_nr / 2;
    fin.close();
    scoreSORT();
    scoreOUT();
}
void scoreOUT()
{
    ofstream fout("score.txt");
    for (int i = 1; i <= text_nr; i++)
        fout << score_text[i] << endl << name_text[i] << endl;
    fout.close();
}
void scoreREG()
{
    bool ok = false;
    if (text_nr < 10)
        text_nr++;
    if (text_nr >= 10 && score > score_text[text_nr] || text_nr < 10)
    {
        string name;
        cout << "Enter your name: "; cin >> name;
        score_text[text_nr] = score;
        name_text[text_nr] = name;
        ok = true;
    }
    if (ok)
    {
        scoreSORT();
        scoreOUT();
    }
    else
    {
        cout << "Press ANY key to get back to the menu!";
        _getch();
    }
}
void scoreSORT()
{
    for (int i = text_nr - 1; i >= 1; i--)
    {
        for (int j = text_nr; j >= i + 1; j--)
        {
            if (score_text[i] < score_text[j])
            {
                swap(score_text[i], score_text[j]);
                swap(name_text[i], name_text[j]);
            }
        }
    }
    if (text_nr > 10)
        text_nr = 10;
}
void cls()
{
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}
void setFontSize(int FontSize)
{
    CONSOLE_FONT_INFOEX info = { 0 };
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = FontSize;
    info.FontWeight = FW_NORMAL;
    wcscpy_s(info.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}
void setConsoleSize(int XSize, int YSize)
{
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    MoveWindow(console, ConsoleRect.left, ConsoleRect.top, XSize, YSize, TRUE);
}
int random(int min, int max) //range : [min, max)
{
    static bool first = true;
    if (first)
    {
        srand((unsigned int)time(NULL)); //seeding for the first time only!
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}