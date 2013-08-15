//===========================================================================//
// Purpose : Method definitions for the TCH_PrePlacedBlock class.
//
//           Public methods include:
//           - TCH_PrePlacedBlock_c, ~TCH_PrePlacedBlock_c
//           - operator=, operator<
//           - operator==, operator!=
//           - Print
//
//===========================================================================//

#include "TC_StringUtils.h"

//---------------------------------------------------------------------------//
// Copyright (C) 2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com)      //
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

#include "TIO_PrintHandler.h"

#include "TCH_StringUtils.h"
#include "TCH_PrePlacedBlock.h"

//===========================================================================//
// Method         : TCH_PrePlacedBlock_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
TCH_PrePlacedBlock_c::TCH_PrePlacedBlock_c( 
      void )
      :
      status_( TCH_PLACE_STATUS_UNDEFINED ),
      vpr_index_( -1 ),
      vpr_type_( 0 )
{
} 

//===========================================================================//
TCH_PrePlacedBlock_c::TCH_PrePlacedBlock_c( 
      const string& srName )
      :
      srName_( srName ),
      status_( TCH_PLACE_STATUS_UNDEFINED ),
      vpr_index_( -1 ),
      vpr_type_( 0 )
{
} 

//===========================================================================//
TCH_PrePlacedBlock_c::TCH_PrePlacedBlock_c( 
      const char* pszName )
      :
      srName_( TIO_PSZ_STR( pszName )),
      status_( TCH_PLACE_STATUS_UNDEFINED ),
      vpr_index_( -1 ),
      vpr_type_( 0 )
{
} 

//===========================================================================//
TCH_PrePlacedBlock_c::TCH_PrePlacedBlock_c( 
      const string&               srName,
            TCH_PlaceStatusMode_t status,
      const TGO_Point_c&          origin,
            int                   vpr_index,
      const t_type_descriptor*    vpr_type )
      :
      srName_( srName ),
      status_( status ),
      origin_( origin ),
      vpr_index_( vpr_index ),
      vpr_type_( const_cast< t_type_descriptor* >( vpr_type ))
{
} 

//===========================================================================//
TCH_PrePlacedBlock_c::TCH_PrePlacedBlock_c( 
      const char*                 pszName,
            TCH_PlaceStatusMode_t status,
      const TGO_Point_c&          origin,
            int                   vpr_index,
      const t_type_descriptor*    vpr_type )
      :
      srName_( TIO_PSZ_STR( pszName )),
      status_( status ),
      origin_( origin ),
      vpr_index_( vpr_index ),
      vpr_type_( const_cast< t_type_descriptor* >( vpr_type ))
{
} 

//===========================================================================//
TCH_PrePlacedBlock_c::TCH_PrePlacedBlock_c( 
      const TCH_PrePlacedBlock_c& relativeBlock )
      :
      srName_( relativeBlock.srName_ ),
      status_( relativeBlock.status_ ),
      origin_( relativeBlock.origin_ ),
      vpr_index_( relativeBlock.vpr_index_ ),
      vpr_type_( relativeBlock.vpr_type_ )
{
} 

//===========================================================================//
// Method         : ~TCH_PrePlacedBlock_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
TCH_PrePlacedBlock_c::~TCH_PrePlacedBlock_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
TCH_PrePlacedBlock_c& TCH_PrePlacedBlock_c::operator=( 
      const TCH_PrePlacedBlock_c& relativeBlock )
{
   if( &relativeBlock != this )
   {
      this->srName_ = relativeBlock.srName_;
      this->status_ = relativeBlock.status_;
      this->origin_ = relativeBlock.origin_;
      this->vpr_index_ = relativeBlock.vpr_index_;
      this->vpr_type_ = relativeBlock.vpr_type_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator<
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
bool TCH_PrePlacedBlock_c::operator<( 
      const TCH_PrePlacedBlock_c& relativeBlock ) const
{
   return(( TC_CompareStrings( this->srName_, relativeBlock.srName_ ) < 0 ) ? 
          true : false );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
bool TCH_PrePlacedBlock_c::operator==( 
      const TCH_PrePlacedBlock_c& relativeBlock ) const
{
   return(( this->srName_ == relativeBlock.srName_ ) &&
          ( this->status_ == relativeBlock.status_ ) &&
          ( this->origin_ == relativeBlock.origin_ ) &&
          ( this->vpr_index_ == relativeBlock.vpr_index_ ) &&
          ( this->vpr_type_ == relativeBlock.vpr_type_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
bool TCH_PrePlacedBlock_c::operator!=( 
      const TCH_PrePlacedBlock_c& relativeBlock ) const
{
   return( !this->operator==( relativeBlock ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
void TCH_PrePlacedBlock_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<block" );
   printHandler.Write( pfile, 0, " name=\"%s\"", TIO_SR_STR( this->srName_ ));
   if( this->status_ != TCH_PLACE_STATUS_UNDEFINED )
   {
      string srStatus;
      TCH_ExtractStringStatusMode( this->status_, &srStatus );
      printHandler.Write( pfile, 0, " status=%s", TIO_SR_STR( srStatus ));
   }

   if( this->origin_.IsValid( ))
   {
      string srOrigin;
      this->origin_.ExtractString( &srOrigin );
      printHandler.Write( pfile, 0, " origin=%s", TIO_SR_STR( srOrigin ));
   }
   if( this->vpr_index_ != -1 )
   {
      printHandler.Write( pfile, 0, " vpr_index=%d", this->vpr_index_ );
   }
   if( this->vpr_type_ )
   {
      printHandler.Write( pfile, 0, " vpr_type=[0x%08x]", this->vpr_type_ );
   }
   printHandler.Write( pfile, 0, ">\n" );
} 
