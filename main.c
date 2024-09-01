#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <mmsystem.h>
#include <stdbool.h>
#include "Prototype.h"
#define T1 120
#define T2 180

//Set Character
#define SNAKES 254
#define WALL 219
#define TABLE 177
#define CLICK 021

//Set Table
#define TABLE_X0 15
#define TABLE_Y0 5
#define TABLE_X 37
#define TABLE_Y 12

//Set Wall
#define WALL_X0 1
#define WALL_Y0 1
#define WALL_X 51
#define WALL_Y 16

//Global Variable
short Start=1;
short Condition;
short NEWGAME=0;
short OverGame=1;
short Continue;
short mode;
short StatusFood;
short Exit=1;

//Set Color
int ColorBackground=15,ColorWall=0,ColorBackGroundTable=8,ColorTable=0,ColorText=0,ColorSnakes=0,ColorFood=0;
int SnakeType=0,FoodType=0;
int Head=254,Body=254,Tail=254;
int FOOD=254;

//Set Volume
int Music=1;
int Sound=1;



int main()
{
    short direction;
    short Element;
    short Count;
    int i;
    int Scores[4];
    ToaDo Snakes[1000];
    ToaDo Food;
    srand((unsigned int)time(NULL));
    SetConsole();
    SetColor(ColorBackground,ColorBackground);
    ClrTable(0,0,53,18);
    while(OverGame)
    {
        if(Exit)
            IntroGame();
        else
        {
            SetColor(ColorBackground,ColorBackground);
            ClrTable(WALL_X0+1,WALL_Y0+1,WALL_X-1,WALL_Y-1);
            SetColor(ColorBackground,ColorWall);
            DrawWall(WALL_X0,WALL_Y0,WALL_X,WALL_Y);
        }

        Menu();

        if(Continue)
        {
            ReadParameter(&direction,&Element,&Count,Scores,Snakes,&Food);
            Condition=1;

        }
        else if(NEWGAME)
        {
            SelectMode();//Chọn chế độ chơi.
            ResetParameter(Scores,&direction,&Element,&Count,Snakes,&Food);
        }
        while(Condition)//Nếu Condition=1 thì tiếp tục vòng lặp.
        {
            Remote(&direction,Food);//Hàm nhận tín hiệu di chuyển từ bàn phím.
            DrawFood(&Food.x,&Food.y,Snakes,Element);
            SnakesMove(Snakes,&Element,&direction,Food);//Hàm di chuyển của rắn.

            if(Snakes[0].x==Food.x&&Snakes[0].y==Food.y)//Nếu ăn food thì khởi tạo food ngẫu nhiên.
            {
                if(Sound)
                    PlaySound("audio game/Food",NULL,SND_FILENAME|SND_ASYNC);

                StatusFood=1;
                DrawFood(&Food.x,&Food.y,Snakes,Element);
                Count++;//Biến đếm tăng lên 1.
                Element++;//Phần tử tăng lên 1.
            }
            if(mode)
            {
                ClassicMode(Snakes);//Chế độ không xuyên tường.
            }
            else
            {
                ModernMode(Snakes);//Chế độ xuyên tường.
            }

            for(i=4; i<=Element; i++)//Nếu đầu trùng với thân thì gameover.
            {
                if(Snakes[0].x==Snakes[i].x&&Snakes[0].y==Snakes[i].y)
                {
                    TableOverGame();//Hiển thị bảng kết thúc game.
                }
            }
            GetScores(Scores,Count);//Hàm lấy điểm.
        }
        PlaySound(NULL,NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);
        HighScores(Count);
        WriteParameter(direction,Element,Count,Scores,Snakes,Food);
    }

    return 0;
}





void SnakesMove(ToaDo *Snakes,short *Element,short *direction,ToaDo Food)//Hàm di chuyển của rắn.
{
    int i;
    if(StatusFood==1)
    {
        Snakes[*Element].x=Snakes[*Element-1].x;
        Snakes[*Element].y=Snakes[*Element-1].y;
    }

    for(i=0; i<=*Element; i++)
    {
        Snakes[i].x0=Snakes[i].x;
        Snakes[i].y0=Snakes[i].y;
        if(i<1)
        {
            Sleep(T1);
            ClrHead(Snakes[0].x,Snakes[0].y);
            switch(*direction)
            {
            case 0:
                Snakes[0].x+=0;
                Snakes[0].y+=0;
                DrawHead(Snakes[0].x,Snakes[0].y,Snakes,Food);
                break;
            case 1:
            {
                Snakes[0].x+=1;
                *direction=1;
            }
            break;
            case 2:
            {
                Snakes[0].x-=1;
                *direction=2;
            }
            break;
            case 3:
            {
                Snakes[0].y-=1;
                *direction=3;
            }
            break;
            case 4:
            {
                Snakes[0].y+=1;
                *direction=4;
            }
            break;
            }
            DrawHead(Snakes[0].x,Snakes[0].y,Snakes,Food);
        }
        else
        {
            if(*direction==0)
            {
                if(i==*Element)
                    DrawTail(Snakes[i].x,Snakes[i].y);
                else
                    DrawBody(Snakes[i].x,Snakes[i].y);
            }
            else
            {
                if(StatusFood==1)
                {
                    Clrxy(Snakes[i].x,Snakes[i].y);
                    Snakes[i].x=Snakes[i-1].x0;
                    Snakes[i].y=Snakes[i-1].y0;
                    if(i==*Element)
                        DrawTail(Snakes[i].x,Snakes[i].y);
                    else
                        DrawBody(Snakes[i].x,Snakes[i].y);
                    if(i==*Element-1)
                    {
                        Snakes[i].x=Snakes[i-1].x0;
                        Snakes[i].y=Snakes[i-1].y0;
                        DrawBody(Snakes[i].x,Snakes[i].y);
                    }
                    else if(i==*Element)
                    {
                        Snakes[i].x+=0;
                        Snakes[i].y+=0;
                    }
                    StatusFood=0;
                }
                else
                {
                    Clrxy(Snakes[i].x,Snakes[i].y);
                    Snakes[i].x=Snakes[i-1].x0;
                    Snakes[i].y=Snakes[i-1].y0;
                    if(i==*Element)
                        DrawTail(Snakes[i].x,Snakes[i].y);
                    else
                        DrawBody(Snakes[i].x,Snakes[i].y);
                }
            }
        }
    }
}

