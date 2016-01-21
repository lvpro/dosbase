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
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "structs.h"
#include "dialogs.h"
#include "screen.h"
#include "defs.h"
#include "mpeg.h"
#include "datacore.h"
#include "output.h"

/* Refer To dosbase.c For Description Of These Global Variables */
extern const char* VERSION;
extern entry *front;
extern int entries;
extern char removeunderScores;
extern char *substring1, *substring2, *substring3;
extern char string_order;
extern char column_order;
extern char formatSet;
extern char successfulFormatChange;
extern char sortMethod;
extern char sortMethodSet;
extern char string_order;
char lastFormatDynamic = 0;

/* Draw Main Interface To Console */
void init_interface(void)
{
	char row;

	HideCursor();	/* Turn Off The Cursor */
	
	/* Write Program Header */
	db_textcolor(YELLOW, BG_BLACK);
	db_gotoxy(1,1);
	printf("%s Coded By Clint Kennedy\n", VERSION);
	printf("Distributed As Open Source FREEWARE.  All rights reserved.");

	/* Display Track List Counter */
	db_gotoxy(3, 4);
	printf("Total Tracks In List: %d", entries);
   
	/* Draw Action Box (Uses Old DOS-Style Lines, Won't Show Up In IDE */
	db_gotoxy(3, 6);
	printf("…ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ Action Box ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕª");
	for(row = 0; row < 11; row++)
	{
		printf("\n");
		printf("%3c%75c", '∫', '∫');
	}
	printf("\n");
	printf("  »ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕº");

	/* Skip A Few Lines And Draw Option Controls */
	printf("\n\n");

	/* Add To List Control (F1) */
	db_textcolor(WHITE, BG_RED);
	db_gotoxy(3, 21);
	printf("F1");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Add To List");

	/* Generate ASCII TXT Control (F2) */
	db_gotoxy(22, 21);
	db_textcolor(WHITE, BG_RED);
	printf("F2");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Generate TXT");

	/* Generate HTML Control (F3) */
	db_gotoxy(42, 21);
	db_textcolor(WHITE, BG_RED);
	printf("F3");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Generate HTML");

	/* Change Format Specification (F4) */
	db_gotoxy(63, 21);
	db_textcolor(WHITE, BG_RED);
	printf("F4");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - New Format");

	/* Write DOSBase Binary .DAT File (F5) */
	db_gotoxy(8, 23);
	db_textcolor(WHITE, BG_RED);
	printf("F5");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Write .DAT");

	/* Read DOSBase Binary .DAT File Control (F6) */
	db_gotoxy(26, 23);
	db_textcolor(WHITE, BG_RED);
	printf("F6");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Read .DAT");

	/* Decode MPEG Frame Header Control (F7) */
	db_gotoxy(43, 23);
	db_textcolor(WHITE, BG_RED);
	printf("F7");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Get MPEG Info");

	/* Exit DOSBase Control (F8) */
	db_gotoxy(64, 23);
	db_textcolor(WHITE, BG_RED);
	printf("F8");
	db_textcolor(WHITE, BG_BLACK);
	printf(" - Exit");
}

/* Write Welcome Message To Action Box */
void write_welcome(void)
{
	writeAction("");
	writeAction("        Welcome To DOSBase V.1.5 - The Simple Way To List Your Music");
	writeAction("");
	writeAction("           To Begin Press F1 To Start The List Creation Wizard!");
	writeAction("");
	writeAction("      If You Have Not Done So, I Strongly Suggest Reading MANUAL.TXT");
	writeAction("");
	writeAction("Please Visit http://revolutionaudio.dhs.org/dosbase For Future Releases!");
	writeAction("");
	writeAction("        Sourcecode Is Also Freely Available At The DOSBase Website!");
	writeAction("");
}

