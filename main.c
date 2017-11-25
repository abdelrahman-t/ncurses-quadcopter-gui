#include <ncurses.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include "utilities.h"

#define DEBUG_GUI
#define BUFF_SIZE 15

typedef struct {
    WINDOW* w_terminalInput;
	WINDOW* w_terminalOutput;
}Tuple;

Block terminalInput = {
	.height = 25,
	.width = 50
};

Block terminalOutput = {
.height = 10,
.width = 50
};

Tuple initTerminal(WINDOW* globalWindow){

	int x=95, y=2;
	mvprintw(y,x,"Terminal Input:");
	terminalInput.y = ++y;
	terminalInput.x = x;

    drawRectangleAround(globalWindow, terminalInput);
	WINDOW *w_terminalInput = newwin(terminalInput.height-2,terminalInput.width-2,terminalInput.y+1,terminalInput.x+1);
	refresh();
    wrefresh(w_terminalInput);

	terminalOutput.y = (y=y+terminalInput.height+2);
	terminalOutput.x = x;

	mvprintw(y-1,x,"Terminal Output:");
	drawRectangleAround(globalWindow, terminalOutput);
	WINDOW *w_terminalOutput = newwin(terminalOutput.height-2,terminalOutput.width-2,terminalOutput.y+1,terminalOutput.x+1);
	refresh();
    wrefresh(w_terminalOutput);

    Tuple t = {.w_terminalInput = w_terminalInput, .w_terminalOutput = w_terminalOutput};
	return t;
}

char* serialRead(){
	int ptr;
	char *buff = malloc(BUFF_SIZE);

	if ((ptr = open("/dev/ttyACM1",O_RDONLY)) != NULL)
	{
		size_t byte_count = read(ptr,buff ,BUFF_SIZE-1);
		buff[byte_count] = '\0';
		//printf("%s",buff);
		close(ptr);
		return buff;
	}

	return NULL;
}

unsigned char serialWrite(char *buff){

	FILE *ptr;

	if ((ptr = fopen("/dev/ttyACM1","w")) != NULL)
	{
		fwrite(buff, 1, BUFF_SIZE, ptr);
		return fclose(ptr), 0;
	}
	else
		exit(0);
	return 1;
}
void refreshTerminal(WINDOW *globalWindow, Tuple t){

	WINDOW  *w_terminalInput = t.w_terminalInput,
	        *w_terminalOutput = t.w_terminalOutput;

	wtimeout(w_terminalInput, 0);
	wmove(w_terminalInput,1,6);
	char input[BUFF_SIZE];
	int input_Y = 0, output_Y = 0;
	int nread;
    while(1)
	{	
		if(wgetch(w_terminalInput) != ERR)
		{
			wtimeout(w_terminalInput, 1000);
			mvwprintw(w_terminalInput,input_Y++,0,"PC:");
			nread = wscanw(w_terminalInput,"%s",input);
			if (nread>1){
				printf("%s", input);
				serialWrite(input);
			}
			wtimeout(w_terminalInput, 0);
		}
		if(input_Y == terminalInput.height-2){
			wclear(w_terminalInput);
			input_Y = 0;
		}

		if(output_Y == terminalOutput.height-2){
			wclear(w_terminalOutput);
			output_Y = 0;
		}

		char *buff = serialRead();
		wattron(w_terminalOutput,COLOR_PAIR(1)| A_BOLD);

		if (buff != NULL && strlen(buff) > 3){
			mvwprintw(w_terminalOutput,output_Y++,0,"%s",buff);
			wrefresh(w_terminalOutput);
			usleep(10000);
		}
		wattroff(w_terminalOutput,COLOR_PAIR(1)| A_BOLD);
		
		refresh();
	}
}

int main(){

	WINDOW* globalWindow = initializeTerminal();
	Tuple t = initTerminal(globalWindow);
	refreshTerminal(globalWindow ,t);

	getch();
	endwin();
}