#include "miniF.h"

///////////////////////////////////////////////////////////////////////////
//
// miniF.cpp
// Copyright(c) 2007, Wiccaan
//
///////////////////////////////////////////////////////////////////////////
// A quick wrapper around the miniFMod functions to allow
// easy use of the basic functions.
///////////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////


miniF::miniF()
{
	FSOUND_File_SetCallbacks( memopen, memclose, memread, memseek, memtell );
}
miniF::~miniF(){}

BOOL miniF::LoadMusic(int iResource)
{
	mod = FMUSIC_LoadSong( MAKEINTRESOURCEA(iResource), NULL );

	if( !mod )
		return false;
	return true;
}

BOOL miniF::Play()
{
	if( !mod )
		return false;

	return (BOOL)FMUSIC_PlaySong( mod );
}

BOOL miniF::Stop()
{
	if( !mod )
		return false;

	return (BOOL)FMUSIC_StopSong( mod );
}


//--------------------------------------------------------------------------------------------
//
// Callbacks
//
//--------------------------------------------------------------------------------------------
unsigned int memopen(char *name)
{
	MEMFILE *memfile;
	memfile = (MEMFILE *)calloc( sizeof(MEMFILE), 1 );

	HRSRC		rec;
	HGLOBAL		handle;

	rec = FindResource( NULL, name, "MODFILE" );
	handle = LoadResource( NULL, rec );

	memfile->data	= LockResource( handle );
	memfile->length = SizeofResource( NULL, rec );
	memfile->pos	= 0;

	return (unsigned int)memfile;
}

void memclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;
	free( memfile );
}

int memread(void *buffer, int size, unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;
	if( memfile->pos + size >= memfile->length )
		size = memfile->length - memfile->pos;

	memcpy( buffer, (char *)memfile->data + memfile->pos, size );
	memfile->pos += size;

	return size;
}

void memseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if( mode == SEEK_SET )
		memfile->pos = pos;
	else if( mode == SEEK_CUR )
		memfile->pos += pos;
	else if( mode == SEEK_END )
		memfile->pos = memfile->length + pos;

	if( memfile->pos > memfile->length )
		memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;
	return memfile->pos;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------