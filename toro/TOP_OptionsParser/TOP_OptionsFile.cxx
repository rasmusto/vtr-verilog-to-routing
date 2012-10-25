//===========================================================================//
// Purpose : Method definitions for the TOP_OptionsFile class.
//
//           Public methods include:
//           - TOP_OptionsFile, ~TOP_OptionsFile
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

#include "stdpccts.h"
#include "ATokPtr.h"
#include "GenericTokenBuffer.h"

#include "TIO_PrintHandler.h"

#include "TOP_OptionsScanner_c.h"
#include "TOP_OptionsParser_c.h"
#include "TOP_OptionsFile.h"

//===========================================================================//
// Method         : TOP_OptionsFile_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
TOP_OptionsFile_c::TOP_OptionsFile_c( 
            FILE*               pfile,    
      const char*               pszFileName,
            TOS_OptionsStore_c* poptionsStore )
{
   this->ok_ = true;

   // Define an input file stream for DLG
   DLGFileInput inputFileStream( pfile );

   // Define a scanner attached to the file stream
   TOP_OptionsScanner_c optionsScanner( &inputFileStream );

   // Define a token buffer attached to the scanner
   GenericTokenBuffer tokenBuffer( &optionsScanner );

   // Generate and pass a token to the scanner
   optionsScanner.setToken( new TC_NOTHROW ANTLRToken );

   // Define a token parser attached to token buffer
   TOP_OptionsParser_c optionsParser( &tokenBuffer );

   // Define a pointer to the default scanner class
   optionsParser.SetScanner( &optionsScanner );

   // Define input file name (for print messages)
   optionsParser.SetFileName( pszFileName );

   // Define a pointer to the current options file parser class
   optionsParser.SetOptionsFile( this );

   // Define a pointer to the given options store class
   optionsParser.SetOptionsStore( poptionsStore );

   // Initialize and start the parser
   optionsParser.init();
   optionsParser.start();
}

//===========================================================================//
// Method         : ~TOP_OptionsFile_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
TOP_OptionsFile_c::~TOP_OptionsFile_c( 
      void )
{
}

//===========================================================================//
// Method         : SyntaxError
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TOP_OptionsFile_c::SyntaxError( 
            unsigned int lineNum, 
      const string&      srFileName,
      const char*        pszMessageText )
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   this->ok_ = printHandler.Error( "Syntax error %s.\n"
                                   "%sSee file '%s', line %d.\n",
                                   TIO_PSZ_STR( pszMessageText ),
                                   TIO_PREFIX_ERROR_SPACE,
                                   TIO_SR_STR( srFileName ), lineNum );
   return( this->ok_ );
}
