//===========================================================================//
// Purpose : Method definitions for the TFS_FloorplanStore class.
//
//           Public methods include:
//           - TFS_FloorplanStore_c, ~TFS_FloorplanStore_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - IsValid
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

#include "TFS_FloorplanStore.h"

//===========================================================================//
// Method         : TFS_FloorplanStore_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
TFS_FloorplanStore_c::TFS_FloorplanStore_c( 
      void )
{
} 

//===========================================================================//
TFS_FloorplanStore_c::TFS_FloorplanStore_c( 
      const TFS_FloorplanStore_c& floorplanStore )
      :
      architectureSpec( floorplanStore.architectureSpec ),
      fabricModel( floorplanStore.fabricModel ),
      circuitDesign( floorplanStore.circuitDesign )
{
} 

//===========================================================================//
// Method         : ~TFS_FloorplanStore_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
TFS_FloorplanStore_c::~TFS_FloorplanStore_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
TFS_FloorplanStore_c& TFS_FloorplanStore_c::operator=( 
      const TFS_FloorplanStore_c& floorplanStore )
{
   if( &floorplanStore != this )
   {
      this->architectureSpec = floorplanStore.architectureSpec;
      this->fabricModel = floorplanStore.fabricModel;
      this->circuitDesign = floorplanStore.circuitDesign;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TFS_FloorplanStore_c::operator==( 
      const TFS_FloorplanStore_c& floorplanStore ) const
{
   return(( this->architectureSpec == floorplanStore.architectureSpec ) &&
          ( this->fabricModel == floorplanStore.fabricModel ) &&
          ( this->circuitDesign == floorplanStore.circuitDesign ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TFS_FloorplanStore_c::operator!=( 
      const TFS_FloorplanStore_c& floorplanStore ) const
{
   return( !this->operator==( floorplanStore ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
void TFS_FloorplanStore_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   this->architectureSpec.Print( pfile, spaceLen );
   this->fabricModel.Print( pfile, spaceLen );
   this->circuitDesign.Print( pfile, spaceLen );
}

//===========================================================================//
// Method         : IsValid
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TFS_FloorplanStore_c::IsValid(
      void ) const
{
   return(( this->architectureSpec.IsValid( )) &&
          ( this->fabricModel.IsValid( )) &&
          ( this->circuitDesign.IsValid( )) ?
          true : false );
}
