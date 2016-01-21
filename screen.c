/**************************************************************************

DOSBase Version 1.5 Final
Copyright (C)1995-2000 Cliny Kennedy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

DOSBase Website: http://revolutionaudio.dhs.org/dosbase

**************************************************************************/

/* Miscellaneous Screen-Interface Functions */

/****************************************************************************
   Most of these functions were written in order to port the code from 
   Borland 5 to Visual C++ 6.0 and use win32 console-related API calls to 
   implement features which were natively supported under Borland 5,
   such as db_gotoxy() & color printing. This code is compatible with 
   standard stdio functions (printf, etc), which ultimately are implemented
   by Windows calling WriteConsoleOutput(). I recommend viewing this module
   under a DOS text-editor as it includes older ASCII line-drawing 
   characters that will not show up properly in a windows based IDE.
****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include "screen.h"
#include "defs.h"

extern int entries;

static void scrollBox(char *message);	/* Scroll Contents Of Action Box */

HANDLE consoleScreen;
char contents[11][73];	/* Contents Of Action Box */

/* Initialize The Console */
void init_console(void)
{
	consoleScreen = GetStdHandle(STD_OUTPUT_HANDLE);
}

/* Clears The Console Window */
void db_clrscr(void)
{
	unsigned int i;	/* Loop Counter */

	db_textcolor(BLACK, BG_BLACK);

	db_gotoxy(1, 1);	/* Start at (1,1) */

	for(i=0; i<25; i++)
		printf("%80c", ' ');	/* Flood The Screen With Spaces */

	db_gotoxy(1, 1);	/* Reposition Cursor To Home Position */
}

/*  Sets the cursor position specified to (x, y) */
void db_gotoxy(short int x, short int y)
{
	COORD coordinates;

	coordinates.X = x-1;	/* 0-based Offset */
	coordinates.Y = y-1;	/* O-based Offset */

	SetConsoleCursorPosition(consoleScreen, coordinates);
}

/* Sets the foreground / background color */
void db_textcolor(unsigned short db_textcolor, unsigned short backcolor)
{
	SetConsoleTextAttribute(consoleScreen, (unsigned short)(db_textcolor | backcolor));		
}

/*  Hides the cursor */
void HideCursor(void)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleScreen, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleScreen, &cursorInfo);
}

/*  Unhides the cursor */
void DisplayCursor(void)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleScreen, &cursorInfo);
	cursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(consoleScreen, &cursorInfo);
}

/****************************************************************************
This function writes to the main action box.  It keeps track of current
position and automatically scrolls the box when needed.
****************************************************************************/
void writeAction(char *message)
{
	static char row = 7;		/* Max Row = 17 */

	if (row == 18)
	{
		scrollBox(message);	/* Scroll the contents of the buffer */
		printBuffer();		/* Reprint the buffer */
	}

	else 
	{
		db_gotoxy(5, row);		/* Goto The Row To Print On */
		strcpy(contents[row-7], message);	/* Copy It Into The Contents Buffer */
		row++;
		db_textcolor(WHITE, BG_BLACK);	/* Set Color To White */
		printf("%s\n", message);	/* Print The Message */
	}
}

/****************************************************************************
The scrollBox() function scrolls the contents of the string buffer IF the
screen needs to be scrolled.
****************************************************************************/
static void scrollBox(char *message)
{
   char row;

   for (row = 0; row < 10; row++)	/* Move Each Row Up */
   {
       strcpy(contents[row], contents[row+1]);
   }

   strcpy(contents[10], message);	/* Attach The New Message */
}

/****************************************************************************
The printBuffer() routines displays the contents of the buffer to the
action box.
****************************************************************************/
void printBuffer(void)
{
   char entry;

   db_textcolor(WHITE, BG_BLACK);	/* Prepare For Color Printing */

   for (entry = 0; entry < 11; entry++)	/* Print Contents Of Action Box */
   {
      db_gotoxy(5, (short int)(entry+7));
      printf("%-73s\n", contents[entry]);
   }
}

/* Increments Entry Counter On Main Interface */
void addToTotalCount(void)
{
	entries++;
	db_gotoxy(25, 4);
	db_textcolor(YELLOW, BG_BLACK);
	printf("%d", entries);
}

/****************************************************************************
   getns() is used as a custom replacement for cgets().  It functions *exactly*
   the same as cgets() or _cgets(), but it also looks for ESC for terminating
   a dialog.  It operates exactly like cgets(), but returns -1 in [1] of 
   inputBuffer if user hits escape.
****************************************************************************/
char* getns(char *inputBuffer)
{
	int maxChar;
	int charCount = 0;
	char inChar;

	maxChar = *(inputBuffer);

	do {

	/* Enter Input Loop */

	while (charCount < maxChar)
	{
		inChar = _getch();

		/* The following block of code checks for control keys, etc
		and if found clears the keyboard buffer and attempts to 
		read another valid character until one is given. */
      
		while (inChar == 0)
		{
			getch();
			inChar = getch();
		}
      
		/* Check For ESC */

		if (inChar == 0x1B) 
		{
			inputBuffer[1] = -1;
			inputBuffer[2] = '\0';
			return NULL;
		}

		/* Check If Enter Was Pressed */
		if (inChar == 0x0D) 
		{
			*(inputBuffer + 2 + charCount) = '\0';	/* NULL out string */
			inputBuffer[1] = charCount;
			return &inputBuffer[2];	/* Input Completed */
		}
		
		/* Check If Backspace Was Pressed */
		if (inChar == 0x08)
		{
			/* First confirm inputBuffer isn't already empty */
			if (charCount != 0)
			{
				/* If Not, Clear Last Character As Requested */
				printf("\b \b");
				charCount--;	/* Remove from inputBuffer */
			}
		}

		/* If Niether Enter nor Backspace, Confirm Valid Input */
      
		if((((isalnum(inChar) || ispunct(inChar)) && (!(iscntrl(inChar))))
			|| isspace(inChar)))
		{
			*(inputBuffer + 2 + charCount) = inChar;	/* Put in inuptBuffer */
			charCount++;	/* Increment Character Count */
			printf("%c", inChar);
		}
	}

	/* Wait For Enter Then Return */

	inChar = getch();
	while ((inChar != 0x0D) && (inChar != 0x08) && (inChar != 0x1B))
		inChar = getch();

	/* Check For ESC */

	if (inChar == 0x1B) 
	{
		inputBuffer[1] = -1;
		inputBuffer[2] = '\0';
		return NULL;
	}

	/* If ENTER is pressed */

	if (inChar == 0x0D)
	{
		*(inputBuffer + 2 + charCount) = '\0';	/* NULL out string */
		inputBuffer[1] = charCount;
		return &inputBuffer[2];
	}
	else	/* Backspace */
	{
		printf("\b \b");
		charCount--;	/* Remove from inputBuffer */
	}	

	/* End Of Do-While */
	} while (inChar == 0x08);	/* If last character typed is backspace,
								re-enter inupt loop */

   return NULL;		/* Unreachable, but avoids warnings */ 
}