/****************************************************************************
This function draws a messagebox. It's functionality simulates win32s
messagebox()
****************************************************************************/
void messagebox(char *message)
{
	char row;
	char enterKey;

	db_gotoxy(10, 9);
   
	db_textcolor(WHITE, BG_RED);

	/* Print Top Of Message Box */
	printf("⁄ƒƒƒƒƒ DOSBase Message ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø\n");
	for(row = 0; row < 5; row++)	/* Print Internals */
	{
		db_gotoxy(10, (short int)(row+10));
		printf("%c%61c\n", '≥', '≥');
	}

	/* Print Bottom Of Message Box */
	db_gotoxy(10, 15);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ");
   
	/* Print Enter "Acceptance" Key */
	db_textcolor(WHITE, BG_BLUE);
	printf(" ENTER ");
	db_textcolor(WHITE, BG_RED);
	printf("ƒƒƒƒŸ");

	/* Print The String Of The Message Box */
	db_gotoxy(15, 12);
	printf("%s", message);

	/* Wait For User To Press "Enter" */
	enterKey = _getch();
	while (enterKey != '\r')
		enterKey = getch();
	
	/* NOTE TO DEVELOPERS: The following 2 blocks of code could be
		optimized to exclude the erasing and "speed-up" the refresh
		of the Action Box */
	
	/* Erase Action Box Area (With Black */
	db_textcolor(WHITE, BG_BLACK);
	for (row = 9; row < 16; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	/* Reprint The Contents Of The Action Buffer */
	printBuffer();
}

/* chooseFormat() sets the character strings that insert() searches for */
void chooseFormat(void)
{
	char row, ch;
	char format, remove;
	char formatNotification[75];

	successfulFormatChange = 0;	/* Innocent Until Proven Guilty */

	db_gotoxy(12, 5);
    db_textcolor(BLACK, BG_CYAN);
	
	/* Print Choose String Format Box */
	printf("⁄ƒƒƒƒƒ Choose String Fromat ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for (row = 6; row < 20; row++)
	{
		db_gotoxy(12, row);
		printf("≥%58c", '≥');
	}

	db_gotoxy(12, row);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	/* Print Options */
	db_gotoxy(15, 7);
	printf("Select The Format You Wish To Search Files For:");
	db_gotoxy(15, 9);
	printf("1.  title - artist.mp3");
	db_gotoxy(15, 10);
	printf("2.  artist - title.mp3");
	db_gotoxy(15, 11);
	printf("3.  (title)artist.mp3");
	db_gotoxy(15, 12);
	printf("4.  (artist)title.mp3");
	db_gotoxy(15, 13);
	printf("5.  (title)-artist.mp3");
	db_gotoxy(15, 14);
	printf("6.  (artist)-title.mp3");
	db_gotoxy(43, 9);
	printf("7.  User Defined");
	db_gotoxy(43, 11);
	printf("If Your Format Does Not");
	db_gotoxy(43, 12);
	printf("Appear In This List,");
	db_gotoxy(43, 13);
	printf("Choose Option 7 To Create");
	db_gotoxy(43, 14);
	printf("A Custom Search Format.");
	db_gotoxy(15, 16);
	printf("Choose Desired Format:");
	db_gotoxy(38, 16);
    
    /* User Input Box */
	db_textcolor(WHITE, BG_BLUE);
    printf(" ");
    db_gotoxy(38, 16);
    DisplayCursor();

	/* Wait For A Valid Answer, Or ESC */
	format = getch();
	while ((format < '1') || (format > '7'))
	{
		if (format == 0x1B) goto ESC;	/* User Hit "ESC" */
		db_gotoxy(38, 16);
		printf(" ");
		db_gotoxy(38, 16);
		format = getch();
	}

	/* Rehide The Cursor & Print The User's Choice */
	HideCursor();
	db_gotoxy(38, 16);
	printf("%c", format);

	/* Print Underscore Removal Prompt */
	db_textcolor(BLACK, BG_CYAN);
	db_gotoxy(15, 17);
	printf("Do You Wish To Convert Underscores To Spaces? (Y/N):");

	/* Print Input Box */
	db_gotoxy(67, 17);
	db_textcolor(WHITE, BG_BLUE);
	printf(" ");
	db_gotoxy(67, 17);
	DisplayCursor();

	/* Wait For Appropriate Choice Or ESC */
	remove = getch();
	while ((remove != 'y') && (remove != 'Y') && (remove != 'n') && (remove != 'N'))
	{
		if (remove == 0x1B) goto ESC;
		db_gotoxy(67, 17);
		printf(" ");
		db_gotoxy(67, 17);
		remove = getch();
	}
    
	HideCursor();
	
	db_gotoxy(67, 17);
	printf("%c", remove);

	/* Print Accept / Cancel Options */
	db_gotoxy(23, 19);
	db_textcolor(BLACK, BG_CYAN);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 19);
	printf("[ENTER To Continue]");

	/* Wait For Valid Option */
	ch = getch();
	while ((ch != 27) && (ch != '\r'))
	{
		ch = getch();
	}

	db_textcolor(WHITE, BG_BLACK);

	if (ch == '\r')
	{
		/* First set removeunderScores flag */
		if ((remove == 'y') || (remove == 'Y'))
			removeunderScores = 1;
		else
			removeunderScores = 0;
      
		switch(format) {
		case '1':	substring1 = "";
					substring2 = " - ";
					substring3 = ".mp3";
					string_order = TITLE_FIRST;
					strcpy(formatNotification, "Search Format Set To \"title - artist.mp3\"");
					break;

		case '2':	substring1 = "";
					substring2 = " - ";
					substring3 = ".mp3";
					string_order = ARTIST_FIRST;
					strcpy(formatNotification, "Search Format Set To \"artist - title.mp3\"");		
					break;

		case '3':	substring1 = "(";
					substring2 = ")";
					substring3 = ".mp3";
					string_order = TITLE_FIRST;
					strcpy(formatNotification, "Search Format Set To \"(title)artist.mp3\"");
					break;

		case '4':	substring1 = "(";
					substring2 = ")";
					substring3 = ".mp3";
					string_order = ARTIST_FIRST;
					strcpy(formatNotification, "Search Format Set To \"(artist)title.mp3\"");
				break;

		case '5':	substring1 = "(";
					substring2 = ")-";
					substring3 = ".mp3";
					string_order = TITLE_FIRST;
					strcpy(formatNotification, "Search Format Set To \"(title)-artist.mp3\"");
					break;

		case '6':	substring1 = "(";
					substring2 = ")-";
					substring3 = ".mp3";
					string_order = ARTIST_FIRST;
					strcpy(formatNotification, "Search Format Set To \"(artist)-title.mp3\"");
					break;

		case '7':	if(customFormat() == 0) goto ESC;
					if(string_order == TITLE_FIRST)
						sprintf(formatNotification, "Search Format Set To \"%stitle%sartist%s\"", substring1, substring2, substring3);
					else
						sprintf(formatNotification, "Search Format Set To \"%sartist%stitle%s\"", substring1, substring2, substring3);
					break;
		default :	break;
		}

		if(format != 7)	lastFormatDynamic = 0;	/* Memory Allocation Safety */
		
		successfulFormatChange = 1;
		formatSet = 1;
		
		writeAction(formatNotification);
	}
	else
		formatSet = 0;

/* ESC Routine */
ESC:

	HideCursor();
	db_textcolor(BLACK, BG_BLACK);
	for (row = 5; row < 21; row++)
	{
		db_gotoxy(12, row);
		printf("%59c", ' ');
	}
   
	db_textcolor(YELLOW, BG_BLACK);
	db_gotoxy(12, 6);
	printf("ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ Action Box ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ");
	db_gotoxy(12, 18);
	printf("ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ");

	printBuffer();
}
    
/****************************************************************************
I wrote this function one night when I came in from partying, and one of
my beta testers suggested user defined search substrings.  It sounded
like a good idea, so I added the feature. Although I wrote this in a 
drunken state, it seems to work wonderfully..
****************************************************************************/
int customFormat(void)
{
	int returnVal=0;

	char row, ch;
	char *subst1, *subst2, *subst3;
	char stringOrder;

	char subst1ptr[47];
	char subst2ptr[47];
	char subst3ptr[47];

	subst1ptr[0] = 45;
	subst1ptr[1] = 0;
	subst2ptr[0] = 45;
	subst2ptr[1] = 0;
	subst3ptr[0] = 45;
	subst3ptr[1] = 0;

	/* Print Dialog Box */
	db_gotoxy(12, 5);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Choose String Fromat ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for (row = 6; row < 20; row++)
	{
		db_gotoxy(12, row);
		printf("≥%58c", '≥');
	}

	db_gotoxy(12, row);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	/* Print Directions */
	db_gotoxy(15, 7);
	printf("The format string is based on 3 searchable substrings");
	db_gotoxy(15, 8);
	printf("used to parse filenames. Each filename consists of 2");
	db_gotoxy(15, 9);
	printf("parts:  the artist part and the title part.  Despite");
	db_gotoxy(15, 10);
	printf("the order of these parts, there is a substring before");
	db_gotoxy(15, 11);
	printf("the first part, between the 2 parts, and after the 2nd");
	db_gotoxy(15, 12);
	printf("part.  Only the 2nd substring MUST be SOMETHING.");
	db_gotoxy(15, 13);
	printf("Refer to MANUAL.TXT for a more detailed description.");
	db_gotoxy(15, 15);
	printf("Enter First Substring (Press Enter For None):");
	
	/* Print Input Box */
	db_gotoxy(15, 17);
    db_textcolor(WHITE, BG_BLUE);
	printf("                                             ");
	db_gotoxy(15, 17);
    
	DisplayCursor();

	subst1 = getns(subst1ptr);

	if(subst1ptr[1] == -1)
		goto ESC;
	
	if(subst1ptr[1] == 0)
		subst1 = "";

	db_textcolor(BLACK, BG_CYAN);
    db_gotoxy(15, 15);
	printf("Enter Second Substring (must be something):  ");
    db_gotoxy(15, 17);

    /* Print Input Box */
    db_textcolor(WHITE, BG_BLUE);
    printf("                                             ");
    db_gotoxy(15, 17);

	subst2 = getns(subst2ptr);
	if(subst2ptr[1] == -1)	goto ESC;
	while (subst2ptr[1] == 0)
	{
		db_gotoxy(15, 17);
		db_textcolor(WHITE, BG_BLUE);
		printf("                                             ");
		db_gotoxy(15, 17);
		subst2 = getns(subst2ptr);
		if(subst2ptr[1] == -1) goto ESC;
    }

	db_textcolor(BLACK, BG_CYAN);
	db_gotoxy(15, 15);
	printf("Enter Third Substring (.mp3 not needed):   ");
	db_gotoxy(15, 17);
	
	/* Print Input Box */
	db_textcolor(WHITE, BG_BLUE);
	printf("                                             ");
	db_gotoxy(15, 17);
    
	subst3 = getns(subst3ptr);
	if(subst3ptr[1] == -1) goto ESC;
	if(subst3ptr[1] == 0)
		subst3 = "";

	HideCursor();
	db_textcolor(BLACK, BG_CYAN);

	for (row = 7; row < 14; row++)
	{
		db_gotoxy(15, row);
		printf("%54c", ' ');
	}
   
	db_gotoxy(15, 15);
	printf("                                             ");
	db_gotoxy(15, 17);
	printf("                                             ");

	db_gotoxy(15, 7);
	printf("Choose String Order:                         ");
	db_gotoxy(15, 9);
	printf("1.  Title then Artist   Example: title - artist.mp3");
	db_gotoxy(15, 10);
	printf("2.  Artist then Title   Example: artist - title.mp3");

	db_gotoxy(15, 12);
	printf("Enter Option: ");
  
	db_textcolor(WHITE, BG_BLUE);
	db_gotoxy(29, 12);
	printf(" ");
	db_gotoxy(29, 12);
    
	DisplayCursor();

	stringOrder = getche();
	while ((stringOrder != '1') && (stringOrder != '2'))
	{
		if (stringOrder == 0x1B) goto ESC;
		db_gotoxy(29, 12);
		printf(" ");
		db_gotoxy(29, 12);
		stringOrder = getche();
	}

    HideCursor();

	db_textcolor(BLACK, BG_CYAN);
	db_gotoxy(17, 15);
	printf("You Have Customized The Following Format String:");

	db_gotoxy(31, 17);
	printf(subst1);
	if (stringOrder == '1')
	{
		printf("title");
		printf(subst2);
		printf("artist");
		printf(subst3);
		printf(".mp3");
	}
	else
	{
		printf("artist");
		printf(subst2);
		printf("title");
		printf(subst3);
		printf(".mp3");
	}

	db_gotoxy(23, 19);
	db_textcolor(BLACK, BG_CYAN);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 19);
	printf("[ENTER To Continue");

	ch = getch();

	while ((ch != 27) && (ch != '\r'))
	{
		ch = getch();
	}

	if (ch != 27)
	{	
		substring1 = (char *) malloc(strlen(subst1) + 1);
		substring2 = (char *) malloc(strlen(subst2) + 1);
		substring3 = (char *) malloc(strlen(subst3) + 4);

		if (subst1ptr[1] == 0)
			substring1[0] = '\0';
		else
			strcpy(substring1, subst1);

		strcpy(substring2, subst2);
		
		if (subst3ptr[1] == 0)
			substring3[0] = '\0';
		else
			strcpy(substring3, subst3);

		strcat(substring3, ".mp3");

		if (stringOrder == '1')
			string_order = TITLE_FIRST;
		else
			string_order = ARTIST_FIRST;
	
		lastFormatDynamic = 1;

		returnVal = 1;
	}

ESC:	
	/* Reset Interface */

	HideCursor();
	db_textcolor(BLACK, BG_BLACK);
	for (row = 5; row < 21; row++)
	{
		db_gotoxy(12, row);
		printf("%59c", ' ');
	}
	
	db_textcolor(YELLOW, BG_BLACK);
	db_gotoxy(12, 6);
	printf("ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ");
	db_gotoxy(12, 18);
	printf("ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ");

	printBuffer();

	return returnVal;
}
     
