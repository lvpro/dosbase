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

#include <windows.h>

/* Global Definitions Used By Multiple Modules */

/* Foreground Colors */
#define		BLACK		0
#define		BLUE		FOREGROUND_BLUE
#define		BLUE_LIGHT	FOREGROUND_BLUE | FOREGROUND_INTENSITY
#define		BLUE_SKY	FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define		BROWN		FOREGROUND_RED | FOREGROUND_GREEN
#define		CYAN		FOREGROUND_BLUE | FOREGROUND_GREEN
#define		GRAY		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
#define		GREEN		FOREGROUND_GREEN
#define		GREEN_LIGHT	FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define		PINK		FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY
#define		PURPLE		FOREGROUND_BLUE | FOREGROUND_RED
#define		RED			FOREGROUND_RED
#define		RED_LIGHT	FOREGROUND_RED | FOREGROUND_INTENSITY
#define		WHITE		FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY
#define		YELLOW		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY

/* Background Colors */
#define		BG_BLACK		0
#define		BG_BLUE			BACKGROUND_BLUE
#define		BG_BLUE_LIGHT	BACKGROUND_BLUE | BACKGROUND_INTENSITY
#define		BG_BLUE_SKY		BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define		BG_BROWN		BACKGROUND_RED | BACKGROUND_GREEN
#define		BG_CYAN			BACKGROUND_BLUE | BACKGROUND_GREEN
#define		BG_GRAY			BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED
#define		BG_GREEN		BACKGROUND_GREEN
#define		BG_GREEN_LIGHT	BACKGROUND_GREEN | BACKGROUND_INTENSITY
#define		BG_PINK			BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_INTENSITY
#define		BG_PURPLE		BACKGROUND_BLUE | BACKGROUND_RED
#define		BG_RED			BACKGROUND_RED
#define		BG_RED_LIGHT	BACKGROUND_RED | BACKGROUND_INTENSITY
#define		BG_WHITE		BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY
#define		BG_YELLOW		BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY

/* Sort Type / Column Order */
#define		SORT_TITLE			0
#define		SORT_ARTIST			1
#define		ARTIST_FIRST		1
#define		TITLE_FIRST			2
#define		ARTIST_COLUMN_1ST	1
#define		TITLE_COLUMN_1ST	2