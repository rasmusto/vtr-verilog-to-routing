//===========================================================================//
// Purpose : Method definitions for the TPO_HierMap class.
//
//           Public methods include:
//           - TPO_HierMap_c, ~TPO_HierMap_c
//           - operator=
//           - operator==, operator!=
//           - Print
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2012 Jeff Rudolph, Texas Instruments (jrudolph@ti.com)      //
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

#include "TIO_PrintHandler.h"

#include "TPO_HierMap.h"

//===========================================================================//
// Method         : TPO_HierMap_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_HierMap_c::TPO_HierMap_c( 
      void )
      :
      hierNameList_( TPO_HIER_NAME_LIST_DEF_CAPACITY )
{
}

//===========================================================================//
TPO_HierMap_c::TPO_HierMap_c( 
      const string&         srInstName,
      const TPO_NameList_t& hierNameList )
      :
      srInstName_( srInstName ),
      hierNameList_( hierNameList )
{
}

//===========================================================================//
TPO_HierMap_c::TPO_HierMap_c( 
      const char*           pszInstName,
      const TPO_NameList_t& hierNameList )
      :
      srInstName_( TIO_PSZ_STR( pszInstName )),
      hierNameList_( hierNameList )
{
}

//===========================================================================//
TPO_HierMap_c::TPO_HierMap_c( 
      const TPO_HierMap_c& hierMap )
      :
      srInstName_( hierMap.srInstName_ ),
      hierNameList_( hierMap.hierNameList_ )
{
}

//===========================================================================//
// Method         : ~TPO_HierMap_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_HierMap_c::~TPO_HierMap_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_HierMap_c& TPO_HierMap_c::operator=( 
      const TPO_HierMap_c& hierMap )
{
   if( &hierMap != this )
   {
      this->srInstName_ = hierMap.srInstName_;
      this->hierNameList_ = hierMap.hierNameList_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TPO_HierMap_c::operator==( 
      const TPO_HierMap_c& hierMap ) const
{
   return(( this->srInstName_ == hierMap.srInstName_ ) &&
          ( this->hierNameList_ == hierMap.hierNameList_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TPO_HierMap_c::operator!=( 
      const TPO_HierMap_c& hierMap ) const
{
   return( !this->operator==( hierMap ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_HierMap_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<pack name=\"%s\">\n", 
                                        TIO_SR_STR( this->srInstName_ ));
   spaceLen += 3;

   printHandler.Write( pfile, spaceLen, "<hier> " );
   for( size_t i = 0; i < this->hierNameList_.GetLength( ); ++i )
   {
      const TC_Name_c& hierName = *this->hierNameList_[i];
      printHandler.Write( pfile, 0, "\"%s\" ", 
                                    TIO_PSZ_STR( hierName.GetName( )));
   }
   printHandler.Write( pfile, 0, "</hier>\n" );

   spaceLen -= 3;
   printHandler.Write( pfile, spaceLen, "</pack>\n" );
}
