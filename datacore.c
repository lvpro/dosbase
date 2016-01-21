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

/* Primary Data Handling Routines */

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "datacore.h"
#include "screen.h"
#include "defs.h"

/* Globals - Refer to dosbase.c for a description */
extern entry *front;
extern char sortMethod;
extern char string_order;
extern char *substring1, *substring2, *substring3;

/* Module Globals */
char *currentpath;

/***************************************************************************
   This function completely removes all underscores from
   both the title and the artist strings.
***************************************************************************/
void removeUnderscores(void)
{
	entry *current = front;
	int length, i;

	while (current != NULL)
	{
		/* Remove underscores from title string */

		length = strlen(current->title);
		for (i = 0; i < (length - 1); i++)
		{
			if(current->title[i] == '_')
			current->title[i] = ' ';
		}

		/* Remove underscores from artist string */

		length = strlen(current->artist);
		for (i = 0; i < (length - 1); i++)
		{
			if(current->artist[i] == '_')
			current->artist[i] = ' ';
		}
		current = current->next;
	}
}

/****************************************************************************
The insert() function adds mp3 file info entries to a linked
list.  It accepts as parameters the front pointer to the beginning
of the list, the title string, the artist string, and the sortMethod
used.  1 is used for title. 0 is used for artist. If front = null, the
list is empty and the item is to be inserted first.
****************************************************************************/
void insert(char *title, char *artist)
{
	entry *predecessor;	/* Pointer to the item BEFORE needed insertion point */
	entry *current;	/* Pointer to currently processing entry node */
	entry *temp;	/* Temporary Pointer */

	if (front == NULL)	/* if list is empty */
	{
		front = (entry *) malloc(sizeof(entry));
		front->title = title;
		front->artist = artist;
		front->next = NULL;
	}
	else                 /* list is NOT empty */
	{
		/* Find the pointer to the predecessor */
		switch(sortMethod)
		{
		case SORT_TITLE:  
			/* Is entry already in the list in first position? */
			if(stricmp(title, front->title) == 0)
				if(stricmp(artist, front->artist) == 0)
					return;

			/* If the entry goes before the first item... */
			if((stricmp(title, front->title) == 0) ?
				(stricmp(artist, front->artist) < 0)
				:(stricmp(title, front->title) < 0))
			{
				temp = front;
				front = (entry*) malloc(sizeof(entry));
				front->title = title;
				front->artist = artist;
				front->next = temp;
				break;
			}
			else
			{
				current = predecessor = front;
				while((stricmp(current->title, title)==0) ?  /* If Titles Match */
						(stricmp(current->artist, artist) < 0)
						:(stricmp(current->title, title) < 0) )
				{
					predecessor = current;
					current = current->next;

					if(current!= NULL)
					{
						if(stricmp(title, current->title) == 0)
							if(stricmp(artist, current->artist) == 0)
								return;
					}
					else break;
				}

				temp = predecessor->next;
				predecessor->next = (entry*) malloc(sizeof(entry));
				predecessor = predecessor->next;
				predecessor->title = title;
				predecessor->artist = artist;
				predecessor->next = temp;
				break;
			}
		case SORT_ARTIST:
			/* Check If The Entry Is Already In The List First */
			if(stricmp(artist, front->artist) == 0)
				if(stricmp(title, front->title) == 0)
					return;

			/* If the entry goes before the first item... */
			if((stricmp(artist, front->artist) == 0) ?
				(stricmp(title, front->title) < 0)
				:(stricmp(artist, front->artist) < 0))
			{
				temp = front;
				front = (entry*) malloc(sizeof(entry));
				front->title = title;
				front->artist = artist;
				front->next = temp;
				break;
			}
			else
			{
				current = predecessor = front;
				while((stricmp(current->artist, artist)==0) ?	/* If Titles Match */
						(stricmp(current->title, title) < 0)
						:(stricmp(current->artist, artist) < 0) )
				{
					predecessor = current;
					current = current->next;
					if(current != NULL)
					{
						if(stricmp(artist, current->artist) == 0)
							if(stricmp(title, current->title) == 0)
								return;
					}
					else break;
				}
				temp = predecessor->next;
				predecessor->next = (entry*) malloc(sizeof(entry));
				predecessor = predecessor->next;
				predecessor->title = title;
				predecessor->artist = artist;
				predecessor->next = temp;
				break;
			}
		}	/* End Of Switch */
	}	/* End Of else */

   addToTotalCount();	/* Increment Interface Counter */
}

/*************************************************************************
InsertEntry is the actual function called from the main driver.
It is passed a filename string and does the work of parsing it, then
calls insert() to add the entry to the linked list.
**************************************************************************/
void insertEntry(char *entryString)
{
	char *title;
	char *artist;
	char *p;	/* A temporary pointer */
	char length;	/* Holds lenghts of substrings */

	if(string_order == ARTIST_FIRST)
	{
		p = (entryString + strlen(substring1));	/* Skip first substring */
		length = strstr(entryString, substring2) - entryString - strlen(substring1);
		artist = (char *) malloc(length + 1);	/* Make room on the heap */
		strncpy(artist, p, length);	/* Extract Artist from main string */
		*(artist + length) = '\0';	/* Null out artist string */

		p = entryString + strlen(substring1) + length + strlen(substring2);
		length = strstr(entryString, substring3) - entryString - strlen(substring1)
				- strlen(artist) - strlen(substring2);
		title = (char *) malloc(length + 1);
		strncpy(title, p, length);
		*(title + length) = '\0';
    
		insert(title, artist);
	}
	else
	{
		p = (entryString + strlen(substring1));	/* Skip first substring */
		length = strstr(entryString, substring2) - entryString - strlen(substring1);
		title = (char *) malloc(length + 1);	/* Make room on the heap */
		strncpy(title, p, length);	/* Extract Artist from main string */
		*(title + length) = '\0';	/* Null out artist string */

		p = entryString + strlen(substring1) + length + strlen(substring2);
		length = strstr(entryString, substring3) - entryString - strlen(substring1)
				- strlen(title) - strlen(substring2);
		artist = (char *) malloc(length + 1);
		strncpy(artist, p, length);
		*(artist + length) = '\0';
    
		insert(title, artist);
	}
}

