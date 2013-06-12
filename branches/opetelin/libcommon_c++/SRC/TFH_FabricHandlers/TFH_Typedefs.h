//===========================================================================//
// Purpose : Enums, typedefs, and defines for TFH_FabricHandlers class.
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com)      //
//                                                                           //
// This program is free software; you can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; version 3 of the License, or any later version. //
//                                                                           //
// This program is distributed in the hope that it will be useful, but       //
// WITHOUT ANY WARRANTY; without even an implied warranty of MERCHANTABILITY //
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License   //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU General Public License along   //
// with this program; if not, see <http://www.gnu.org/licenses>.             //
//---------------------------------------------------------------------------//

#ifndef TFH_TYPEDEFS_H
#define TFH_TYPEDEFS_H

#include "TC_IndexCount.h"
#include "TCT_OrderedVector.h"
#include "TCT_SortedVector.h"

//---------------------------------------------------------------------------//
// Define fabric grid block constants and typedefs
//---------------------------------------------------------------------------//

class TFH_GridBlock_c; // Forward declaration for subsequent class
typedef TCT_SortedVector_c< TFH_GridBlock_c > TFH_GridBlockList_t;

enum TFH_BlockType_e
{
   TFH_BLOCK_UNDEFINED = 0,
   TFH_BLOCK_PHYSICAL_BLOCK,
   TFH_BLOCK_INPUT_OUTPUT,
   TFH_BLOCK_SWITCH_BOX
};
typedef enum TFH_BlockType_e TFH_BlockType_t;

//---------------------------------------------------------------------------//
// Define fabric switch box constants and typedefs
//---------------------------------------------------------------------------//

class TFH_SwitchBox_c; // Forward declaration for subsequent class
typedef TCT_SortedVector_c< TFH_SwitchBox_c > TFH_SwitchBoxList_t;

//---------------------------------------------------------------------------//
// Define fabric connection block constants and typedefs
//---------------------------------------------------------------------------//

class TFH_ConnectionBlock_c; // Forward declaration for subsequent class
typedef TCT_SortedVector_c< TFH_ConnectionBlock_c > TFH_ConnectionBlockList_t;

class TFH_ConnectionMap_c; // Forward declaration for subsequent class
typedef TCT_SortedVector_c< TFH_ConnectionMap_c > TFH_ConnectionMapList_t;

class TC_Bit_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TC_Bit_c > TFH_BitPattern_t;

//---------------------------------------------------------------------------//
// Define fabric channel width constants and typedefs
//---------------------------------------------------------------------------//

typedef TC_IndexCount_c TFH_ChannelWidth_t;
typedef TCT_OrderedVector_c< TFH_ChannelWidth_t > TFH_ChannelWidthList_t;

enum TFH_SelectChannelMode_e
{
   TFH_SELECT_CHANNEL_UNDEFINED = 0,
   TFH_SELECT_CHANNEL_X,
   TFH_SELECT_CHANNEL_Y
};
typedef enum TFH_SelectChannelMode_e TFH_SelectChannelMode_t;

#endif
