//===========================================================================//
// Purpose : Method definitions for the TFM_Config class.
//
//           Public methods include:
//           - TFM_Config_c, ~TFM_Config_c
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

#if defined( SUN8 ) || defined( SUN10 )
   #include <math.h>
#endif

#include "TIO_PrintHandler.h"

#include "TFM_Config.h"

//===========================================================================//
// Method         : TFM_Config_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
TFM_Config_c::TFM_Config_c( 
      void )
{
} 

//===========================================================================//
TFM_Config_c::TFM_Config_c( 
      const TFM_Config_c& config )
      :
      fabricRegion( config.fabricRegion ),
      ioPolygon( config.ioPolygon )
{
} 

//===========================================================================//
// Method         : ~TFM_Config_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
TFM_Config_c::~TFM_Config_c( 
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
TFM_Config_c& TFM_Config_c::operator=( 
      const TFM_Config_c& config )
{
   if( &config != this )
   {
      this->fabricRegion = config.fabricRegion;
      this->ioPolygon = config.ioPolygon;
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
bool TFM_Config_c::operator==( 
      const TFM_Config_c& config ) const
{
   return(( this->fabricRegion == config.fabricRegion ) &&
          ( this->ioPolygon == config.ioPolygon ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TFM_Config_c::operator!=( 
      const TFM_Config_c& config ) const
{
   return( !this->operator==( config ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
void TFM_Config_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   if( this->fabricRegion.IsValid( ) ||
       this->ioPolygon.IsValid( ))
   {
      printHandler.Write( pfile, spaceLen, "<config>\n" );
      spaceLen += 3;

      if( this->fabricRegion.IsValid( ))
      {
         string srFabricRegion;
         this->fabricRegion.ExtractString( &srFabricRegion );

         printHandler.Write( pfile, spaceLen, "<region> %s </region>\n", 
                                              TIO_SR_STR( srFabricRegion ));
      }
      if( this->ioPolygon.IsValid( ))
      {
         string srIoPolygon;
         this->ioPolygon.ExtractString( &srIoPolygon );

         printHandler.Write( pfile, spaceLen, "<polygon> %s </polygon>\n", 
                                              TIO_SR_STR( srIoPolygon ));
      }
      spaceLen -= 3;
      printHandler.Write( pfile, spaceLen, "</config>\n" );
   }
}

//===========================================================================//
// Method         : IsValid
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/20/12 jeffr : Original
//===========================================================================//
bool TFM_Config_c::IsValid( 
      void ) const
{
   return( this->fabricRegion.IsValid( ) ||
           this->ioPolygon.IsValid( ) ?
           true : false );
}