/****************************************************************************
setSort() sets the SORT_METHOD variable.

NOTE TO DEVELOPERS:
This function needs to be rewritten to be more intuitive (thereby allowing
the user to switch sort methods mid-execution.) I've had the idea in the back
of my mind, and I always planned to do it, but I never did.  You do it!
****************************************************************************/
void setSort(void)
{
	char row;
	char ch, ch2;

	/* Print Dialog Box */
	db_gotoxy(10, 8);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Select Sorting Method (Set One Time Only) ƒƒƒƒƒƒƒƒƒƒƒƒø\n");
 
	for(row = 0; row < 7; row++)
	{
		db_gotoxy(10, (short int)(row+9));
		printf("%c%61c\n", '≥', '≥');
	}

	db_gotoxy(10, 16);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	/* Print Sorting Options */
	db_gotoxy(13, 10);
	printf("(T)  Sort By Title");
	db_gotoxy(13, 11);
	printf("(A)  Sort By Artist");
	db_gotoxy(13, 13);
	printf("Enter Sort Option:");
   
	/* Print Input Prompt */
	db_gotoxy(32, 13);
	db_textcolor(WHITE, BG_BLUE);
	printf(" ");
	db_gotoxy(32, 13);
	DisplayCursor();

	/* Wait For Valid Response, Or ESC */
	ch = getch();
	while ((ch != 't') && (ch != 'T') && (ch != 'a') && (ch != 'A'))
	{
		if (ch == 0x1B) goto ESC;
		db_gotoxy(32, 13);
		printf(" ");
		db_gotoxy(32, 13);
		ch = getche();
	}

	/* Hide Cursor & Print Choice */
	HideCursor();
	db_gotoxy(32, 13);
	printf("%c", ch);

	/* Print Accept / Cacel */
	db_textcolor(BLACK, BG_CYAN);
	db_gotoxy(23, 15);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 15);
	printf("[ENTER To Continue]");

	/* Get Answer */
	ch2 = getch();
	while ((ch2 != '\r') && (ch2 != 0x1B))
		ch2 = getch();

	/* Check For Escape, If So, Escape */
	if (ch2 == 0x1B) goto ESC;

	/* Erase Dialog */
	db_textcolor(BLACK, BG_BLACK);
	for (row = 9; row < 16; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();

	if (ch == 't')
	{
		sortMethod = SORT_TITLE;
		db_gotoxy(51, 4);
		db_textcolor(YELLOW, BG_BLACK);
		printf("Sort Method: SORT BY TITLE ");
		writeAction("Sort Method Set To SORT BY TITLE For This Session");
	}
	else
	{
		sortMethod = SORT_ARTIST;
		db_gotoxy(51, 4);
		db_textcolor(YELLOW, BG_BLACK);
		printf("Sort Method: SORT BY ARTIST");
		writeAction("Sort Method Set To SORT BY ARTIST For This Session");
	}

	sortMethodSet = 1;
	return;		/* No Need To Process ESC: Code Here */

ESC:

   HideCursor();
   db_textcolor(BLACK, BG_BLACK);

   for (row = 9; row < 16; row++)
   {
      db_gotoxy(10, row);
      printf("%c%61c", ' ', ' ');
   }

   printBuffer();
}

