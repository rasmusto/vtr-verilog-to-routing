//===========================================================================//
// Purpose : Method definitions for the (regexp) TC_Bit class.
//
//           Public methods include:
//           - TC_Bit, ~TC_Bit
//           - operator=
//           - operator==, operator!=
//           - Print
//           - ExtractString
//           - GetValue
//           - SetValue
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

#include "TIO_PrintHandler.h"

#include "TC_StringUtils.h"
#include "TC_Bit.h"

//===========================================================================//
// Method         : TC_Bit_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TC_Bit_c::TC_Bit_c( 
      TC_BitMode_t value )
{
   this->SetValue( value );
}

//===========================================================================//
TC_Bit_c::TC_Bit_c( 
      char value )
{
   this->SetValue( value );
}

//===========================================================================//
TC_Bit_c::TC_Bit_c( 
      const TC_Bit_c& bit )
      :
      value_( bit.value_ )
{
}

//===========================================================================//
// Method         : ~TC_Bit_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TC_Bit_c::~TC_Bit_c( 
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
TC_Bit_c& TC_Bit_c::operator=( 
      const TC_Bit_c& bit )
{
   if( &bit != this )
   {
      this->value_ = bit.value_;
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
bool TC_Bit_c::operator==( 
      const TC_Bit_c& bit ) const
{
   return( this->value_ == bit.value_ ? true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TC_Bit_c::operator!=( 
      const TC_Bit_c& bit ) const
{
   return(( !this->operator==( bit )) ?
          true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_Bit_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   string srValue;
   this->ExtractString( &srValue );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );
   printHandler.Write( pfile, spaceLen, "%s\n", TIO_SR_STR( srValue ));
}

//===========================================================================//
// Method         : ExtractString
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_Bit_c::ExtractString( 
      string* psrValue ) const
{
   if( psrValue )
   {
      *psrValue = "";

      switch( this->value_ )
      {
      case TC_BIT_TRUE:      *psrValue = "1"; break;
      case TC_BIT_FALSE:     *psrValue = "0"; break;
      case TC_BIT_DONT_CARE: *psrValue = "-"; break;
      default:               *psrValue = "?"; break;
      }
   }
} 

//===========================================================================//
// Method         : GetValue
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TC_BitMode_t TC_Bit_c::GetValue( 
      void ) const 
{
   return( static_cast< TC_BitMode_t >( this->value_ ));
}

//===========================================================================//
// Method         : SetValue
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_Bit_c::SetValue( 
      TC_BitMode_t value )
{
   this->value_ = static_cast< char >( value );
}

//===========================================================================//
void TC_Bit_c::SetValue( 
      char value )
{
   switch( value )
   {
   case '1': this->value_ = TC_BIT_TRUE;      break;
   case '0': this->value_ = TC_BIT_FALSE;     break;
   case '-': this->value_ = TC_BIT_DONT_CARE; break;
   default:  this->value_ = TC_BIT_UNDEFINED; break;
   }
}
