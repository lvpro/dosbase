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

#include <stdio.h>
#include <conio.h>
#include "structs.h"
#include "screen.h"
#include "dialogs.h"
#include "defs.h"

/* FuncKey enumerated datatype for user input: F1-F7 in this case */

enum FuncKey { F8 = 0, F1, F2, F3, F4, F5, F6, F7 };

/* Globals */
const char *VERSION = "DOSBase V.1.5 (Final)";
entry *front = NULL;	/* Linked-list pointer of "entry" structures */
char formatSet = 0;		/* Format-specification "set" flag -- init to false */
char successfulFormatChange = 0;	/* Used As An Odd Bug-Fix */
char sortMethodSet = 0; /* Sort-specification "set" flag -- init to false */
char removeunderScores;	/* Remove Underscores Option Flag */
char sortMethod;		/* Selected Sort-Method Option */            
char string_order;		/* Artist Then Title or Title Then Artist? */          
char column_order;		/* Column Order In ASCII Text Generation */
char *substring1, *substring2, *substring3;	/* Format Specification */
unsigned int entries = 0;	/* Number of entries (2^32 max) in the list -- init to zero */

/* Module Prototypes */
static void processKey(enum FuncKey key);
static void exitDosbase(void);
static enum FuncKey getKey(void);

/**************************************************************************
In an attempt to keep the main function as small as possible, I wrote it
to emmulate the functionality of a message-loop. So, you won't see much
low-level functionality here... it is pretty much straightforward.
**************************************************************************/
int main(void)	/* argc & argv unused at this point */
{
	enum FuncKey key;	/* Data-type with values F1-F12 */
	
	init_console();		/* Initialize The Console */
	
	db_clrscr(); 		/* Clear The Screen */
	init_interface();	/* Paint Main Interface */
	write_welcome();	/* Initialize Message Notification Box */
	
	/* Enter Input Loop */

	while((key = getKey()) != F8)	/* F8 = Exit DOSBase */
		processKey(key);	/* Process The Option */

	exitDosbase();		/* Clear The Screen & Write Closing Comments */

	return 0;
}

/**************************************************************************
This function calls the function which appropriately responds to the user's
option-choice. It also verifies the ability to call a certain function:
i.e., a user must first create a list before creating TXT or HTML files. 
**************************************************************************/
static void processKey(enum FuncKey key)
{
	switch (key)
	{
		case F1:	if (front == NULL) chooseFormat();	/* User Hasn't Configured
														   format-specification yet */
					
					/* User might have read in .DAT file (which doesn't
					require the format-specification to be set, thus the
					list isn't "empty", but a format specification has not
					been set */
					else if (!formatSet) chooseFormat(); 

					if (successfulFormatChange)	/* Otherwise User Escaped Input Box */
					{
						/* Select sort type: sort by artist or sort by title */
						if (entries == 0) 
							if(!sortMethodSet) setSort();
						
						/* Check if the sort method was set. If it wasn't,
						   the user escaped out of the dialog */
						if (sortMethodSet) processDirectory();
					}
					break;
		case F2:	generateTxt();	/* Create ASCII *.txt File */
					break;
		case F3:	generateHTML(); /* Create HTML File */
					break;
		case F4:	if (front)	/* Does A List Exist? */
						chooseFormat();	/* Choose A New Format */
					else
						messagebox("**ERROR** You Must First Create A List! (F1)");
					break;
		case F5:	if (front)	/* Does A List Exist? */
						writeDAT();	/* Write DOSBase .DAT File */
					else
						messagebox("**ERROR** You Must First Create A List! (F1)");
					break;
		case F6:	readDAT();	/* Read DOSBase .DAT File */
					break;
		case F7:	getInfo();	/* Get MPEG Frame Information */
					break;
	}
}

/* Set Cursor & Colors Back To Normal, Print Exit Message */
static void exitDosbase(void)
{
	db_clrscr();	/* Clear The Screen */
   
	db_textcolor(GRAY, BG_BLACK);	/* Put Color Back To Console-Default */
   
	printf("Thank You For Using %s\n(C)1995-2000 Clint Kennedy. All Rights Reserved.\n", VERSION);
	printf("\n** Remember, this program is FREEWARE and may be used as\n");
	printf("long as it is useful. I do ask that you e-mail me if\n");
	printf("you enjoy using my software!  Refer to manual.txt for\n");
	printf("instructions on usage. Suggestions and feedback are welcomed\n");
	printf("to DJSlakoR@hotmail.com. \n");
	printf("\nPlease visit the website at http://revolutionaudio.dhs.org/dosbase\n");
	printf("to check for any future releases and sourcecode download!\n\n");

	/* Used So Windows Users Who Didn't Run The Program From The Command
		Line Will See The Exit Message.  Remove The Next 3 lines If You 
		Don't Like This */
	printf("Press Any Key To Exit ...");
	getch();
	printf("\n\n");

	DisplayCursor();	/* Turn Back On The Cursor */
}

/* Waits For A Function Key To Be Pressed (F1-F8 Only In This Case)
and Returns Its Value. Review a formal description of getch()
and perhaps bioskey() for an explanation of function key reading. */
static enum FuncKey getKey(void)
{
	char key = 0;

	while ( !(key > 58) || !(key < 67)) 
	{
		while (getch() != 0);	/* Waits for a control key */
		key = getch();			/* See getch() definition for explanation */
	}

	switch (key)
	{
	case 59: return F1;
	case 60: return F2;
	case 61: return F3;
	case 62: return F4;
	case 63: return F5;
	case 64: return F6;
	case 65: return F7;
	case 66: return F8;
	default: break;		/* Simply to defeat compiler warnings */
	}

	return F8;	/* Unreachable, but to defeat compiler warnings */
}
