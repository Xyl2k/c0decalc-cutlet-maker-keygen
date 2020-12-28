#pragma once

///////////////////////////////////////////////////////////////////////////
//
// miniF.h
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

#include <windows.h>
#include "minifmod.h"

typedef struct
{
	int length;
	int pos;
	void *data;
} MEMFILE;

unsigned int	memopen		( char* name );
void			memclose	( unsigned int handle );
int				memread		( void *buffer, int size, unsigned int handle );
void			memseek		( unsigned int handle, int pos, signed char mode );
int				memtell		( unsigned int handle );

class miniF
{
public:
	miniF( VOID );
	~miniF( VOID );

	BOOL LoadMusic	( int iResource );
	BOOL Play		( VOID );
	BOOL Stop		( VOID );

private:
	FMUSIC_MODULE *mod;
};