void Remote(short *direction,ToaDo Food)//Hàm điều khiển rắn, nhận sự kiện từ bàn phím.
{
    int key=0;
    if(kbhit())
    {
        key=getch();

        if(key==32)
        {
            *direction=0;
            TablePauseGame(Food);
        }
        if(key==77||key==100)
        {
            if(*direction!=2)
                *direction=1;
            else
                *direction=2;
        }
        if(key==75||key==97)
        {
            if(*direction!=1)
                *direction=2;
            else
                *direction=1;
        }
        if(key==72||key==119)
        {
            if(*direction!=4)
                *direction=3;
            else
                *direction=4;
        }
        if(key==80||key==115)
        {
            if(*direction!=3)
                *direction=4;
            else
                *direction=3;
        }
    }
}
void DrawBody(int x, int y)//Hàm in ra ký tự tại tọa độ (x,y).
{
    SetColor(ColorBackground,ColorSnakes);
    gotoxy(x,y);
    printf("%c",Body);
}
void DrawFood(int *x,int *y,ToaDo *Snakes,short Element)//Hàm vẽ food.
{
    SetColor(ColorBackground,ColorFood);
    if(StatusFood)
    {
        Clrxy(*x,*y);
        *x=GetRandom(2,50);
        *y=GetRandom(2,15);

        if(*x==1||*x==51||*y==1||*y==16)
        {
            Clrxy(*x,*y);
            *x=GetRandom(2,50);
            *y=GetRandom(2,15);
        }
        int i;
        for(i=0; i<=Element; i++)
        {
            if(Snakes[i].x==*x&&Snakes[i].y==*y)
            {
                Clrxy(*x,*y);
                *x=GetRandom(2,50);
                *y=GetRandom(2,15);
            }
        }
        gotoxy(*x,*y);
        printf("%c",FOOD);
    }
    else
    {
        gotoxy(*x,*y);
        printf("%c",FOOD);
    }
    StatusFood=0;
}
void DrawHead(int x,int y,ToaDo *Snakes,ToaDo Food)//Hàm vẽ đầu.
{
    SetColor(ColorBackground,ColorSnakes);
    int a,b;
    a=Snakes[0].x-Food.x;
    b=Snakes[0].y-Food.y;
    if(abs(a)==1&&Snakes[0].y==Food.y)
    {
        gotoxy(x,y);
        printf("%c",Head);
    }
    else if(abs(b)==1&&Snakes[0].x==Food.x)
    {
        gotoxy(x,y);
        printf("%c",Head);
    }
    else
    {
        gotoxy(x,y);
        printf("%c",Head);
    }
    SetColor(ColorBackground,ColorText);
}
void ClrHead(int x,int y)//Hàm xóa đầu.
{
    gotoxy(x,y);
    printf(" ");
}
void DrawTail(int x,int y)//Hàm vẽ đuôi.
{
    SetColor(ColorBackground,ColorSnakes);
    gotoxy(x,y);
    printf("%c",Tail);
}
void Clrxy(int x,int y)//Hàm xóa ký tự.
{
    gotoxy(x,y);
    printf(" ");
}
void gotoxy(int x,int y)//x,y là tọa điểm x,y trên màn hình
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD a = {x, y};
    SetConsoleCursorPosition(h, a);
}
int GetRandom(int MIN,int MAX)//Hàm lấy random.
{
    return MIN + (int)(rand()*(MAX-MIN+1)/(1+RAND_MAX));
}
void DrawWall(int x0,int y0,int x,int y)//Hàm vẽ tường.
{
    int i,j;
    for(j=y0; j<=y; j++)
    {
        for(i=x0; i<=x; i++)
        {
            if(j==y0||j==y)
            {
                gotoxy(i,j);
                printf("%c",WALL);
            }
            else
            {
                if(i==x0||i==x)
                {
                    gotoxy(i,j);
                    printf("%c",WALL);
                }
            }
        }
    }
}

