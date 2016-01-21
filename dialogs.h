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

/* GUI Dialog Function Prototypes */

void init_interface(void);	/* Draw Main Interface To Console */
void write_welcome(void);	/* Write Welcome Message To Action Box */
void messagebox(char *message);	/* Draws A Message Box */
void chooseFormat(void);	/* Sets Format Secification */
int customFormat(void);		/* Custom Format Specification Dialog */
void setSort(void);			/* Sets Sorting Method */
void getInfo(void);			/* Dialog For MPEG Frame Header Decoding */
void displayMp3Info(MPEGINFO *mpegDetails);	/* Display Of MPEG Info */
void generateHTML(void);	/* Dialog For HTML Creation */
void generateTxt(void);		/* Dialog For TEXT Creation */
void processDirectory(void);	/* Dialog For Directory Processing */
void readDAT(void);	/* Dialog & Implementation Of Database Reading */
void writeDAT(void);	/* Creates Database Files */