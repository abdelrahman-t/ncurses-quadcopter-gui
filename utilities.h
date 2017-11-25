#define DEBUG_GUI

#define LINE 0
#define BAR 1

typedef struct{
	int height,width;
	int y,x;
}Block;

WINDOW* initializeTerminal();
void drawRectangleAround(WINDOW* win, Block block);
void splitRectangle(WINDOW* win, Block rectangle, int numberOfRows, int numberOfColumns);
void drawVerticalLine(WINDOW* win, int y, int x, int height ,int f);
void drawHorizontalLine(WINDOW* win, int y, int x, int width , int f);
void printVertically(char* str,int length,int y,int x);