void GetScores(int *Scores,short Count)//Hàm lấy điểm.
{
    int i;
    for(i=3; i>=0; i--)
    {
        if(i==3)
            Scores[i]=Count%10;
        else if(i==2)
        {
            if(Count<99)
                Scores[i]=Count/10;
            else
                Scores[i]=Count%100;
        }
        else if(i==1)
            Scores[i]=0;
        else if(i==0)
            Scores[i]=0;
    }
    SetColor(ColorBackground,ColorText);
    gotoxy(36,0);
    printf("SCORES : %d%d%d%d",Scores[0],Scores[1],Scores[2],Scores[3]);
}
void DrawTable(int x0,int y0,int x,int y)//Hàm vẽ bảng.
{
    int i,j;
    for(j=y0; j<=y; j++)
    {
        for(i=x0; i<=x; i++)
        {
            if(j==y0||j==y)
            {
                gotoxy(i,j);
                printf("%c",TABLE);
            }
            else
            {
                if(i==x0||i==x)
                {
                    gotoxy(i,j);
                    printf("%c",TABLE);
                }
            }
        }
    }
}
void ClrTable(int x0,int y0,int x,int y)//Hàm xóa bảng.
{
    int i,j;
    for(j=y0; j<=y; j++)
    {
        for(i=x0; i<=x; i++)
        {
            gotoxy(i,j);
            printf(" ");
        }
    }
}
void Clrline(int x0,int x,int y)
{
    int i;
    for(i=x0; i<=x; i++)
    {
        Clrxy(i,y);
    }
}
void SelectMode()
{
    int key=0;
    int Select=2;
    TableTransOn(15,5,37,12);
    gotoxy(17,7);
    SetColor(ColorBackGroundTable,ColorText);
    printf("%c%c%c SELECT MODE %c%c%c",240,240,240,240,240,240);
    gotoxy(22,9);
    printf("%cCLASSIC%c",249,249);
    gotoxy(22,10);
    printf("%cMODERN%c%c",249,233,249);
    while(key!=13)
    {
        if(kbhit())
        {
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);

            key=getch();
            if(key==115)
                Select=1;
            else if(key==119)
                Select=2;
            else if(key==13)
                Select=0;
        }
        if(Select==1)
        {
            Clrxy(32,9);
            mode=0;
            Condition=1;
            gotoxy(32,10);
            printf("%c",CLICK);

        }
        else if(Select==2)
        {
            Clrxy(32,10);
            mode=1;
            Condition=1;
            gotoxy(32,9);
            printf("%c",CLICK);

        }
        else if(Select==0)
        {
            Condition=1;
        }
    }
    TableTransOff(15,5,37,12);
    PlaySound(NULL,NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);

}
void ClassicMode(ToaDo *Snakes)
{
    if(Snakes[0].x==1||Snakes[0].x==51||Snakes[0].y==1||Snakes[0].y==16)
    {
        TableOverGame();
    }
}
void ModernMode(ToaDo *Snakes)
{
    SetColor(ColorBackground,ColorWall);
    if(Snakes[0].x==1||Snakes[0].x==51||Snakes[0].y==1||Snakes[0].y==16)
    {
        if(Snakes[0].x==1)
        {
            gotoxy(Snakes[0].x,Snakes[0].y);
            printf("%c",WALL);
            Snakes[0].x=50;
        }
        else if(Snakes[0].x==51)
        {
            gotoxy(Snakes[0].x,Snakes[0].y);
            printf("%c",WALL);
            Snakes[0].x=2;
        }
        else if(Snakes[0].y==1)
        {
            gotoxy(Snakes[0].x,Snakes[0].y);
            printf("%c",WALL);
            Snakes[0].y=15;
        }
        else if(Snakes[0].y==16)
        {
            gotoxy(Snakes[0].x,Snakes[0].y);
            printf("%c",WALL);
            Snakes[0].y=2;
        }
    }
}
void TableOverGame()
{
    if(Sound)
        PlaySound("audio game/OverGame",NULL,SND_FILENAME|SND_ASYNC);

    SetColor(ColorBackGroundTable,ColorTable);
    int key=0;
    int t;
    int Select=2;
    TableTransOn(15,5,37,12);
    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(18,7);
    printf("%c%c%c GAME OVER %c%c%c",240,240,240,240,240,240);
    gotoxy(22,9);
    printf("%cNEW GAME%c",249,249);
    gotoxy(24,10);
    printf("%cEXIT%c",249,249);

    while(key!=13)
    {
        if(kbhit())
        {
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);

            key=getch();
            if(key==115)
                Select=1;
            else if(key==119)
                Select=2;
            else if(key==13)
                Select=0;
        }
        if(Select==1)
        {
            t=0;
            Clrxy(33,9);
            gotoxy(33,10);
            printf("%c",CLICK);
        }
        else if(Select==2)
        {
            t=1;
            Clrxy(33,10);
            gotoxy(33,9);
            printf("%c",CLICK);
        }
        else if(Select==0)
        {
            if(t==0)
            {
                Condition=0;
                OverGame=1;
                Exit=0;
                Start=1;
            }
            else if(t==1)
            {
                Condition=0;
                NEWGAME=1;
                Exit=0;
                OverGame=1;
                Start=1;
            }

        }
    }
    TableTransOff(15,5,37,12);
}
void TablePauseGame(ToaDo Food)
{
    if(Sound)
        PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);

    SetColor(ColorBackGroundTable,ColorTable);
    int key;
    int t;
    int Select=2;
    TableTransOn(15,5,37,12);
    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(19,7);
    printf("== PAUSE GAME ==");
    gotoxy(23,9);
    printf("CONTINUE");
    gotoxy(25,10);
    printf("EXIT");

    while(key!=13)
    {
        if(kbhit())
        {
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);

            key=getch();
            if(key==115)
                Select=1;
            else if(key==119)
                Select=2;
            else if(key==13)
                Select=0;
        }
        if(Select==1)
        {
            t=0;
            Clrxy(33,9);
            gotoxy(33,10);
            printf("%c",CLICK);

        }
        else if(Select==2)
        {
            t=1;
            Clrxy(33,10);
            gotoxy(33,9);
            printf("%c",CLICK);

        }
        else if(Select==0)
        {
            SetColor(ColorBackground,ColorText);
            if(t==1)
            {
                ClrTable(15,5,37,12);
                SetColor(ColorBackground,ColorFood);
                gotoxy(Food.x,Food.y);
                printf("%c",FOOD);
                SetColor(ColorBackground,ColorText);
            }
            if(t==0)
            {
                Condition=0;
                Exit=0;
                OverGame=1;
                Start=0;
            }

        }
    }
    TableTransOff(15,5,37,12);
    ClrTable(2,2,50,15);
    if(Music)
        PlaySound("audio game/Badguy2",NULL,SND_FILENAME|SND_ASYNC);
}

void IntroGame()
{
    SetColor(ColorBackground,ColorWall);

    int x0,y0,x,y;
    y0=9;
    y=8;
    x0=WALL_X0;
    x=WALL_X;
    while(y0!=WALL_Y0&&y!=WALL_Y)
    {
        Clrxy(x0,y0);
        gotoxy(x0,y0);
        printf("%c",WALL);
        Clrxy(x,y0);
        gotoxy(x,y0);
        printf("%c",WALL);
        Clrxy(x0,y);
        gotoxy(x0,y);
        printf("%c",WALL);
        Clrxy(x,y);
        gotoxy(x,y);
        printf("%c",WALL);
        y0--;
        y++;
        Sleep(15);
    }
    while(x0!=x)
    {
        Clrxy(x0,WALL_Y0);
        gotoxy(x0,WALL_Y0);
        printf("%c",WALL);
        Clrxy(x,WALL_Y);
        gotoxy(x,WALL_Y);
        printf("%c",WALL);
        Clrxy(x0,WALL_Y);
        gotoxy(x0,WALL_Y);
        printf("%c",WALL);
        Clrxy(x,WALL_Y0);
        gotoxy(x,WALL_Y0);
        printf("%c",WALL);
        Sleep(15);
        x0++;
        x--;
    }
    DrawWall(WALL_X0,WALL_Y0,WALL_X,WALL_Y);//Vẽ biên.
    SetColor(ColorBackground,ColorText);
}

void SetWindowSize(short width, short height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 1;
    WindowSize.Right = width;
    WindowSize.Bottom = height;

    SetConsoleWindowInfo(hStdout,1,&WindowSize);
}

void SetScreenBufferSize(short width, short height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, NewSize);
}

void DisableResizeWindow()
{
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void DisableCtrButton(int Close, int Min, int Max)
{
    HWND hWnd = GetConsoleWindow();
    HMENU hMenu = GetSystemMenu(hWnd, 0);

    if (Close == 1)
    {
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
    }
    if (Min == 1)
    {
        DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
    }
    if (Max == 1)
    {
        DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
    }
}

void ShowScrollbar(WINBOOL Show)
{
    HWND hWnd = GetConsoleWindow();
    ShowScrollBar(hWnd, SB_BOTH, Show);
}

void ShowCur(WINBOOL CursorVisibility)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConCurInf;

    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = CursorVisibility;

    SetConsoleCursorInfo(handle, &ConCurInf);
}

