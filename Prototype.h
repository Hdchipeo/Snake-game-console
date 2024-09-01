typedef struct ToaDo ToaDo;
struct ToaDo
{
    int x;
    int y;
    int x0;
    int y0;
};
typedef struct User User;
struct User
{
    char Name[10];
    int Scores;
};
typedef enum GetColor GetColor;
enum GetColor
{
    BLACK,BLUE,GREEN,AQUA,RED,PURPLE,YELLOW,WHILE,GRAY,LBLUE,LGREEN,LAQUA,LRED,LPURPLE,LYELLOW,BWHILE
};

void Remote( short *direction,ToaDo Food);
void DrawBody(int x, int y);
void DrawFood(int *x,int *y,ToaDo *Snakes, short Element);
void DrawHead(int x,int y,ToaDo *Snakes,ToaDo Food);
void ClrHead(int x,int y);
void DrawTail(int x,int y);
void Clrxy(int x,int y);
void gotoxy(int x,int y);//x,y là tọa điểm x,y trên màn hình
void SnakesMove(ToaDo *Snakes, short *Element, short *direction,ToaDo Food);
int GetRandom(int MIN,int MAX);
void DrawWall(int x0,int y0,int x,int y);
void GetScores(int *Scores, short Count);
void DrawTable(int x0,int y0,int x,int y);
void ClrTable(int x0,int y0,int x,int y);
void SelectMode();
void Clrline(int x0,int x,int y);
void ClassicMode(ToaDo *Snakes);
void ModernMode(ToaDo *Snakes);
void TableOverGame();
void TablePauseGame(ToaDo Food);
void IntroGame();
void SetWindowSize(SHORT width, SHORT height);
void SetScreenBufferSize(short width, short height);
void DisableResizeWindow();
void DisableCtrButton(int Close, int Min, int Max);
void ShowScrollbar(WINBOOL Show);
void ShowCur(WINBOOL CursorVisibility);
void HighScores( short Count);
void ArrangeArray(User *Player);
void SetColor(int backgound_color, int text_color);
void ReadParameter( short *direction, short *Element, short *Count,int *Scores,ToaDo *Snakes,ToaDo *Food);
void WriteParameter( short direction, short Element, short Count,int *Scores, ToaDo *Snakes, ToaDo Food);
void Menu();
void ResetParameter(int *Scores,  short *direction, short *Element,  short *Count,ToaDo *Snakes, ToaDo *Food);
void TopList();
void SetConsole();
void SelectSlide1(int i,int x);
void SelectSlide2(int i,int x);
void SlideTrans1(int x,int i);
void SlideTrans2(int x,int i);
void TableTransOff(int x0,int y0,int x,int y);
void TableTransOn(int X0,int Y0,int X,int Y);
void Setting();
void SetSnakeType();
void SetColorType();
void TableScreen();
void SetColorGame();
void SetVolume();

