//===========================================================================//
// Purpose : Method definitions for the TC_NameFile class.
//
//           Public methods include:
//           - TC_NameFile_c, ~TC_NameFile_c
//           - operator=, operator<
//           - operator==, operator!=
//           - Print
//           - ExtractString
//           - Set
//           - Clear
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
#include "TC_NameFile.h"

//===========================================================================//
// Method         : TC_NameFile_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TC_NameFile_c::TC_NameFile_c( 
      void )
{
} 

//===========================================================================//
TC_NameFile_c::TC_NameFile_c( 
      const string& srName,
      const string& srFileName )
      :
      srName_( srName ),
      srFileName_( srFileName )
{
} 

//===========================================================================//
TC_NameFile_c::TC_NameFile_c( 
      const char* pszName,
      const char* pszFileName )
      :
      srName_( TIO_PSZ_STR( pszName )),
      srFileName_( TIO_PSZ_STR( pszFileName ))
{
} 

//===========================================================================//
TC_NameFile_c::TC_NameFile_c( 
      const TC_NameFile_c& nameFile )
      :
      srName_( nameFile.srName_ ),
      srFileName_( nameFile.srFileName_ )
{
} 

//===========================================================================//
// Method         : ~TC_NameFile_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TC_NameFile_c::~TC_NameFile_c( 
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
TC_NameFile_c& TC_NameFile_c::operator=( 
      const TC_NameFile_c& nameFile )
{
   if( &nameFile != this )
   {
      this->srName_ = nameFile.srName_;
      this->srFileName_ = nameFile.srFileName_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator<
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TC_NameFile_c::operator<( 
      const TC_NameFile_c& nameFile ) const
{
   return(( TC_CompareStrings( this->srName_, nameFile.srName_ ) < 0 ) ? 
          true : false );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TC_NameFile_c::operator==( 
      const TC_NameFile_c& nameFile ) const
{
   return(( this->srName_ == nameFile.srName_ ) &&
          ( this->srFileName_ == nameFile.srFileName_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TC_NameFile_c::operator!=( 
      const TC_NameFile_c& nameFile ) const
{
   return( !this->operator==( nameFile ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_NameFile_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   string srNameFile;
   this->ExtractString( &srNameFile );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );
   printHandler.Write( pfile, spaceLen, "%s\n", TIO_SR_STR( srNameFile ));
}

//===========================================================================//
// Method         : ExtractString
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_NameFile_c::ExtractString( 
      string* psrNameFile ) const
{
   if( psrNameFile )
   {
      if( this->IsValid( ))
      {
         *psrNameFile = "\"";
         *psrNameFile += this->srName_;
         *psrNameFile += "\"";

         if( this->srFileName_.length( ))
         {
            *psrNameFile += " ";
            *psrNameFile += "\"";
            *psrNameFile += this->srFileName_;
            *psrNameFile += "\"";
         }
      }
      else
      {
         *psrNameFile = "?";
      }
   }
} 

//===========================================================================//
// Method         : Set
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_NameFile_c::Set( 
      const string& srName,
      const string& srFileName )
{
   this->srName_ = srName;
   this->srFileName_ = srFileName;
} 

//===========================================================================//
void TC_NameFile_c::Set( 
      const char* pszName,
      const char* pszFileName )
{
   this->srName_ = TIO_PSZ_STR( pszName );
   this->srFileName_ = TIO_PSZ_STR( pszFileName );
} 


//===========================================================================//
// Method         : Clear
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TC_NameFile_c::Clear( 
      void )
{
   this->srName_ = "";
   this->srFileName_ = "";
} 