void HighScores(short Count)
{
    SetColor(ColorBackground,ColorWall);
    User Player[4];
    FILE* taptin;
    int i=0;
    int Min;

    taptin=fopen("HighScores","r");

    while(fscanf(taptin,"%s %d",Player[i].Name,&Player[i].Scores)!=EOF)
    {
        i++;
    }
    fclose(taptin);

    Min=Player[0].Scores;

    if(Count>Min)
    {
        int i;
        SetColor(ColorBackground,ColorText);
        ClrTable(TABLE_X0,TABLE_Y0,TABLE_X,TABLE_Y);
        SetColor(ColorBackground,ColorWall);
        TableTransOn(11,5,41,11);
        gotoxy(15,7);
        printf("=== NEW HIGH SCORES ===");
        gotoxy(13,9);
        printf("Player Name : ");
        gotoxy(27,9);
        ShowCur(1);
        scanf("%s",Player[0].Name);
        gotoxy(41,16);
        ShowCur(0);
        Player[0].Scores=Count;
        ArrangeArray(Player);
        taptin=fopen("HighScores","w");
        for(i=0; i<=3; i++)
        {
            fprintf(taptin,"%s %d\n",Player[i].Name,Player[i].Scores);
        }
        fclose(taptin);
        TableTransOff(11,5,41,11);
        SetColor(ColorBackground,ColorText);
        ClrTable(2,2,50,15);
        SetColor(ColorBackground,ColorWall);
        TopList();
        Start=1;
    }
    SetColor(ColorBackground,ColorText);
}
void ArrangeArray(User *Player)
{
    int i,j;
    int a,c;
    char b[10],d[10];
    int e,f;

    for(i=0; i<3; i++)
    {
        if(Player[i].Scores>Player[i+1].Scores)
        {
            a=Player[i].Scores;
            for(e=0; e<=10; e++)
            {
                b[e]=Player[i].Name[e];
                Player[i].Name[e]=Player[i+1].Name[e];
            }
            Player[i].Scores=Player[i+1].Scores;
            Player[i+1].Scores=a;
            for(e=0; e<=10; e++)
            {
                Player[i+1].Name[e]=b[e];
            }
            for(j=i; j>0; j--)
            {
                if(Player[j].Scores<Player[j-1].Scores)
                {
                    c=Player[j].Scores;
                    for(f=0; f<=10; f++)
                    {
                        d[f]=Player[j].Name[f];
                        Player[j].Name[f]=Player[j-1].Name[f];
                    }
                    Player[j].Scores=Player[j-1].Scores;
                    Player[j-1].Scores=c;
                    for(f=0; f<=10; f++)
                    {
                        Player[j-1].Name[f]=d[f];
                    }
                }
            }
        }
    }
    SetColor(ColorBackground,ColorText);
}
void SetColor(int backgound_color, int text_color)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    int color_code = backgound_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}
void ReadParameter(short *direction,short *Element,short *Count,int *Scores,ToaDo *Snakes, ToaDo *Food)
{
    FILE *taptin=NULL;
    taptin=fopen("Parameter","r");
    fscanf(taptin,"%hd %hd %hd %hd %hd %hd",&Condition,direction,&StatusFood,Element,Count,&mode);

    int i;
    for(i=0; i<=3; i++)
    {
        fscanf(taptin,"%d ",&Scores[i]);
    }
    for(i=0; i<=2; i++)
    {
        fscanf(taptin,"%d %d",&Snakes[i].x,&Snakes[i].y);
    }
    fscanf(taptin,"%d %d",&Food->x,&Food->y);
    fclose(taptin);
}
void WriteParameter(short direction,short Element,short Count,int *Scores,ToaDo *Snakes,ToaDo Food)
{
    FILE *taptin=NULL;
    taptin=fopen("Parameter","w");
    fprintf(taptin,"%hd %hd %hd %hd %hd %hd\n",Condition,direction,StatusFood,Element,Count,mode);
    int i;
    for(i=0; i<=3; i++)
    {
        fprintf(taptin,"%d ",Scores[i]);
    }
    for(i=0; i<=2; i++)
    {
        fprintf(taptin,"%d %d\n",Snakes[i].x,Snakes[i].y);
    }
    fprintf(taptin,"%d %d",Food.x,Food.y);
    fclose(taptin);
}
void Menu()
{
    if(Music)
        PlaySound("audio game/Badguy1",NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);

    SetColor(ColorBackGroundTable,ColorTable);
    if(Start==0)
    {
        int key=0;
        int i=8;
        int x=23;
        int On=0;
        TableTransOn(15,4,37,13);

        while(key!=13)
        {
            if(On==1)
            {
                TableTransOn(15,4,37,13);
                On=0;
            }
            SetColor(ColorBackGroundTable,ColorText);
            gotoxy(20,6);
            printf("%c%c%c%c MENU %c%c%c%c",240,240,240,240,240,240,240,240);
            gotoxy(x,8);
            printf("CONTINUE");
            gotoxy(x,9);
            printf("NEWGAME");
            gotoxy(x,10);
            printf("SETTING");
            gotoxy(x,11);
            printf("TOPLIST");
            gotoxy(33,i);
            printf("%c",CLICK);
            if(kbhit())
            {
                key=getch();
                if(key==115)
                {
                    i++;
                    if(i>=11)
                        i=11;
                    Clrxy(33,i-1);
                    gotoxy(33,i);
                    printf("%c",CLICK);
                }
                else if(key==119)
                {
                    i--;
                    if(i<=8)
                        i=8;
                    Clrxy(33,i+1);
                    gotoxy(33,i);
                    printf("%c",CLICK);
                }

                else if(key==13)
                {
                    switch(i)
                    {
                    case 8:
                        SlideTrans2(x,i);
                        Continue=1;
                        NEWGAME=0;
                        PlaySound(NULL,0,0);
                        TableTransOff(15,4,37,13);
                        break;
                    case 9:
                        SlideTrans2(x,i);
                        NEWGAME=1;
                        Continue=0;
                        TableTransOff(15,4,37,13);
                        break;
                    case 10:
                        SlideTrans2(x,i);
                        TableTransOff(15,4,37,13);
                        Setting();
                        key=0;
                        On=1;
                        continue;
                    case 11:
                        SlideTrans2(x,i);
                        TableTransOff(15,4,37,13);
                        TopList();
                        key=0;
                        On=1;
                        continue;
                    }
                }

            }
        }
    }
    else
    {
        int key=0;
        int i=8;
        int x=23;
        int On=0;
        TableTransOn(15,4,37,13);
        while(key!=13)
        {
            if(On==1)
            {
                TableTransOn(15,4,37,13);
                On=0;
            }
            SetColor(ColorBackGroundTable,ColorText);
            gotoxy(x-3,6);
            printf("%c%c%c%c MENU %c%c%c%c",240,240,240,240,240,240,240,240);
            gotoxy(x,8);
            printf("NEWGAME");
            gotoxy(x,9);
            printf("SETTING");
            gotoxy(x,10);
            printf("TOPLIST");
            gotoxy(33,i);
            printf("%c",CLICK);

            if(kbhit())
            {
                key=getch();
                if(key==115)
                {
                    i++;
                    if(i>=10)
                        i=10;
                    Clrxy(33,i-1);
                    gotoxy(33,i);
                    printf("%c",CLICK);
                }
                else if(key==119)
                {
                    i--;
                    if(i<=8)
                        i=8;
                    Clrxy(33,i+1);
                    gotoxy(33,i);
                    printf("%c",CLICK);
                }
                else if(key==13)
                {
                    switch(i)
                    {
                    case 8:
                        SlideTrans2(x,i);
                        NEWGAME=1;
                        Continue=0;
                        TableTransOff(15,4,37,13);
                        break;
                    case 9:
                        SlideTrans2(x,i);
                        TableTransOff(15,4,37,13);
                        Setting();
                        key=0;
                        On=1;
                        continue;
                    case 10:
                        SlideTrans2(x,i);
                        TableTransOff(15,4,37,13);
                        TopList();
                        key=0;
                        On=1;
                        continue;
                    }
                }

            }
        }
    }
}
void ResetParameter(int *Scores,short *direction,short *Element,short *Count,ToaDo *Snakes, ToaDo *Food)
{
    Scores[0]=0;
    Scores[1]=0;
    Scores[2]=0;
    Scores[3]=0;
    *direction=0;
    StatusFood=0;
    *Element=2;
    *Count=0;
    Snakes[0].x=GetRandom(4,50);
    Snakes[0].y=GetRandom(2,15);
    Snakes[1].x=Snakes[0].x-1;
    Snakes[1].y=Snakes[0].y;
    Snakes[2].x=Snakes[1].x-1;
    Snakes[2].y=Snakes[1].y;
    Food->x=GetRandom(2,50);
    Food->y=GetRandom(2,15);
}

