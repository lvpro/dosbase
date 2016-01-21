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

/* Custom Structures Used By DOSBase. New structures should be added here. */

/* Linked-List "entry" structure: holds title & artist strings */
struct entry {            
   char *title;
   char *artist;
   struct entry *next;
};

typedef struct entry entry;

/* MPEG Frame Information Structure */
struct mpeg_frame {
   char MPEGversion;
   char *ChannelMode;
   char *Emphasis;
   char *PlayingTime;

   char LayerDescription;
   char Copyright;
   char Original;
   char CRCProtected;
   char PaddingBit;

   int Bitrate;
   int FrameCount;

   unsigned long SamplingRate;
     
};

typedef struct mpeg_frame MPEGINFO;