/* Implementation Of GetInfo() Dialog For MPEG Frame Header Reading */
void getInfo()
{
	char row;
	char pathBuffer[61];
	char *path;
	char ch;

	FILE *mpegfp;
	MPEGINFO mpegDetails;

	pathBuffer[0] = 58;
	pathBuffer[1] = 0;

	db_gotoxy(10, 8);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Enter Filename To Extract MPEG Info From ƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for (row = 0; row < 7; row++)
	{
		db_gotoxy(10, (short int)(row+9));
		printf("%c%61c\n", '≥', '≥');
	}

	db_gotoxy(10, 16);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(13, 10);
	printf("Filename (Full Path Recommended):");
	db_gotoxy(13, 11);
	printf("(Long Filenames and Spaces Are Allowed, Do NOT Use Quotes)");

	db_gotoxy(12, 13);
	db_textcolor(WHITE, BG_BLUE);
    DisplayCursor();
	
	while (pathBuffer[1] == 0)
	{
		db_gotoxy(12, 13);
		printf("%58c", ' ');
		db_gotoxy(12, 13);
		path = getns(pathBuffer);
		if(pathBuffer[1] == -1)
			goto ESC;
	}

	HideCursor();

	db_gotoxy(23, 15);	
	db_textcolor(BLACK, BG_CYAN);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 15);
	printf("[ENTER To Continue]");

	ch = getch();	
	while ((ch != 27) && (ch != '\r'))
	{
		ch = getch();
	}

	if (ch == 27) goto ESC;

	db_textcolor(BLACK, BG_BLACK);

	db_gotoxy(10, 8);
	for (row = 8; row < 17; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();

	if((mpegfp = fopen(path, "r")) == NULL)
		messagebox("**ERROR**  Unable To Find File!");
	else
	{
		if(!getMp3Info(&mpegDetails, mpegfp))
			messagebox("**ERROR** Valid Frame Header Not Found!");
		else
			displayMp3Info(&mpegDetails);		
	}  
	return;

ESC:
	HideCursor();
	db_textcolor(BLACK, BG_BLACK);

	db_gotoxy(10, 8);
	for (row = 8; row < 17; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();
}

/* MPEG Information Display Dialog */
void displayMp3Info(MPEGINFO *mpegDetails)
{
	char row;
	char ch;

	db_gotoxy(12, 6);
	db_textcolor(WHITE, BG_BLUE);
	printf("⁄ƒƒƒƒƒ MPEG File Information ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for (row = 7; row < 18; row++)
	{
		db_gotoxy(12, row);
		printf("≥%58c", '≥');
	}

	db_gotoxy(12, row);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(15, 8);
	printf(" MPEG Version:");
	db_gotoxy(15, 10);
	printf("        Layer:");
	db_gotoxy(15, 12);
	printf("      Bitrate:");
	db_gotoxy(15, 14);
	printf("Sampling Rate:");
	db_gotoxy(15, 16);
	printf(" Playing Time:");

	db_gotoxy(42, 8);
	printf("Channel Mode:");
	db_gotoxy(42, 10);
	printf("   Copyright:");
	db_gotoxy(42, 12);
	printf("    Original:");
	db_gotoxy(42, 14);
	printf("    Emphasis:");
	db_gotoxy(42, 16);
	printf("      Frames:");

	db_gotoxy(30, 8);
	if(mpegDetails->MPEGversion == MPEG2_5)
		printf("2.5");
	else
		printf("%d", mpegDetails->MPEGversion);
   
	db_gotoxy(30, 10);
	printf("%d", mpegDetails->LayerDescription);
	db_gotoxy(30, 12);
	printf("%dkbit/s", mpegDetails->Bitrate);
	db_gotoxy(30, 14);
	printf("%ldkHz", mpegDetails->SamplingRate);
	db_gotoxy(30, 16);
	printf("%s", mpegDetails->PlayingTime);
	db_gotoxy(56, 8);
	printf("%s", mpegDetails->ChannelMode);
	db_gotoxy(56, 10);

	if(mpegDetails->Copyright) printf("Yes");
	else printf("No");
   
	db_gotoxy(56, 12);
	if(mpegDetails->Original) printf("Yes");
	else printf("No");
   
	db_gotoxy(56, 14);
	printf("%s", mpegDetails->Emphasis);
	db_gotoxy(56, 16);
	printf("%d", mpegDetails->FrameCount);

	db_gotoxy(57, 18);
	db_textcolor(WHITE, BG_RED);
	printf(" ENTER ");

	ch = getch();
	while (ch != '\r') ch = getch();

   db_textcolor(BLACK, BG_BLACK);
   for (row = 6; row < 19; row++)
   {
      db_gotoxy(12, row);
      printf("%c%58c", ' ', ' ');
   }

	db_textcolor(YELLOW, BG_BLACK);
	db_gotoxy(12, 6);
	printf("ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ Action Box ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ");
	db_gotoxy(12, 18);
	printf("ÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕÕ");

	printBuffer();
}

/****************************************************************************
This function is in charge of obtaining a file name for html output
then generates the HTML based on routines in output.c
*****************************************************************************/
void generateHTML(void)
{
	char pathBuffer[43];
	char *path;
	char row, ch;
	FILE *fp;

	pathBuffer[0] = 40;
	pathBuffer[1] = 0;

	/* Make Sure List Exists */
	if(front == NULL)
	{
		messagebox("**ERROR**  You Must First Create A List (F1)");
		return;
	}

	/* Print Input Dialog */
	db_gotoxy(10, 9);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Enter Filename For HTML ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for(row = 0; row < 6; row++)
	{
		db_gotoxy(10, (short int)(row+10));
		printf("%c%61c", '≥', '≥');
		printf("\n");
	}

	db_gotoxy(10, 16);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(12, 11);
	printf("Save As (.htm or .html extension recommended) :");

	db_gotoxy(54, 13);
	printf("(40 Chars Max)");

	db_textcolor(WHITE, BG_BLUE);
	DisplayCursor();

	while(pathBuffer[1] == 0)
	{
		db_gotoxy(12, 13);
		printf("%40c", ' ');
		db_gotoxy(12, 13);
		path = getns(pathBuffer);

		if(pathBuffer[1] == -1)
			goto ESC;
	}

	HideCursor();

	db_gotoxy(23, 15);
	db_textcolor(BLACK, BG_CYAN);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 15);
	printf("[ENTER To Continue]");

	ch = getch();

	while((ch != 27) && (ch != '\r'))
		ch = getch();

	/* Remove Dialog Window */
	db_textcolor(BLACK, BG_BLACK);
	for(row = 9; row < 16; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	/* Repaint Action Box Contents */
	printBuffer();

	if(ch == '\r')
	{
		if((fp = fopen(path, "w")) == NULL)
		{
			messagebox("**ERROR** Opening Output File!");
			return;
		}
		htmlOut(fp);
		writeAction("HTML Document Created Successfully!");
	}

	return;

ESC:
	/* Remove Dialog Window */
	db_textcolor(BLACK, BG_BLACK);
	for(row = 9; row < 16; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	/* Repaint Action Box Contents */
	printBuffer();

	return;
}

/****************************************************************************
generateTxt() is in charge of collecting information needed to create an
ASCII file output, then generates the file. 
****************************************************************************/
void generateTxt(void)
{
	char row, ch;
	char columnOrg;
	char filenameBuffer[43];	
	char *filename;
	FILE *fp;

	filenameBuffer[0] = 40;
	filenameBuffer[1] = 0;

	if(front == NULL)
	{
		messagebox("**ERROR** You Must First Create A List (F1)");
		return;
	}

	db_gotoxy(12,5);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Generate ASCII *.txt ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for (row = 6; row < 19; row++)
	{
		db_gotoxy(12, row);
		printf("≥%58c", '≥');
	}

	db_gotoxy(12, row);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(14,7);
	db_textcolor(BLACK, BG_CYAN);
	printf("Select Column Organization:        (T) Title First");

	db_gotoxy(49,8);
	printf("(A) Artist First");

	db_gotoxy(14,9);
	printf("Enter Choice: ");

	db_textcolor(WHITE, BG_BLUE);
	printf(" ");

	db_gotoxy(28,9);
	DisplayCursor();

	columnOrg = getch();	
	while (!(columnOrg == 'T') && !(columnOrg == 't') && !(columnOrg == 'a') && !(columnOrg == 'A'))
	{
		if(columnOrg == 0x1B) goto ESC;
		db_gotoxy(28, 9);
		printf(" ");
		db_gotoxy(28, 9);
		columnOrg = getch();
	}

	db_gotoxy(28, 9);
	printf("%c", columnOrg);

	HideCursor();   

	db_textcolor(BLACK, BG_CYAN);
	db_gotoxy(14,12);
	printf("Save As:           (40 Characters Max)");

	db_gotoxy(14, 16);
	db_textcolor(BLACK, BG_CYAN);
	printf("(Full Path Not Required, But Permitted)");

	DisplayCursor();

	while (filenameBuffer[1] == 0)
	{
		db_gotoxy(14, 14);
		db_textcolor(WHITE, BG_BLUE);
		printf("%40c", ' ');
		db_gotoxy(14, 14);
		filename = getns(filenameBuffer);
	
		if(filenameBuffer[1] == -1)
			goto ESC;
	}

	HideCursor();

	db_gotoxy(23, 18);
	db_textcolor(BLACK, BG_CYAN);

	printf("[ESC To Cancel]");
	db_gotoxy(42, 18);
	printf("[ENTER To Continue]");

	ch = getch();

	while ((ch != 27) && (ch != '\r'))
		ch = getch();

	/* Reset Interface */

	db_textcolor(BLACK, BLACK);
	for (row = 5; row < 20; row++)
	{
		db_gotoxy(12, row);
		printf("%59c", ' ');
	}
   
	init_interface();
	printBuffer();

	if(ch == '\r')
	{
		if((columnOrg == 'T') || (columnOrg == 't'))
			column_order = TITLE_COLUMN_1ST;
		else
			column_order = ARTIST_COLUMN_1ST;
		writeAction("Opening ASCII output file....");
		writeAction("Output File Created Successfully!");

		fp = fopen(filename, "w");
		txtout(fp);
	}
	return;

ESC:

   /* Reset Interface */

	HideCursor();
	db_textcolor(BLACK, BG_BLACK);

	for (row = 5; row < 20; row++)
	{
		db_gotoxy(12, row);
		printf("%59c", ' ');
	}
   
	init_interface();
	printBuffer();
	
	return;
}

/***************************************************************************
This function accepts as input a path and searches for all filenames
matching the current search substrings, including error checking.
***************************************************************************/
void processDirectory(void)
{
	char row;
	char cwdir[70];
	char pathBuffer[43];
	char *path;
	char *pathptr;
	char ch;
	char recurse;
	int pathlength;

	pathBuffer[0] = 40;
	pathBuffer[1] = 0;

	db_gotoxy(10, 8);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Enter Path To Search ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for(row = 0; row < 8; row++)
	{
		db_gotoxy(10, (short int)(row+9));
		printf("%c%61c\n", '≥', '≥');
	}

	db_gotoxy(10, 17);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(12, 10);
	printf("Enter Path (example: C:\\ ) :");
	db_gotoxy(54, 12);
	printf("(40 Chars Max)");

	db_textcolor(WHITE, BG_BLUE);
	DisplayCursor();

	while(pathBuffer[1] == 0)
	{
		db_gotoxy(12, 12);
		printf("%40c", ' ');
		db_gotoxy(12, 12);
		path = getns(pathBuffer);
	
		if(pathBuffer[1] == -1)
			goto ESC;
	}

	HideCursor();

	db_gotoxy(12, 14);
   
	db_textcolor(BLACK, BG_CYAN);

	printf("Should DOSBase Search Subdirectories As Well? (Y/N):");  
	db_gotoxy(65, 14);
   
	db_textcolor(WHITE, BG_BLUE);
	
	printf(" ");
	db_gotoxy(65, 14);
   
	DisplayCursor();

	recurse = getch();
	while((recurse != 'y') && (recurse != 'Y') && (recurse != 'n') && (recurse != 'N'))
	{
		if (recurse == 0x1B) goto ESC;
		db_gotoxy(65, 14);
		printf(" ");
		db_gotoxy(65, 14);
		recurse = getch();
	}

	HideCursor();
	db_gotoxy(65, 14);
	printf("%c", recurse);

	db_gotoxy(23, 16);
	db_textcolor(BLACK, BG_CYAN);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 16);
	printf("[ENTER To Continue]");

	ch = getch();
	while ((ch != 27) && (ch != '\r'))
		ch = getch();

	db_textcolor(BLACK, BG_BLACK);
	for (row = 9; row < 16; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();   

	if(ch == '\r')
	{
		GetCurrentDirectory(70, cwdir);

		/* Check path to see if it ends with \, if not, add it */
		pathlength = strlen(path);
		if (*(path + pathlength - 1) != '\\')
		{
			*(path + pathlength) = '\\';
			*(path + pathlength + 1) = '\0';
		}

		pathptr = (char *) malloc(strlen(path) + 1);
		strcpy(pathptr, path);

		if(!SetCurrentDirectory(path))
			messagebox("** ERROR **  Path Not Found!!");
		else
		{
			if ((recurse == 'y') || (recurse == 'Y'))
			{
				recursePaths(pathptr);
				writeAction("Directory Processed Successfully!");
			}
			else
			{
				searchDirectory(pathptr);
				writeAction("Directory Processed Successfully!");
			}
         
			/* Take out underscores if selected */
			if (removeunderScores)
				removeUnderscores();
		}

		/* Reset Current Working Directory */      
		SetCurrentDirectory(cwdir);
	}
	return;

ESC:

	HideCursor();
	db_textcolor(BLACK, BG_BLACK);
	for(row = 9; row < 16; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();   
}

/****************************************************************************
The follwing code is new additions to Version 1.1.  writeDAT()
reads the linked list stored in memory and writes the data to a user
specified output file in binary format.  The bitstream is in the
form:

	Bytes 0 - 3 = 32 bit integer specifying the number of entries
	Next 2 Bytes = 16 bit integer specifying Title Length
	Next N Bytes = Title String (based on title length)
	Next 2 Bytes = 16 bit integer specifying Artist Length
	Next N Bytes = Artist String (based on artist length)
	Repeat Back to Title Length Bytes

This feature allows the easy storage of previously made lists so that the
user no longer has to re-enter his entire list if he wishes to add to
it. Dialog & Implementation.
****************************************************************************/
void writeDAT(void)
{
	FILE *fp;
	char pathBuffer[62];
	char *path;
	char row, ch;
	entry *current = front;
	short length;

	pathBuffer[0] = 58;
	pathBuffer[1] = 0;

	db_gotoxy(10, 8);
	db_textcolor(BLACK, BG_CYAN);
	printf("⁄ƒƒƒƒƒ Enter Filename To Save Binary DAT File As ƒƒƒƒƒƒƒƒƒƒƒƒø");

	for (row = 0; row < 7; row++)
	{
		db_gotoxy(10, (short int)(row+9));
		printf("%c%61c\n", '≥', '≥');
	}

	db_gotoxy(10, 16);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(13, 10);
	printf("Filename (Full Path Recommended, No Extension Necessary):");
	
	db_gotoxy(12, 13);
    db_textcolor(WHITE, BG_BLUE);
    
	DisplayCursor();

	while (pathBuffer[1] == 0)
	{
		db_gotoxy(12, 12);
        printf("%58c", ' ');
		db_gotoxy(12, 12);
		path = getns(pathBuffer);
	
		if(pathBuffer[1] == -1)
			goto ESC;
	}

	strcat(path, ".dat");

	HideCursor();

	db_gotoxy(23, 15);
	db_textcolor(BLACK, BG_CYAN);
	printf("[ESC To Cancel]");
	db_gotoxy(42, 15);
	printf("[ENTER To Continue]");
    
	ch = getch();
	while ((ch != 27) && (ch != '\r'))
		ch = getch();

	db_textcolor(BLACK, BG_BLACK);

	db_gotoxy(10, 8);
	for(row = 8; row < 17; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();

	if(ch == '\r')
	{
		if((fp = fopen(path, "wb")) == NULL)
		{
			messagebox("**ERROR** Unable To Create File!");
			return;
		}

		fwrite(&entries, sizeof(entries), 1, fp);

		while(current != NULL)
		{
			length = (short) strlen(current->title);
			/* Write Title Length */
			fwrite(&length, sizeof(short), 1, fp);
			/* Write Title String */
			fwrite(current->title, strlen(current->title), 1, fp);

			length = (short) strlen(current->artist);
			/* Write Artist Length */
			fwrite(&length, sizeof(short), 1, fp);
			/* Write Artist String */
			fwrite(current->artist, strlen(current->artist), 1, fp);

			current = current->next;
		}        

		fclose(fp);

		writeAction("Binary DAT file created successfully!");    
	}
	return;

ESC:
	HideCursor();

	db_textcolor(BLACK, BG_BLACK);
	db_gotoxy(10, 8);
	for(row = 8; row < 17; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();
}

/* Reads DOSBase database files. Dialog & Implementation */
void readDAT(void)
{
	FILE *fp;
	char pathBuffer[62];
	char *path;
	char *artist, *title;
	char row, ch;
	int i;
	int count;
	short int titleLength;
	short int artistLength;

	pathBuffer[0] = 58;
	pathBuffer[1] = 0;

	db_gotoxy(10, 8);
	db_textcolor(BLACK, BG_CYAN);

	printf("⁄ƒƒƒƒƒ Enter Filename Of Binary DAT File ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø");

	for(row = 0; row < 7; row++)
	{
		db_gotoxy(10, (short int)(row+9));
		printf("%c%61c", '≥', '≥');
		printf("\n");
	}

	db_gotoxy(10, 16);
	printf("¿ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ");

	db_gotoxy(13, 10);
	printf("Filename (Full Path Recommended):");
	db_gotoxy(22, 11);
	printf("(.DAT extension not needed)");
	db_gotoxy(12, 13);
	db_textcolor(WHITE, BG_BLUE);
     
	DisplayCursor();

	while(pathBuffer[1] == 0)
	{
		db_gotoxy(12, 13);
		printf("%58c", ' ');
		db_gotoxy(12, 13);
		path = getns(pathBuffer);
	
		if(pathBuffer[1] == -1)
			goto ESC;
	}

	strcat(path, ".dat");

	HideCursor();

	db_gotoxy(23, 15);
	db_textcolor(BLACK, BG_CYAN);
        
	printf("[ESC To Cancel]");
	db_gotoxy(42, 15);
	printf("[ENTER To Continue]");

	ch = getch();
	while ((ch != '\r') && (ch != 27))
		ch = getch();

	db_textcolor(BLACK, BG_BLACK);
	db_gotoxy(10, 8);
	for(row = 8; row < 17; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();

	if(ch == 27)
		return;
        
	if((fp = fopen(path, "rb")) == NULL)
	{
		messagebox("**ERROR** Can Not Find Specified .DAT file!");
		return;
	}

	writeAction("Binary .DAT file opened");

	if(!entries)
		setSort();
    
	fread(&count, sizeof(int), 1, fp);

	for(i=0; i < count; i++)
	{
		fread(&titleLength, sizeof(short), 1, fp);
		title = (char*) malloc(titleLength + 1);
		fread(title, sizeof(char), titleLength, fp);
		title[titleLength] = '\0';
		fread(&artistLength, sizeof(short), 1, fp);
		artist = (char*) malloc(artistLength + 1);
		fread(artist, sizeof(char), artistLength, fp);
		artist[artistLength] = '\0';

		insert(title, artist);
	}

	fclose(fp);
	return;

ESC:
	HideCursor();
	db_textcolor(BLACK, BG_BLACK);
	db_gotoxy(10, 8);
	for(row = 8; row < 17; row++)
	{
		db_gotoxy(10, row);
		printf("%c%61c", ' ', ' ');
	}

	printBuffer();
}