void TopList()
{
    if(Music)
        PlaySound("audio game/Badguy3",NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);

    User Player[4];
    FILE *taptin=NULL;
    int i;
    taptin=fopen("HighScores","r");
    for(i=0; i<=3; i++)
    {
        fscanf(taptin,"%s %d",Player[i].Name,&Player[i].Scores);
    }
    fclose(taptin);
    SetColor(ColorBackground,ColorText);
    ClrTable(15,4,37,13);
    SetColor(ColorBackGroundTable,ColorTable);
    ClrTable(13,3,38,14);
    DrawTable(13,3,38,14);
    int key;
    while(key!=13)
    {
        if(kbhit())
            key=getch();
        SetColor(ColorBackGroundTable,ColorText);
        gotoxy(21,4);
        printf(" TOP LIST ");
        int a=4,b=4;
        for(i=3; i>=0; i--)
        {
            gotoxy(20,a+=2);
            printf("%s",Player[i].Name);
            gotoxy(30,b+=2);
            printf("%d",Player[i].Scores);
        }
    }
    PlaySound(NULL,0,0);
    TableTransOff(13,3,38,14);
}
void SetConsole()
{
    ShowScrollbar(FALSE);
    DisableCtrButton(0,1,1);
    DisableResizeWindow();
    ShowCur(FALSE);
    SetConsoleTitleW(L"GAME SNAKES");
}
void SelectSlide1(int i,int x)
{
    switch(i)
    {
    case 8:
        Clrline(x,32,9);
        gotoxy(x,9);
        printf("NEWGAME");
        Clrline(x,32,8);
        gotoxy(x+1,8);
        printf("CONTINUE");
        break;
    case 9:
        Clrline(x,32,8);
        gotoxy(x,8);
        printf("CONTINUE");
        Clrline(x,32,10);
        gotoxy(x,10);
        printf("SETTING");
        Clrline(x,32,9);
        gotoxy(x+2,9);
        printf("NEWGAME");
        break;
    case 10:
        Clrline(x,32,9);
        gotoxy(x,9);
        printf("NEWGAME");
        Clrline(x,32,11);
        gotoxy(x,11);
        printf("TOPLIST");
        Clrline(x,32,10);
        gotoxy(x+2,10);
        printf("SETTING");
        break;
    case 11:
        Clrline(x,32,10);
        gotoxy(x,10);
        printf("SETTING");
        Clrline(x,32,11);
        gotoxy(x+2,11);
        printf("TOPLIST");
        break;
    }
}
void SelectSlide2(int i,int x)
{
    switch(i)
    {
    case 8:
        Clrline(x,32,9);
        gotoxy(x,9);
        printf("SETTING");
        Clrline(x,32,8);
        gotoxy(x+2,8);
        printf("NEWGAME");
        break;
    case 9:
        Clrline(x,32,8);
        gotoxy(x,8);
        printf("NEWGAME");
        Clrline(x,32,10);
        gotoxy(x,10);
        printf("TOPLIST");
        Clrline(x,32,9);
        gotoxy(x+2,9);
        printf("SETTING");
        break;
    case 10:
        Clrline(x,32,9);
        gotoxy(x,9);
        printf("SETTING");
        Clrline(x,32,10);
        gotoxy(x+2,10);
        printf("TOPLIST");
        break;
    }
}
void SlideTrans1(int x,int i)
{
    while(x!=37)
    {
        SetColor(ColorBackGroundTable,ColorText);
        Sleep(10);
        Clrline(x,x+6,i);
        x++;
        gotoxy(x,i);
        switch(i)
        {
        case 8:
            printf("NEWGAME");
            break;
        case 9:
            printf("SETTING");
            break;
        case 10:
            printf("TOPLIST");
            break;
        }
        gotoxy(37,i);
        printf("%c",177);
        SetColor(ColorBackground,ColorBackground);
        Clrline(38,50,i);
        SetColor(ColorBackGroundTable,ColorTable);
    }
}
void SlideTrans2(int x,int i)
{
    while(x!=37)
    {
        SetColor(ColorBackGroundTable,ColorText);
        Sleep(10);
        Clrline(x,x+6,i);
        x++;
        gotoxy(x,i);
        switch(i)
        {
        case 8:
            printf("CONTINUE");
            break;
        case 9:
            printf("NEWGAME");
            break;
        case 10:
            printf("SETTING");
            break;
        case 11:
            printf("TOPLIST");
            break;
        }
        gotoxy(37,i);
        printf("%c",177);
        SetColor(ColorBackground,ColorBackground);
        Clrline(38,50,i);
        SetColor(ColorBackGroundTable,ColorTable);
    }
}
void TableTransOff(int x0,int y0,int x,int y)
{
    while(y0!=9&&y!=8)
    {
        DrawTable(x0,y0,x,y);
        Sleep(10);
        SetColor(ColorBackground,ColorBackground);
        Clrline(x0,x,y0);
        Clrline(x0,x,y);
        SetColor(ColorBackGroundTable,ColorTable);
        y0++;
        y--;
        Sleep(10);
    }
    DrawTable(x0,9,x,7);
    Sleep(10);
    SetColor(ColorBackground,ColorBackground);
    Clrline(x0,x,9);
    ClrTable(TABLE_X0,TABLE_Y0,TABLE_X,TABLE_Y);
}
void TableTransOn(int X0,int Y0,int X,int Y)
{
    int y0=8,y=9;
    int x0=26,x=26;
    SetColor(ColorBackGroundTable,ColorTable);
    while(x0!=X0&&x!=X)
    {
        gotoxy(x0,y0);
        printf("%c",TABLE);
        gotoxy(x0,y);
        printf("%c",TABLE);
        gotoxy(x,y0);
        printf("%c",TABLE);
        gotoxy(x,y);
        printf("%c",TABLE);
        x0--;
        x++;
        Sleep(10);
    }
    while(y0!=Y0&&y!=Y)
    {
        DrawTable(X0,y0,X,y);
        Sleep(10);
        Clrline(X0,X,y0);
        Clrline(X0,X,y);
        y0--;
        y++;
    }
    SetColor(ColorBackground,ColorWall);
    ClrTable(WALL_X0+1,WALL_Y0+1,WALL_X-1,WALL_Y-1);
    SetColor(ColorBackGroundTable,ColorTable);
    ClrTable(X0,Y0,X,Y);
    DrawTable(X0,Y0,X,Y);
}

