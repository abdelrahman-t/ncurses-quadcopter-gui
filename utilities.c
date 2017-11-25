#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <stddef.h>
#include "utilities.h"

WINDOW* initializeTerminal(){
	
	setlocale(LC_ALL, "");

	WINDOW* window = initscr();

	if(window == NULL)
		;
	
	curs_set(2);
	
    start_color();
    use_default_colors();

	init_pair(1,COLOR_GREEN,COLOR_BLACK);		//radio active green
	init_pair(2,COLOR_CYAN,COLOR_BLACK);		//cyan
	init_pair(3,COLOR_MAGENTA,COLOR_BLACK);		//deep purple

  	init_pair(5,COLOR_BLACK,COLOR_YELLOW);		//yellow is grey!!!
	init_pair(10,COLOR_WHITE,COLOR_BLACK);		//background colors

    bkgd(COLOR_PAIR(10));

	return window;
}

void drawRectangleAround(WINDOW* win, Block block ) {

	int height = block.height - 2,
		width  = block.width  - 2,
		y = block.y,
		x = block.x;

    drawHorizontalLine(win, y,x,width+1,LINE);         // box top side
	drawHorizontalLine(win, y+height+1,x,width+1,LINE);// box bottom side

	drawVerticalLine(win, y,x,height,LINE);            // box right side
	drawVerticalLine(win, y,x+width+1,height,LINE);    // box left side

	mvwprintw(win, y,x,"\u256D");                       // left-top corner
	mvwprintw(win, y,x+width+1,"\u256E");               // right-top corner
	mvwprintw(win, y+height+1,x,"\u2570");              // bottom-left corner
	mvwprintw(win, y+height+1,x+width+1,"\u256F");      // bottom-right corner  
}

void splitRectangle(WINDOW* win, Block rectangle, int numberOfRows, int numberOfColumns){

	int height = rectangle.height,
		width  = rectangle.width,
		y = rectangle.y,
		x = rectangle.x;
    
	int rowSpacing = (height)/(numberOfRows);

    
	int i;// drawing rows
	for(i = 1; i < numberOfRows; ++i){
        mvwaddch(win, y+(rowSpacing*i), x, ACS_LTEE);
		drawHorizontalLine(win, y+(rowSpacing*i),1+x ,width-2,LINE);
		waddch(win, ACS_RTEE);	
	}

	int columnSpacing = (width)/numberOfColumns;

    
	int j;// drawing columns
	for(j = 1; j < numberOfColumns; ++j){

        mvwaddch(win, y, x+(columnSpacing*j), ACS_TTEE);

		drawVerticalLine(win, y, x+(columnSpacing*j) ,height-2,LINE);
        mvwaddch(win, y+height-1, x+(columnSpacing*j), ACS_BTEE);
	}

	if(numberOfRows > 1)// repairing rows-columns intersections
	{
		int k;
		for(k=1;k<numberOfColumns;k++)
		{
			int z;
			for(z=1;z<numberOfRows;z++)
                mvwaddch(win, y+(rowSpacing*z),x+(columnSpacing*k), ACS_PLUS);
		}
	}
}

void drawVerticalLine(WINDOW* win, int y, int x, int height ,int f){

	wchar_t c;

    if(f == 0)
		c = "\u2502";
	else
		c = "\u2592";

	int i;
	for( i = 0; i < height; ++i )
		mvwprintw(win,y+i+1,x,c);
}

void drawHorizontalLine(WINDOW* win, int y, int x, int width , int f){

    wchar_t c;

	if(f == 0)
		c = "\u2500";
	else
		c = "\u2592";

	move( y, x );

    int j;
	for (j = 0; j < width; ++j)
		mvwprintw(win, y,x+j,c);
}

void printVertically(char* str,int length,int y,int x){

	int i;
	for(i=0;i<length;i++)
		mvaddch(y+i,x,str[i]);
}