/****************************************************************************
formatCheck() compares the current filename string against the defined
searchable substrings to determine whether or not it is a valid filename
for the current search. This prevents illegal memory operation and General
Protection Faults.
****************************************************************************/
int formatCheck(char *string)
{
	char *sub1POS, *sub2POS, *sub3POS;

	/* In order for a string to be validly "parsable" it must contain all
	3 substrings (in the proper order and location). If all tests are
	passed, the function returns 1 (true), else it returns 0 (false). */

	/* Is The First Substring In The String? */

	if((sub1POS = strstr(string, substring1)) == NULL)
		return 0;

	/* If So, Is It At The Beginning Of The String? */

	if(sub1POS != string)
		return 0;

	/* Now, Is The Second Substring In The String? */

	if((sub2POS = strstr(string, substring2)) == NULL)
		return 0;

	/* If So, Is It Somewhere After The First Substring? */

	if(sub1POS > sub2POS)
		return 0;

	/* Now, Is The Third Substring In The String? */

	if((sub3POS = strstr(string, substring3)) == NULL)
		return 0;

	/* If So, Is It AFTER The Second Substring? */

	if(sub2POS > sub3POS)
		return 0;

	/* If All Of These Tests Are Successful, return true! */

	return 1;
}

/***************************************************************************
searchDirectory() is called if recursion is NOT selected....
hence it does not recurse subdirectory paths. This function was
updated in Version 1.5 Final release to accomodate the file finding
differences proprietary to Borland. win32api function calls are now
used.
***************************************************************************/
void searchDirectory(char *path)
{
	WIN32_FIND_DATA findData;
	HANDLE findHandle;
	int fileFound;

	if(SetCurrentDirectory(path))
	{
		/* Coders: To Search For mp1,mp2,mp3,mp4, etc, change *.mp3 to *.mp? */
		findHandle = FindFirstFile("*.mp3", &findData);
		
		if(findHandle == INVALID_HANDLE_VALUE)	/* No Matches */
			return;
		
		if(formatCheck(findData.cFileName))	/* Otherwise Check First Return */
			insertEntry(findData.cFileName);

		fileFound = FindNextFile(findHandle, &findData); 
		while (fileFound)
		{
			if(formatCheck(findData.cFileName))	/* Proper Form */
				insertEntry(findData.cFileName);
			
			fileFound = FindNextFile(findHandle, &findData);
		}
	}
	return;
}

/***************************************************************************
recursePaths() is called if recursion is selected.... it uses function call
recursion to scan all subdirectories of a given path.
***************************************************************************/
void recursePaths(char *path)
{
	WIN32_FIND_DATA findData;
	HANDLE findHandle;
	int fileFound;
	char cwdir[70];

	SetCurrentDirectory(path);
	GetCurrentDirectory(70, cwdir);

	/* Scan The Directory Pointed To By Path */
	if(SetCurrentDirectory(path))
	{
		/* Coders: To Search For mp1,mp2,mp3,mp4, etc, change *.mp3 to *.mp? */
		findHandle = FindFirstFile("*.mp3", &findData);
		
		if(findHandle != INVALID_HANDLE_VALUE)
		{
			if(formatCheck(findData.cFileName))
				insertEntry(findData.cFileName);

			fileFound = FindNextFile(findHandle, &findData); 
			while (fileFound)
			{
				if(formatCheck(findData.cFileName))	/* Proper Form */
					insertEntry(findData.cFileName);
			
				fileFound = FindNextFile(findHandle, &findData);
			}
		}

		findHandle = FindFirstFile("*.*", &findData);
		
		if(findHandle != INVALID_HANDLE_VALUE)
		{
			fileFound = 1;
			while(fileFound)
			{
				if((findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					&& (strcmp(findData.cFileName, "..")) && (strcmp(findData.cFileName, ".")))
				{
					/* Attach This Directory Name To The Current Working Directory */
					currentpath = (char *) malloc(strlen(cwdir) + strlen(findData.cFileName) + 2);
					strcpy(currentpath, cwdir);

					/* Remove \ if exists */
					if(*(currentpath + strlen(currentpath) - 1) == '\\')
						*(currentpath + strlen(currentpath) - 1) = '\0';

					/* Add \ directory splitter */
					strcat(currentpath, "\\");

					/* Attach New Directory */
					strcat(currentpath, findData.cFileName);

					/* Recurse Subdirectories Of This Name */
					recursePaths(currentpath);
				}

				fileFound = FindNextFile(findHandle, &findData);
			}
		}
	}

	/* Free The Memory Path Points To */
	free(path);
}