void SetSnakeType()
{
    int i=5;
    int key=0;
    int x=12;

    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(x+11,5);
    printf("COLOR");
    SetColor(ColorBackGroundTable,ColorSnakes);
    gotoxy(x+13,7);
    printf("%c",254);
    SetColor(ColorBackGroundTable,ColorFood);
    gotoxy(x+13,9);
    printf("%c",254);
    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(x+13,11);
    printf("%c",254);
    SetColor(ColorBackGroundTable,ColorTable);
    TableScreen();
    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(x+5,i);
    printf("%c",CLICK);

    while(TRUE)
    {
        SetColor(ColorBackGroundTable,ColorText);
        gotoxy(x,5);
        printf("TYPE");
        gotoxy(x-1,7);
        printf("Snake%d",SnakeType);
        gotoxy(x-1,9);
        printf("Food%d",FoodType);
        gotoxy(x,11);
        printf("Text");
        gotoxy(x+4,13);
        printf("Back %c",27);

        if(kbhit())
        {
            key=getch();
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);
        }

        if(key==115)
        {
            i+=2;
            if(i>=13)
                i=13;
            SetColor(ColorBackGroundTable,ColorBackGroundTable);
            Clrxy(x-2,i-2);
            Clrxy(x+5,i-2);
            SetColor(ColorBackGroundTable,ColorText);
            if(i!=13)
            {
                gotoxy(x-2,i);
                printf("%c",CLICK);
                gotoxy(x+5,i);
                printf("%c",020);
            }
            else
            {
                gotoxy(x+12,13);
                printf("%c",CLICK);
            }

            key=0;
        }
        else if(key==119)
        {
            i-=2;
            if(i<=5)
                i=5;
            SetColor(ColorBackGroundTable,ColorBackGroundTable);
            if(i!=11)
            {
                Clrxy(x+5,i+2);
                Clrxy(x-2,i+2);
            }
            else
                Clrxy(x+12,13);
            SetColor(ColorBackGroundTable,ColorText);
            if(i!=5)
            {
                gotoxy(x+5,i);
                printf("%c",020);
                gotoxy(x-2,i);
                printf("%c",CLICK);
            }
            else
            {
                gotoxy(x+5,i);
                printf("%c",CLICK);
            }
            key=0;
        }
        else if(key==97)
        {
            if(i!=5)
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrline(x,x+4,i);
            }
            else
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrxy(x+17,i);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+5,i);
                printf("%c",CLICK);
            }
            if(i==7)
            {
                SnakeType--;
                if(SnakeType<=0)
                    SnakeType=0;
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrline(x-1,x+4,i);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-1,i);
                printf("Snake%d",SnakeType);

            }
            if(i==9)
            {
                FoodType--;
                if(FoodType<=0)
                    FoodType=0;
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrline(x-1,x+4,i);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-1,i);
                printf("Food%d",FoodType);
            }
            key=0;
            TableScreen();
        }
        else if(key==100)
        {
            if(i!=5)
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrline(x,x+3,i);
            }
            else
            {
                Clrxy(x+5,i);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+17,i);
                printf("%c",CLICK);
                SetColorType();
                break;
            }

            if(i==7)
            {
                SnakeType++;
                if(SnakeType>=3)
                    SnakeType=3;
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrline(x-1,x+4,i);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-1,i);
                printf("Snake%d",SnakeType);
            }
            if(i==9)
            {
                FoodType++;
                if(FoodType>=3)
                    FoodType=3;
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrline(x-1,x+3,i);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-1,i);
                printf("Food%d",FoodType);
            }
            key=0;
            TableScreen();
        }
        else if(key==13)
        {
            if(i==13)
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(6,4,46,13);
                break;
            }
        }

    }
}
void SetColorType()
{
    int i=5;
    int x=25;
    int key=0;
    while(TRUE)
    {
        if(kbhit())
        {
            key=getch();
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);
        }
        if(key==115)
        {
            SetColor(ColorBackGroundTable,ColorBackGroundTable);
            i+=2;
            if(i>=13)
                i=13;
            if(i==7)
            {
                Clrxy(x+4,i-2);
            }
            else
            {
                Clrxy(x-2,i-2);
                Clrxy(x+2,i-2);
            }


            if(i!=13)
            {
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-2,i);
                printf("%c",CLICK);
                gotoxy(x+2,i);
                printf("%c",020);
            }
            else
            {
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-1,13);
                printf("%c",CLICK);
            }

            key=0;
        }
        else if(key==119)
        {
            SetColor(ColorBackGroundTable,ColorBackGroundTable);
            i-=2;
            if(i<=5)
                i=5;
            if(i!=11)
            {
                Clrxy(x+2,i+2);
                Clrxy(x-2,i+2);
            }
            else
                Clrxy(x-1,i+2);
            if(i!=5)
            {
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+2,i);
                printf("%c",020);
                gotoxy(x-2,i);
                printf("%c",CLICK);
            }
            else
            {
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+4,i);
                printf("%c",CLICK);
            }

            key=0;
        }
        else if(key==97)
        {
            if(i==5)
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrxy(x+4,i);
                SetSnakeType();
                break;
            }

            if(i==7)
            {
                Clrxy(x,i);
                ColorSnakes--;
                if(ColorSnakes<=0)
                    ColorSnakes=0;
                SetColor(ColorBackGroundTable,ColorSnakes);
                gotoxy(x,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorTable);
            }

            if(i==9)
            {
                Clrxy(x,i);
                ColorFood--;
                if(ColorFood<=0)
                    ColorFood=0;
                SetColor(ColorBackGroundTable,ColorFood);
                gotoxy(x,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorTable);
            }

            if(i==11)
            {
                Clrxy(x,i);
                ColorText--;
                if(ColorText<=0)
                    ColorText=0;
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-13,5);
                printf("TYPE");
                gotoxy(x-14,7);
                printf("Snake%d",SnakeType);
                gotoxy(x-14,9);
                printf("Food%d",FoodType);
                gotoxy(x-13,11);
                printf("Text");
                gotoxy(x-9,13);
                printf("Back %c",27);
                gotoxy(x-2,5);
                printf("COLOR");
                SetColor(ColorBackGroundTable,ColorTable);
            }
            key=0;
            TableScreen();
        }
        else if(key==100)
        {
            switch(i)
            {
            case 7:
                Clrxy(x,i);
                ColorSnakes++;
                if(ColorSnakes>=15)
                    ColorSnakes=15;
                SetColor(ColorBackGroundTable,ColorSnakes);
                gotoxy(x,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorTable);
                break;
            case 9:
                Clrxy(x,i);
                ColorFood++;
                if(ColorFood>=15)
                    ColorFood=15;
                SetColor(ColorBackGroundTable,ColorFood);
                gotoxy(x,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorTable);
                break;
            case 11:
                Clrxy(x,i);
                ColorText++;
                if(ColorText>=15)
                    ColorText=15;
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x-13,5);
                printf("TYPE");
                gotoxy(x-14,7);
                printf("Snake%d",SnakeType);
                gotoxy(x-14,9);
                printf("Food%d",FoodType);
                gotoxy(x-13,11);
                printf("Text");
                gotoxy(x-9,13);
                printf("Back %c",27);
                gotoxy(x-2,5);
                printf("COLOR");
                SetColor(ColorBackGroundTable,ColorTable);
                break;
            }
            key=0;
            TableScreen();
        }
        else if(key==13)
        {
            if(i==13)
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(6,4,46,13);
                break;
            }
        }
    }
}
void TableScreen()
{
    SetColor(ColorBackground,ColorBackground);
    ClrTable(33,5,43,11);

    switch(SnakeType)
    {
    case 0:
        Head=254;
        Body=254;
        Tail=254;
        break;
    case 1:
        Head=148;
        Body=111;
        Tail=45;
        break;
    case 2:
        Head=94;
        Body=111;
        Tail=45;
        break;
    case 3:
        Head=154;
        Body=233;
        Tail=233;
        break;
    }

    switch(FoodType)
    {
    case 0:
        FOOD=254;
        break;
    case 1:
        FOOD=003;
        break;
    case 2:
        FOOD=001;
        break;
    case 3:
        FOOD=148;
        break;
    }
    SetColor(ColorBackground,ColorSnakes);
    gotoxy(38,7);
    printf("%c",Head);
    SetColor(ColorBackground,ColorSnakes);
    gotoxy(37,7);
    printf("%c",Body);
    SetColor(ColorBackground,ColorSnakes);
    gotoxy(36,7);
    printf("%c",Tail);
    SetColor(ColorBackground,ColorFood);
    gotoxy(39,9);
    printf("%c",FOOD);
    SetColor(ColorBackGroundTable,ColorTable);
}
void SetColorGame()
{
    int i=5;
    int key=0;
    int x=12;
    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(x+27,i);
    printf("%c",020);
    gotoxy(x+23,i);
    printf("%c",CLICK);

    while(key!=13)
    {
        SetColor(ColorBackGroundTable,ColorText);
        gotoxy(x,5);
        printf("Background Game");
        gotoxy(x,7);
        printf("Wall");
        gotoxy(x,9);
        printf("Table");
        gotoxy(x,11);
        printf("Background Table");
        gotoxy(x+10,13);
        printf("Back %c",27);
        SetColor(ColorBackGroundTable,ColorBackground);
        gotoxy(x+25,5);
        printf("%c",254);
        SetColor(ColorBackGroundTable,ColorWall);
        gotoxy(x+25,7);
        printf("%c",254);
        SetColor(ColorBackGroundTable,ColorTable);
        gotoxy(x+25,9);
        printf("%c",254);
        SetColor(ColorBackGroundTable,ColorBackGroundTable);
        gotoxy(x+25,11);
        printf("%c",254);

        if(kbhit())
        {
            key=getch();
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);
        }

        if(key==115)
        {
            i+=2;
            if(i>=13)
                i=13;
            SetColor(ColorBackGroundTable,ColorTable);
            Clrxy(x+23,i-2);
            Clrxy(x+27,i-2);
            SetColor(ColorBackGroundTable,ColorText);

            if(i==13)
            {
                gotoxy(x+20,13);
                printf("%c",CLICK);
            }
            else
            {
                gotoxy(x+27,i);
                printf("%c",020);
                gotoxy(x+23,i);
                printf("%c",CLICK);
            }
            key=0;
        }
        else if(key==119)
        {
            SetColor(ColorBackGroundTable,ColorTable);
            i-=2;
            if(i<=5)
                i=5;
            if(i==11)
                Clrxy(x+20,13);
            Clrxy(x+27,i+2);
            Clrxy(x+23,i+2);
            SetColor(ColorBackGroundTable,ColorText);
            gotoxy(x+27,i);
            printf("%c",020);
            gotoxy(x+23,i);
            printf("%c",CLICK);
            key=0;
            SetColor(ColorBackGroundTable,ColorTable);

        }
        else if(key==97)
        {
            if(i==5)
            {

                ColorBackground--;
                if(ColorBackground<=0)
                    ColorBackground=0;

                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorBackground);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackground,ColorBackground);
                ClrTable(0,0,53,18);
                SetColor(ColorBackground,ColorWall);
                DrawWall(1,1,51,16);
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(5,3,47,14);
                SetColor(ColorBackGroundTable,ColorTable);
                DrawTable(5,3,47,14);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+27,i);
                printf("%c",020);
                gotoxy(x+23,i);
                printf("%c",CLICK);
                SetColor(ColorBackGroundTable,ColorTable);
                key=0;
            }
            if(i==7)
            {
                ColorWall--;
                if(ColorWall<=0)
                    ColorWall=0;
                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorWall);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackground,ColorWall);
                DrawWall(1,1,51,16);
                SetColor(ColorBackGroundTable,ColorTable);
                key=0;
            }
            if(i==9)
            {
                ColorTable--;
                if(ColorTable<=0)
                    ColorTable=0;

                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorTable);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorTable);
                DrawTable(5,3,47,14);
                SetColor(ColorBackGroundTable,ColorTable);
                key=0;
            }
            if(i==11)
            {
                ColorBackGroundTable--;
                if(ColorBackGroundTable<=0)
                    ColorBackGroundTable=0;

                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(6,4,46,13);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+27,i);
                printf("%c",020);
                gotoxy(x+23,i);
                printf("%c",CLICK);
                SetColor(ColorBackGroundTable,ColorTable);
                key=0;
            }
        }
        else if(key==100)
        {
            switch(i)
            {
            case 5:
                ColorBackground++;
                if(ColorBackground>=15)
                    ColorBackground=15;

                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorBackground);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackground,ColorBackground);
                ClrTable(0,0,53,18);
                SetColor(ColorBackground,ColorWall);
                DrawWall(1,1,51,16);
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(5,3,47,14);
                SetColor(ColorBackGroundTable,ColorTable);
                DrawTable(5,3,47,14);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+27,i);
                printf("%c",020);
                gotoxy(x+23,i);
                printf("%c",CLICK);
                SetColor(ColorBackGroundTable,ColorTable);

                key=0;
                break;
            case 7:
                ColorWall++;
                if(ColorWall>=15)
                    ColorWall=15;

                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorWall);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackground,ColorWall);
                DrawWall(1,1,51,16);
                SetColor(ColorBackGroundTable,ColorTable);

                key=0;
                break;
            case 9:
                ColorTable++;
                if(ColorTable>=15)
                    ColorTable=15;

                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorTable);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorTable);
                DrawTable(5,3,47,14);
                SetColor(ColorBackGroundTable,ColorTable);
                key=0;
                break;
            case 11:
                ColorBackGroundTable++;
                if(ColorBackGroundTable>=16)
                    ColorBackGroundTable=16;
                Clrxy(x+25,i);
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                gotoxy(x+25,i);
                printf("%c",254);
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(6,4,46,13);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+27,i);
                printf("%c",020);
                gotoxy(x+23,i);
                printf("%c",CLICK);
                SetColor(ColorBackGroundTable,ColorTable);
                key=0;
                break;
            }
        }
        else if(key==13)
        {
            if(i==13)
            {
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                ClrTable(6,4,46,13);
                break;
            }
            key=0;

        }

    }
}
void Setting()
{
    int key=0;
    int j=7;
    int x=20;

    SetColor(ColorBackGroundTable,ColorTable);
    TableTransOn(5,3,47,14);

    while(key!=13)
    {
        SetColor(ColorBackGroundTable,ColorText);
        gotoxy(x+10,j);
        printf("%c",CLICK);
        gotoxy(x+2,5);
        printf("SETTINGS");
        gotoxy(x+3,7);
        printf("Style");
        gotoxy(x+3,9);
        printf("Color");
        gotoxy(x+3,11);
        printf("Volume");
        gotoxy(x+3,13);
        printf("Back %c",27);
        if(kbhit())
        {
            key=getch();
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);

            if(key==115)
            {
                j+=2;
                if(j>=13)
                {
                    j=13;
                }
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrxy(x+10,j-2);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+10,j);
                printf("%c",CLICK);
                key=0;
            }
            else if(key==119)
            {
                j-=2;
                if(j<=7)
                {
                    j=7;
                }
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrxy(x+10,j+2);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(x+10,j);
                printf("%c",CLICK);
                key=0;
            }
            else if(key==13)
            {
                switch(j)
                {
                case 7:
                    SetColor(ColorBackGroundTable,ColorBackGroundTable);
                    ClrTable(6,4,46,13);
                    SetColor(ColorBackGroundTable,ColorText);
                    SetSnakeType();
                    key=0;
                    continue;
                case 9:
                    SetColor(ColorBackGroundTable,ColorBackGroundTable);
                    ClrTable(6,4,46,13);
                    SetColor(ColorBackGroundTable,ColorText);
                    SetColorGame();
                    key=0;
                    continue;
                case 11:
                    SetColor(ColorBackGroundTable,ColorBackGroundTable);
                    ClrTable(6,4,46,13);
                    SetColor(ColorBackGroundTable,ColorText);
                    SetVolume();
                    key=0;
                    continue;
                case 13:
                    SetColor(ColorBackGroundTable,ColorBackGroundTable);
                    TableTransOff(5,3,47,14);
                    break;
                }
            }
        }
    }
}


