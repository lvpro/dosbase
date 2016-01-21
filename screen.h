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

/* Miscellaneous Screen-Interface Function Prototypes */

void init_console(void);	/* Initialize The Console */
void db_clrscr(void);	/* Clears The Console Screen */
void db_gotoxy(short int x, short int y);	/* Sets Cursor To (x,y) */
void db_textcolor(unsigned short textcolor, unsigned short backcolor);	/* Sets Foreground / Background Color */
void HideCursor(void);	/* Turns Off The Cursor */
void DisplayCursor(void);	/* Turns On The Cursor */
void writeAction(char *message);	/* Write Entry To Action Box */
void printBuffer(void);				/* Print The Buffer Contents */
void addToTotalCount(void);	/* Controls "Entry" Control On Interface */
char* getns(char *inputBuffer);		/* Borland Function Emulation */