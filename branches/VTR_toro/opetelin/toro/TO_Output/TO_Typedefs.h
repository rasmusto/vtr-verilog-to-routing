//===========================================================================//
// Purpose : Enums, typedefs, and defines for TO_Output class.
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2012-2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com) //
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

#ifndef TO_TYPEDEFS_H
#define TO_TYPEDEFS_H

//---------------------------------------------------------------------------//
// Define LAFF file constants and typedefs
//---------------------------------------------------------------------------//

enum TO_LaffColorId_e
{
   TO_LAFF_COLOR_UNKNOWN = 0,
   TO_LAFF_COLOR_RED     = 1,
   TO_LAFF_COLOR_GREEN   = 2,
   TO_LAFF_COLOR_BLUE    = 3,
   TO_LAFF_COLOR_CYAN    = 4,
   TO_LAFF_COLOR_YELLOW  = 5,
   TO_LAFF_COLOR_PINK    = 6,
   TO_LAFF_COLOR_WHITE   = 7
};
typedef enum TO_LaffColorId_e TO_LaffColorId_t;

enum TO_LaffFillId_e
{
   TO_LAFF_FILL_UNDEFINED  = 0,
   TO_LAFF_FILL_PATTERN_10 = 1,
   TO_LAFF_FILL_PATTERN_50 = 3,
   TO_LAFF_FILL_PATTERN_70 = 0,
   TO_LAFF_FILL_PATTERN_90 = 9
};
typedef enum TO_LaffFillId_e TO_LaffFillId_t;

enum TO_LaffLayerId_e
{
   TO_LAFF_LAYER_UNDEFINED = 0,
   TO_LAFF_LAYER_PB        = 10 + TO_LAFF_FILL_PATTERN_50,
   TO_LAFF_LAYER_IO        = 20 + TO_LAFF_FILL_PATTERN_50,
   TO_LAFF_LAYER_SB        = 30 + TO_LAFF_FILL_PATTERN_50,
   TO_LAFF_LAYER_CB        = 40 + TO_LAFF_FILL_PATTERN_70,
   TO_LAFF_LAYER_CH        = 50 + TO_LAFF_FILL_PATTERN_10,
   TO_LAFF_LAYER_CV        = 60 + TO_LAFF_FILL_PATTERN_10,
   TO_LAFF_LAYER_SH        = 70 + TO_LAFF_FILL_PATTERN_90,
   TO_LAFF_LAYER_SV        = 80 + TO_LAFF_FILL_PATTERN_90,
   TO_LAFF_LAYER_IG        = 100,
   TO_LAFF_LAYER_IM        = 110,
   TO_LAFF_LAYER_IP        = 120,
   TO_LAFF_LAYER_BB        = 130
};
typedef enum TO_LaffLayerId_e TO_LaffLayerId_t;

#endif