void SetVolume()
{
    int x=20;
    int i=7;
    int key=0;
    SetColor(ColorBackGroundTable,ColorText);
    gotoxy(x+15,i);
    printf("%c",CLICK);
    gotoxy(x+3,5);
    printf("VOLUME");
    gotoxy(x+1,7);
    printf("MUSIC");
    gotoxy(x+10,7);
    if(Music)
    {
        printf("On");
    }
    else
    {
        printf("Off");
    }
    gotoxy(x+1,9);
    printf("SOUND");
    gotoxy(x+10,9);
    if(Sound)
    {
        printf("On");
    }
    else
    {
        printf("Off");
    }
    gotoxy(x+3,11);
    printf("Back %c",27);

    while(TRUE)
    {
        if(kbhit())
        {
            key=getch();
            if(Sound)
                PlaySound("audio game/Click",NULL,SND_FILENAME|SND_ASYNC);

            if(key==115)
            {
                i+=2;
                if(i>=11)
                {
                    i=11;
                }
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrxy(35,i-2);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(35,i);
                printf("%c",CLICK);
                key=0;
            }
            else if(key==119)
            {
                i-=2;
                if(i<=7)
                {
                    i=7;
                }
                SetColor(ColorBackGroundTable,ColorBackGroundTable);
                Clrxy(35,i+2);
                SetColor(ColorBackGroundTable,ColorText);
                gotoxy(35,i);
                printf("%c",CLICK);
                key=0;
            }
            else if(key==13)
            {
                if(i==7)
                {
                    Clrline(30,32,i);
                    gotoxy(30,i);
                    if(Music)
                    {
                        printf("Off");
                        Music=0;
                    }
                    else
                    {
                        printf("On");
                        Music=1;
                    }

                }
                else if(i==9)
                {
                    Clrline(30,32,i);
                    gotoxy(30,i);
                    if(Sound)
                    {
                        printf("Off");
                        Sound=0;
                    }
                    else
                    {
                        printf("On");
                        Sound=1;
                    }
                }
                else if(i==11)
                {
                    ClrTable(6,4,46,13);
                    break;
                }
            }
        }
    }
}







