//===========================================================================//
// Purpose : Declaration and inline definitions for a TOP_OptionsFile class.
//
//           Inline methods include:
//           - IsValid
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

#ifndef TOP_OPTIONS_FILE_H
#define TOP_OPTIONS_FILE_H

#include <cstdio>
using namespace std;

#include "TOS_OptionsStore.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
class TOP_OptionsFile_c
{
public:

   TOP_OptionsFile_c( FILE* pfile, 
                      const char* pszFileParserName,
                      TOS_OptionsStore_c* poptionsStore );
   ~TOP_OptionsFile_c( void );

   bool SyntaxError( unsigned int lineNum, 
                     const string& srFileName,
                     const char* pszMessageText );

   bool IsValid( void ) const;

private:

   bool ok_;
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
inline bool TOP_OptionsFile_c::IsValid( void ) const
{
   return( this->ok_ );
}

#endif
