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
#include <io.h>
#include <string.h>
#include "structs.h"
#include "mpeg.h"

/* Extracts MPEG Information From Frame Header */
int getMp3Info(MPEGINFO *mpegDetails, FILE *mp3fp)
{
	unsigned int nextByte;
	char frameSyncFound = 0;
	int seconds, minutes;
	char secondsptr[3], minutesptr[3];
	unsigned frameLength;

	while (!frameSyncFound)
	{
		/* Scan For A nextByte With All Bits Set */
		while (((nextByte = fgetc(mp3fp)) != EOF) && (nextByte != 0xFF));

		if(nextByte == EOF) return 0;    /* Error Checking */
      
		if((nextByte = fgetc(mp3fp)) == EOF)  /* Read next nextByte & error check */
			return 0;                       

		/* Confirm Next nextByte has its 3 most significant bits set */
		if((nextByte & 0xE0) == 0xE0)
			frameSyncFound = 1;

		/* Else keep reading in nextBytes until syncWord is found */
	}

	/* Process nextByte 2 of 4 nextByte Frame Header */

	/* Read MPEG Audio Version */
	if((nextByte & 0x18) == 0x18)
		mpegDetails->MPEGversion = MPEG1;
	else if((nextByte & 0x10) == 0x10)
		mpegDetails->MPEGversion = MPEG2;
	else if ((nextByte & 0x00) == 0x00)
		mpegDetails->MPEGversion = MPEG2_5;
	else
		return 0;	/* Error In Frame Header */

	/* Read Layer Description */
	if((nextByte & 0x02) == 0x02)
		mpegDetails->LayerDescription = 3;
	else if((nextByte & 0x04) == 0x04)
		mpegDetails->LayerDescription = 2;
	else if((nextByte & 0x06) == 0x06)
		mpegDetails->LayerDescription = 1;
	else
		return 0;	/* Error In Frame Header */

	/* Read CRC Y/N */
	if((nextByte & 0x01) == 0x01)
		mpegDetails->CRCProtected = 0;

	/* Read Next nextByte For Processing */
	if((nextByte = fgetc(mp3fp)) == EOF)	/* Read next nextByte & error check */
		return 0;                       

	/* Bitrate Calculation Table Block */

	/* The High-Order Nibble Of This nextByte Does Not Exhibit The
	Prefix Property, Thus We Must Shift Right 4 Bits For Comparison */
	switch(mpegDetails->MPEGversion)
	{
	case MPEG1:
		switch(mpegDetails->LayerDescription)
		{
		case 1:	/* Layer I */
			if(((nextByte >> 4) | 0x01) == 0x01)
				mpegDetails->Bitrate = 32;
			else if(((nextByte >> 4) | 0x02) == 0x02)
				mpegDetails->Bitrate = 64;
			else if(((nextByte >> 4) | 0x03) == 0x03)
				mpegDetails->Bitrate = 96;
			else if(((nextByte >> 4) | 0x04) == 0x04)
				mpegDetails->Bitrate = 128;
			else if(((nextByte >> 4) | 0x05) == 0x05)
				mpegDetails->Bitrate = 160;
			else if(((nextByte >> 4) | 0x06) == 0x06)
				mpegDetails->Bitrate = 192;
			else if (((nextByte >> 4) | 0x07) == 0x07)
				mpegDetails->Bitrate = 224;
			else if (((nextByte >> 4) | 0x08) == 0x08)
				mpegDetails->Bitrate = 256;
			else if (((nextByte >> 4) | 0x09) == 0x09)
				mpegDetails->Bitrate = 288;
			else if (((nextByte >> 4) | 0x0A) == 0x0A)
				mpegDetails->Bitrate = 320;
			else if (((nextByte >> 4) | 0x0B) == 0x0B)
				mpegDetails->Bitrate = 352;
			else if (((nextByte >> 4) | 0x0C) == 0x0C)
				mpegDetails->Bitrate = 384;
			else if (((nextByte >> 4) | 0x0D) == 0x0D)
				mpegDetails->Bitrate = 416;
			else if (((nextByte >> 4) | 0x0E) == 0x0E)
				mpegDetails->Bitrate = 448;
			else
				return 0;	/* Error */
			break;

		case 2:	/* Layer II */
			if(((nextByte >> 4) | 0x01) == 0x01)
				mpegDetails->Bitrate = 32;
			else if(((nextByte >> 4) | 0x02) == 0x02)
				mpegDetails->Bitrate = 48;
			else if(((nextByte >> 4) | 0x03) == 0x03)
				mpegDetails->Bitrate = 56;
			else if(((nextByte >> 4) | 0x04) == 0x04)
				mpegDetails->Bitrate = 64;
			else if(((nextByte >> 4) | 0x05) == 0x05)
				mpegDetails->Bitrate = 80;
			else if(((nextByte >> 4) | 0x06) == 0x06)
				mpegDetails->Bitrate = 96;
			else if(((nextByte >> 4) | 0x07) == 0x07)
				mpegDetails->Bitrate = 112;
			else if(((nextByte >> 4) | 0x08) == 0x08)
				mpegDetails->Bitrate = 128;
			else if(((nextByte >> 4) | 0x09) == 0x09)
				mpegDetails->Bitrate = 160;
			else if(((nextByte >> 4) | 0x0A) == 0x0A)
				mpegDetails->Bitrate = 192;
			else if(((nextByte >> 4) | 0x0B) == 0x0B)
				mpegDetails->Bitrate = 224;
			else if(((nextByte >> 4) | 0x0C) == 0x0C)
				mpegDetails->Bitrate = 256;
			else if(((nextByte >> 4) | 0x0D) == 0x0D)
				mpegDetails->Bitrate = 320;
			else if(((nextByte >> 4) | 0x0E) == 0x0E)
				mpegDetails->Bitrate = 384;
			else
				return 0;	/* Error */
			break;

		case 3:	/* Layer III */
			if(((nextByte >> 4) | 0x01) == 0x01)
				mpegDetails->Bitrate = 32;
			else if(((nextByte >> 4) | 0x02) == 0x02)
				mpegDetails->Bitrate = 40;
			else if(((nextByte >> 4) | 0x03) == 0x03)
				mpegDetails->Bitrate = 48;
			else if(((nextByte >> 4) | 0x04) == 0x04)
				mpegDetails->Bitrate = 56;
			else if(((nextByte >> 4) | 0x05) == 0x05)
				mpegDetails->Bitrate = 64;
			else if(((nextByte >> 4) | 0x06) == 0x06)	
				mpegDetails->Bitrate = 80;
			else if(((nextByte >> 4) | 0x07) == 0x07)
				mpegDetails->Bitrate = 96;
			else if(((nextByte >> 4) | 0x08) == 0x08)
				mpegDetails->Bitrate = 112;
			else if(((nextByte >> 4) | 0x09) == 0x09)
				mpegDetails->Bitrate = 128;
			else if(((nextByte >> 4) | 0x0A) == 0x0A)
				mpegDetails->Bitrate = 160;
			else if(((nextByte >> 4) | 0x0B) == 0x0B)
				mpegDetails->Bitrate = 192;
			else if(((nextByte >> 4) | 0x0C) == 0x0C)
				mpegDetails->Bitrate = 224;
			else if(((nextByte >> 4) | 0x0D) == 0x0D)
				mpegDetails->Bitrate = 256;
			else if(((nextByte >> 4) | 0x0E) == 0x0E)
				mpegDetails->Bitrate = 320;
			else
				return 0;	/* Error */
			break;
			default:
				break;	/* Avoid Compiler Warnings */
		}

		break;

	case MPEG2:
	case MPEG2_5:
		switch(mpegDetails->LayerDescription)
		{
		case 1:	/* Layer I */
			if(((nextByte >> 4) | 0x01) == 0x01)
				mpegDetails->Bitrate = 32;
			else if(((nextByte >> 4) | 0x02) == 0x02)
				mpegDetails->Bitrate = 48;
			else if(((nextByte >> 4) | 0x03) == 0x03)
				mpegDetails->Bitrate = 56;
			else if(((nextByte >> 4) | 0x04) == 0x04)
				mpegDetails->Bitrate = 64;
			else if(((nextByte >> 4) | 0x05) == 0x05)
				mpegDetails->Bitrate = 80;
			else if(((nextByte >> 4) | 0x06) == 0x06)
				mpegDetails->Bitrate = 96;
			else if(((nextByte >> 4) | 0x07) == 0x07)
				mpegDetails->Bitrate = 112;
			else if(((nextByte >> 4) | 0x08) == 0x08)
				mpegDetails->Bitrate = 128;
			else if(((nextByte >> 4) | 0x09) == 0x09)
				mpegDetails->Bitrate = 144;
			else if(((nextByte >> 4) | 0x0A) == 0x0A)
				mpegDetails->Bitrate = 160;
			else if(((nextByte >> 4) | 0x0B) == 0x0B)
				mpegDetails->Bitrate = 176;
			else if(((nextByte >> 4) | 0x0C) == 0x0C)
				mpegDetails->Bitrate = 192;
			else if(((nextByte >> 4) | 0x0D) == 0x0D)
				mpegDetails->Bitrate = 224;
			else if(((nextByte >> 4) | 0x0E) == 0x0E)
				mpegDetails->Bitrate = 256;
			else
				return 0;	/* Error */
			break;

		case 2:	/* Layer II */
		case 3:	/* Layer III */
			if(((nextByte >> 4) | 0x01) == 0x01)
				mpegDetails->Bitrate = 8;
			else if(((nextByte >> 4) | 0x02) == 0x02)
				mpegDetails->Bitrate = 16;
			else if(((nextByte >> 4) | 0x03) == 0x03)
				mpegDetails->Bitrate = 24;
			else if(((nextByte >> 4) | 0x04) == 0x04)
				mpegDetails->Bitrate = 32;
			else if(((nextByte >> 4) | 0x05) == 0x05)
				mpegDetails->Bitrate = 40;
			else if(((nextByte >> 4) | 0x06) == 0x06)
				mpegDetails->Bitrate = 48;
			else if(((nextByte >> 4) | 0x07) == 0x07)
				mpegDetails->Bitrate = 56;
			else if(((nextByte >> 4) | 0x08) == 0x08)	
				mpegDetails->Bitrate = 64;
			else if(((nextByte >> 4) | 0x09) == 0x09)
				mpegDetails->Bitrate = 80;
			else if(((nextByte >> 4) | 0x0A) == 0x0A)
				mpegDetails->Bitrate = 96;
			else if(((nextByte >> 4) | 0x0B) == 0x0B)
				mpegDetails->Bitrate = 112;
			else if(((nextByte >> 4) | 0x0C) == 0x0C)
				mpegDetails->Bitrate = 128;
			else if(((nextByte >> 4) | 0x0D) == 0x0D)
				mpegDetails->Bitrate = 144;
			else if(((nextByte >> 4) | 0x0E) == 0x0E)
				mpegDetails->Bitrate = 160;
			else
				return 0;	/* Error */
			break;
			default:
				break; /* Avoid Compiler Warnings */
		}
		break;

		default: break;
	}

	/* Sampling Rate Calculation Block */

	switch(mpegDetails->MPEGversion)
	{
	case MPEG1:
		if((nextByte | 0xF3) == 0xF3)	/* Bits 5 & 6 0? */
			mpegDetails->SamplingRate = 44100;
		else if((nextByte | 0xF3) == 0xF7)
			mpegDetails->SamplingRate = 48000;
		else if((nextByte | 0xF3) == 0xFB)
			mpegDetails->SamplingRate = 32000;
		else
			return 0;
		break;

	case MPEG2:
		if((nextByte | 0xF3) == 0xF3)
			mpegDetails->SamplingRate = 22050;
		else if ((nextByte | 0xF3) == 0xF7)
			mpegDetails->SamplingRate = 24000;
		else if ((nextByte | 0xF3) == 0xFB)
			mpegDetails->SamplingRate = 16000;
		else
			return 0;	/* Error */
		break;

	case MPEG2_5:
		if((nextByte | 0xF3) == 0xF3)
			mpegDetails->SamplingRate = 11025;
		else if((nextByte | 0xF3) == 0xF7)
			mpegDetails->SamplingRate = 12000;
		else if((nextByte | 0xF3) == 0xFB)
			mpegDetails->SamplingRate = 8000;
		else
			return 0;	/* Error */
		break;
		default: break;
	}

	/* Check For Bit Padding */
	if((nextByte & 0x02) == 0x02)
		mpegDetails->PaddingBit = 1;
	else
		mpegDetails->PaddingBit = 0;

	/* Determine Frame Size */
	switch(mpegDetails->LayerDescription)
	{
	case 1:
		frameLength = 12 * mpegDetails->Bitrate * 1000 / mpegDetails->SamplingRate
					+ mpegDetails->PaddingBit;
		break;
	case 2:
	case 3:
		frameLength = 144 * mpegDetails->Bitrate * 1000 / mpegDetails->SamplingRate
					+ mpegDetails->PaddingBit;
	default: break;
	}

	/* Calculate # of playable frames */
	mpegDetails->FrameCount = filelength(fileno(mp3fp)) / frameLength;

	/* Read Last nextByte For Processing */
	if((nextByte = fgetc(mp3fp)) == EOF)  /* Read next nextByte & error check */
		return 0;                       
   
	/* Get Channel Mode */
	if((nextByte | 0x3F) == 0x3F)
		mpegDetails->ChannelMode = "Stereo";
	else if(( nextByte | 0x3F) == 0x7F)
		mpegDetails->ChannelMode = "Joint Stereo";
	else if(( nextByte | 0x3F) == 0xBF)
		mpegDetails->ChannelMode = "Dual Channel";
	else if(( nextByte | 0x3F) == 0xFF)
		mpegDetails->ChannelMode = "Single Channel";
	else
		return 0;	/* Error */

	if((nextByte & 0x08) == 0x08)
		mpegDetails->Copyright = 1;
	else
		mpegDetails->Copyright = 0;

	if((nextByte & 0x04) == 0x04)
		mpegDetails->Original = 1;
	else
		mpegDetails->Original = 0;

	if((nextByte | 0xFC) == 0xFC)
		mpegDetails->Emphasis = "None";
	else if(( nextByte | 0xFC) == 0xFD)
		mpegDetails->Emphasis = "50/15 ms";
	else if(( nextByte | 0xFC) == 0xFF)
		mpegDetails->Emphasis = "CCIT J.17";
	else
		return 0;

	seconds = (filelength(fileno(mp3fp)) * 8) / (mpegDetails->Bitrate * 1000);
	minutes = seconds / 60;
	seconds = seconds % 60;

	mpegDetails->PlayingTime = (char *) malloc(6);
	strcpy(mpegDetails->PlayingTime, itoa(minutes, minutesptr, 10));
	strcat(mpegDetails->PlayingTime, ":");
	if(seconds < 10)
		strcat(mpegDetails->PlayingTime, "0");
	strcat(mpegDetails->PlayingTime, itoa(seconds, secondsptr, 10));

	return 1;	/* No Errors */
}




