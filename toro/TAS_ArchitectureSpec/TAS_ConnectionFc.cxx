//===========================================================================//
// Purpose : Method definitions for the TAS_ConnectionFc class.
//
//           Public methods include:
//           - TAS_ConnectionFc_c, ~TAS_ConnectionFc_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - PrintXML
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

#include "TC_MinGrid.h"
#include "TCT_Generic.h"

#include "TIO_PrintHandler.h"

#include "TAS_StringUtils.h"
#include "TAS_ConnectionFc.h"

//===========================================================================//
// Method         : TAS_ConnectionFc_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
TAS_ConnectionFc_c::TAS_ConnectionFc_c( 
      void )
      :
      type( TAS_CONNECTION_BOX_UNDEFINED ),
      percent( 0.0 ),
      absolute( 0 ),
      dir_( TC_TYPE_UNDEFINED )
{
}

//===========================================================================//
TAS_ConnectionFc_c::TAS_ConnectionFc_c( 
      const TAS_ConnectionFc_c& connectionFc )
      :
      type( connectionFc.type ),
      percent( connectionFc.percent ),
      absolute( connectionFc.absolute ),
      srName( connectionFc.srName ),
      dir_( connectionFc.dir_ )
{
}

//===========================================================================//
// Method         : ~TAS_ConnectionFc_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
TAS_ConnectionFc_c::~TAS_ConnectionFc_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
TAS_ConnectionFc_c& TAS_ConnectionFc_c::operator=( 
      const TAS_ConnectionFc_c& connectionFc )
{
   if( &connectionFc != this )
   {
      this->type = connectionFc.type;
      this->percent = connectionFc.percent;
      this->absolute = connectionFc.absolute;
      this->srName = connectionFc.srName;
      this->dir_ = connectionFc.dir_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
bool TAS_ConnectionFc_c::operator==( 
      const TAS_ConnectionFc_c& connectionFc ) const
{
   return(( this->type == connectionFc.type ) &&
          ( TCTF_IsEQ( this->percent, connectionFc.percent )) &&
          ( this->absolute == connectionFc.absolute ) &&
	  ( this->srName == connectionFc.srName ) &&
          ( this->dir_ == connectionFc.dir_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
bool TAS_ConnectionFc_c::operator!=( 
      const TAS_ConnectionFc_c& connectionFc ) const
{
   return( !this->operator==( connectionFc ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
void TAS_ConnectionFc_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   const char* pszFc = "";
   switch( this->dir_ )
   {
   case TC_TYPE_INPUT:  pszFc = "fc_in";  break;
   case TC_TYPE_OUTPUT: pszFc = "fc_out"; break;
   default:             pszFc = "fc";     break;
   }

   if( this->srName.length( ))
   {
      printHandler.Write( pfile, spaceLen, "name=\"%s\" ", 
                                           TIO_SR_STR( this->srName ));
   }

   if( this->type == TAS_CONNECTION_BOX_FRACTION )
   {
      printHandler.Write( pfile, spaceLen, "%s=\"%0.*f\"", 
                                           TIO_PSZ_STR( pszFc ), 
                                           precision, this->percent );
   }
   else if( this->type == TAS_CONNECTION_BOX_ABSOLUTE )
   {
      printHandler.Write( pfile, spaceLen, "%s=\"%u\"", 
                                           TIO_PSZ_STR( pszFc ),
                                           this->absolute );
   }
   else if( this->type == TAS_CONNECTION_BOX_FULL )
   {
      printHandler.Write( pfile, spaceLen, "%s=\"full\"", 
                                           TIO_PSZ_STR( pszFc ));
   }
}

//===========================================================================//
// Method         : PrintXML
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
void TAS_ConnectionFc_c::PrintXML( 
      void ) const
{
   FILE* pfile = 0;
   size_t spaceLen = 0;

   this->PrintXML( pfile, spaceLen );
}

//===========================================================================//
void TAS_ConnectionFc_c::PrintXML( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   const char* pszFc = "";
   switch( this->dir_ )
   {
   case TC_TYPE_INPUT:  pszFc = "default_in";  break;
   case TC_TYPE_OUTPUT: pszFc = "default_out"; break;
   default:             pszFc = "fc";          break;
   }

   string srFcType;
   TAS_ExtractStringConnectionBoxType( this->type, &srFcType );

   if( this->srName.length( ))
   {
      printHandler.Write( pfile, spaceLen, "<pin name=\"%s\" ",
                                           TIO_SR_STR( this->srName ));
   }

   if( this->type == TAS_CONNECTION_BOX_FRACTION )
   {
      printHandler.Write( pfile, 0, "%s_type=\"%s\" %s_val=\"%0.*f\"",
                                    TIO_PSZ_STR( pszFc ),
                                    TIO_SR_STR( srFcType ),
                                    TIO_PSZ_STR( pszFc ),
                                    precision, this->percent );
   }
   else if( this->type == TAS_CONNECTION_BOX_ABSOLUTE )
   {
      printHandler.Write( pfile, 0, "%s_type=\"%s\" %s_val=\"%u\"",
                                    TIO_PSZ_STR( pszFc ),
                                    TIO_SR_STR( srFcType ),
                                    TIO_PSZ_STR( pszFc ),
                                    this->absolute );
   }
   else if( this->type == TAS_CONNECTION_BOX_FULL )
   {
      printHandler.Write( pfile, 0, "%s_type=\"%s\"",
                                    TIO_PSZ_STR( pszFc ),
                                    TIO_SR_STR( srFcType ));
   }

   if( this->srName.length( ))
   {
      printHandler.Write( pfile, 0, "/>\n" );
   }
}
