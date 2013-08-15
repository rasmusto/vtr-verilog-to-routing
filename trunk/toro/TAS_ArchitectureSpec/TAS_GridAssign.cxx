//===========================================================================//
// Purpose : Method definitions for the TAS_GridAssign class.
//
//           Public methods include:
//           - TAS_GridAssign_c, ~TAS_GridAssign_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - PrintXML
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2012-2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com) //
//                                                                           //
// Permission is hereby granted, free of charge, to any person obtaining a   //
// copy of this software and associated documentation files (the "Software"),//
// to deal in the Software without restriction, including without limitation //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,  //
// and/or sell copies of the Software, and to permit persons to whom the     //
// Software is furnished to do so, subject to the following conditions:      //
//                                                                           //
// The above copyright notice and this permission notice shall be included   //
// in all copies or substantial portions of the Software.                    //
//                                                                           //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   //
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN // 
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  //
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR     //
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE //
// USE OR OTHER DEALINGS IN THE SOFTWARE.                                    //
//---------------------------------------------------------------------------//

#include "TC_MinGrid.h"
#include "TCT_Generic.h"

#include "TIO_PrintHandler.h"

#include "TAS_StringUtils.h"
#include "TAS_GridAssign.h"

//===========================================================================//
// Method         : TAS_GridAssign_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TAS_GridAssign_c::TAS_GridAssign_c( 
      void )
      :
      distr( TAS_GRID_ASSIGN_DISTR_UNDEFINED ),
      orient( TAS_GRID_ASSIGN_ORIENT_UNDEFINED ),
      priority( 0 ),
      singlePercent( 0.0 ),
      multipleStart( 0 ),
      multipleRepeat( 0 )
{
}

//===========================================================================//
TAS_GridAssign_c::TAS_GridAssign_c( 
      const TAS_GridAssign_c& gridAssign )
      :
      distr( gridAssign.distr ),
      orient( gridAssign.orient ),
      priority( gridAssign.priority ),
      singlePercent( gridAssign.singlePercent ),
      multipleStart( gridAssign.multipleStart ),
      multipleRepeat( gridAssign.multipleRepeat )
{
}

//===========================================================================//
// Method         : ~TAS_GridAssign_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TAS_GridAssign_c::~TAS_GridAssign_c( 
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
TAS_GridAssign_c& TAS_GridAssign_c::operator=( 
      const TAS_GridAssign_c& gridAssign )
{
   if( &gridAssign != this )
   {
      this->distr = gridAssign.distr;
      this->orient = gridAssign.orient;
      this->priority = gridAssign.priority;
      this->singlePercent = gridAssign.singlePercent;
      this->multipleStart = gridAssign.multipleStart;
      this->multipleRepeat = gridAssign.multipleRepeat;
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
bool TAS_GridAssign_c::operator==( 
      const TAS_GridAssign_c& gridAssign ) const
{
   return(( this->distr == gridAssign.distr ) &&
          ( this->orient == gridAssign.orient ) &&
          ( this->priority == gridAssign.priority ) &&
          ( TCTF_IsEQ( this->singlePercent, gridAssign.singlePercent )) &&
          ( this->multipleStart == gridAssign.multipleStart ) &&
          ( this->multipleRepeat == gridAssign.multipleRepeat ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TAS_GridAssign_c::operator!=( 
      const TAS_GridAssign_c& gridAssign ) const
{
   return( !this->operator==( gridAssign ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_GridAssign_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<grid_assign" );

   string srDistrMode;
   TAS_ExtractStringGridAssignDistrMode( this->distr, &srDistrMode );
   printHandler.Write( pfile, 0, " mode=\"%s\"", 
                                 TIO_SR_STR( srDistrMode ));

   if( this->orient != TAS_GRID_ASSIGN_ORIENT_UNDEFINED )
   {
      string srOrientMode;
      TAS_ExtractStringGridAssignOrientMode( this->orient, &srOrientMode );
      printHandler.Write( pfile, 0, " orient=\"%s\"", 
                                    TIO_SR_STR( srOrientMode ));
   }
   printHandler.Write( pfile, 0, " priority=\"%u\"", 
                                 this->priority );

   if( this->distr == TAS_GRID_ASSIGN_DISTR_SINGLE )
   {
      printHandler.Write( pfile, 0, " single_percent=\"%0.*f\"", 
                                    precision, this->singlePercent );
   }
   else if( this->distr == TAS_GRID_ASSIGN_DISTR_MULTIPLE )
   {
      printHandler.Write( pfile, 0, " multiple_start=\"%u\" multiple_repeat=\"%u\"", 
                                    this->multipleStart,
                                    this->multipleRepeat );
   }
   printHandler.Write( pfile, 0, "/>\n" );
}

//===========================================================================//
// Method         : PrintXML
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_GridAssign_c::PrintXML( 
      void ) const
{
   FILE* pfile = 0;
   size_t spaceLen = 0;

   this->PrintXML( pfile, spaceLen );
}

//===========================================================================//
void TAS_GridAssign_c::PrintXML( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   switch( this->distr )
   {
   case TAS_GRID_ASSIGN_DISTR_SINGLE:

      printHandler.Write( pfile, spaceLen, "<loc type=\"rel\" pos=\"%0.*f\" priority=\"%u\"/>\n",
                                           precision, this->singlePercent,
                                           this->priority );
      break;

   case TAS_GRID_ASSIGN_DISTR_MULTIPLE:

      printHandler.Write( pfile, spaceLen, "<loc type=\"col\" start=\"%u\" repeat=\"%u\" priority=\"%u\"/>\n",
                                           this->multipleStart,
                                           this->multipleRepeat,
                                           this->priority );
      break;

   case TAS_GRID_ASSIGN_DISTR_FILL:

      printHandler.Write( pfile, spaceLen, "<loc type=\"fill\" priority=\"%u\"/>\n",
                                           this->priority );
      break;

   case TAS_GRID_ASSIGN_DISTR_PERIMETER:

      printHandler.Write( pfile, spaceLen, "<loc type=\"perimeter\" priority=\"%u\"/>\n",
                                           this->priority );
      break;

   case TAS_GRID_ASSIGN_DISTR_UNDEFINED:
      break;
   }
}
