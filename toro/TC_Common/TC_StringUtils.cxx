//===========================================================================//
// Purpose : Function definitions for miscellaneous helpful string functions.
//
//           Functions include:
//           - TC_CompareStrings
//           - TC_FormatStringCentered
//           - TC_FormatStringFilled
//           - TC_FormatStringDateTimeStamp
//           - TC_FormatStringFileNameLineNum
//           - TC_FormatStringNameIndex
//           - TC_ParseStringNameIndex
//           - TC_ParseStringNameIndices
//           - TC_ExtractStringSideMode
//           - TC_ExtractStringTypeMode
//           - TC_AddStringPrefix
//           - TC_AddStringPostfix
//           - TC_stricmp
//           - TC_strnicmp
//           - TC_strdup
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

#include <cstdio>
#include <climits>
#include <ctime>
#include <cstring>
#include <string>
using namespace std;

#include "TCT_Generic.h"

#include "TIO_Typedefs.h"

#include "TC_StringUtils.h"

//===========================================================================//
// Function       : TC_CompareStrings
// Purpose        : Compare two strings, returning a negative value if the 
//                  1st string is less than the 2nd string, returning a
//                  positive value if the 1st string is greater than the
//                  2nd string, and returning a 0 if both strings are the
//                  same.  This function correctly handles alphanumeric
//                  strings in the form "xxx[nnn]" or "xxx(nnn)".  For
//                  example, the sort order ( "A[1]", "A[11]", "A[2]" )
//                  results from a generic string compare function, while
//                  the sort order ( "A[1]", "A[2]", "A[11]" ) results from
//                  this compare function.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
int TC_CompareStrings(
      const char* pszStringA,
      const char* pszStringB )
{
   // Compare given two strings, but only after testing for a quick
   // rejection based on 1st character from each given string
   int i = 0;

   // Loop until all characters in alphanumeric strings have been compared
   while( pszStringA && pszStringB )
   {
      if( *pszStringA <= '9' && *pszStringA >= '1' && 
          *pszStringB <= '9' && *pszStringB >= '1' )
      {
         // Handle special case when comparing digits from alphanumeric strings
         // (by design, 'atol( )' reads until 1st non-digit character is found)
         long lA = atol( pszStringA );
         long lB = atol( pszStringB );
         if(( lA != LONG_MAX ) && ( lB != LONG_MAX ))
         {
            long l = lA - lB;
            if( l == 0 )
            {
               // Skip past digits when digits within alphanumeric strings match
               while( *pszStringA <= '9' && *pszStringA >= '0' )
               {
                  ++pszStringA;
               }
               while( *pszStringB <= '9' && *pszStringB >= '0' )
               {
                  ++pszStringB;
               }
            }
            else
            {
               // Detected mis-compare between given alphanumeric strings
               i = ( l > 0 ? 1 : -1 );
               break;
            }
         }
         else
         {
            i = *pszStringA - *pszStringB;
            if( i == 0 )
            {
               // Skip past digits when digits within alphanumeric strings match
               ++pszStringA;
               ++pszStringB;
               continue;
            }
            else
            {
               // Detected mis-compare between given alphanumeric strings
               break;
            }
         }
      }
      else
      {
         // Handle case when comparing non-digits from alphanumeric strings
         i = *pszStringA - *pszStringB;
         if( i == 0 )
         {
            // Continue iterating characters in given alphanumeric strings
            if( *pszStringA && *pszStringB )
            {
               ++pszStringA;
               ++pszStringB;
            }
            else if( *pszStringA )
            {
               ++pszStringA;
            }
            else if( *pszStringB )
            {
               ++pszStringB;
            }
            else
            {
               break;
            }
         }
         else
         {
            // Detected mis-compare between given alphanumeric strings
            break;
         }
      }
   }
   return( i );
}

//===========================================================================//
int TC_CompareStrings(
      const string& srStringA,
      const string& srStringB )
{
   const char* pszStringA = srStringA.data( );
   const char* pszStringB = srStringB.data( );

   return( TC_CompareStrings( pszStringA, pszStringB ));
}

//===========================================================================//
int TC_CompareStrings(
      const void* pvoidA,
      const void* pvoidB )
{
   const char* pszStringA = static_cast< const char* >( pvoidA );
   const char* pszStringB = static_cast< const char* >( pvoidB );

   return( TC_CompareStrings( pszStringA, pszStringB ));
}

//===========================================================================//
// Function       : TC_FormatStringCentered
// Purpose        : Format and return a centered string based on the given 
//                  centered string reference length.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TC_FormatStringCentered(
      const char*  pszString,
            size_t lenRefer,
            char*  pszCentered,
            size_t lenCentered )
{
   if( pszString && pszCentered )
   {
      memset( pszCentered, 0, lenCentered );

      size_t lenString = strlen( pszString );
      size_t lenBeginSpc = ( lenRefer - lenString ) > 0 ? 
                           ( lenRefer - lenString ) / 2 : 0;
      size_t lenEndSpc = ( lenRefer - lenString + 1 ) > 0 ?
                         ( lenRefer - lenString + 1 ) / 2 : 0;

      if( lenCentered >= lenString + lenBeginSpc + lenEndSpc + 1 )
      {
         sprintf( pszCentered, "%*s%s%*s", 
                  static_cast< int >( lenBeginSpc ), lenBeginSpc > 0 ? " " : "",
                  pszString,
                  static_cast< int >( lenEndSpc ), lenEndSpc > 0 ? " " : "" );
      }
   }
   return( pszCentered && *pszCentered ? true : false );
}

//===========================================================================//
// Function       : TC_FormatStringFilled
// Purpose        : Format and return a filled string based on the given fill
//                  character.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TC_FormatStringFilled(
      char   cFill,
      char*  pszFilled,
      size_t lenFilled )
{
   if( pszFilled )
   {
      memset( pszFilled, 0, lenFilled + 1 );
      memset( pszFilled, cFill, lenFilled );
   }
   return( pszFilled && *pszFilled ? true : false );
}

//===========================================================================//
// Function       : TC_FormatStringDateTimeStamp
// Purpose        : Format and return a date/time stamp string with optional 
//                  prefix/postfix strings.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TC_FormatStringDateTimeStamp(
            char*  pszDateTimeStamp,
            size_t lenDateTimeStamp,
      const char*  pszPrefix,
      const char*  pszPostfix )
{
   if( pszDateTimeStamp )
   {
      memset( pszDateTimeStamp, 0, lenDateTimeStamp );

      time_t gmtTime;
      time( &gmtTime );
      struct tm* localTime = localtime( &gmtTime );

      size_t lenPrefix = pszPrefix ? strlen( pszPrefix ) : 0;
      size_t lenPostfix = pszPostfix ? strlen( pszPostfix ) : 0;

      if( lenDateTimeStamp >= 17 + lenPrefix + lenPostfix + 1 )
      {
         sprintf( pszDateTimeStamp, "%s%2.2d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d%s", 
                  TIO_PSZ_STR( pszPrefix ),
                  localTime->tm_mon + 1, 
                  localTime->tm_mday, 
                  localTime->tm_year % 100, 
                  localTime->tm_hour, 
                  localTime->tm_min, 
                  localTime->tm_sec,
                  TIO_PSZ_STR( pszPostfix ));
      }
   }
   return( pszDateTimeStamp && *pszDateTimeStamp ? true : false );
}

//===========================================================================//
// Function       : TC_FormatStringFileNameLineNum
// Purpose        : Format and return a file name/line number string with
//                  optional prefix/postfix strings.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 06/18/13 jeffr : Original
//===========================================================================//
bool TC_FormatStringFileNameLineNum(
            char*        pszFileNameLineNum,
            size_t       lenFileNameLineNum,
      const char*        pszFileName,
            unsigned int lineNum,
      const char*        pszPrefix,
      const char*        pszPostfix )
{
   if( pszFileNameLineNum )
   {
      memset( pszFileNameLineNum, 0, lenFileNameLineNum );

      size_t lenPrefix = strlen( pszPrefix );
      size_t lenPostfix = strlen( pszPostfix );
      size_t lenFileName = lenFileNameLineNum - lenPrefix - lenPostfix - 4;
      size_t lenLineNum = 5;

      if( lenFileNameLineNum >= lenPrefix + lenFileName + lenLineNum + lenPostfix )
      {
         sprintf( pszFileNameLineNum, "%s%.*s:%d%s", 
                  TIO_PSZ_STR( pszPrefix ),
                  static_cast<int>( lenFileName ), 
                  TIO_PSZ_STR( pszFileName ),
                  lineNum,
                  TIO_PSZ_STR( pszPostfix ));
      }
   }
   return( pszFileNameLineNum && *pszFileNameLineNum ? true : false );
}


//===========================================================================//
// Function       : TC_FormatStringNameIndex
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/12 jeffr : Original
//===========================================================================//
void TC_FormatStringNameIndex(
      const char*   pszName,
            size_t  index,
            string* psrNameIndex ) 
{
   if( psrNameIndex )
   {  
      *psrNameIndex = "";

      if( pszName )
      {
         *psrNameIndex += pszName;
      }
      if( index != SIZE_MAX )
      {
         char szIndex[TIO_FORMAT_STRING_LEN_VALUE];
         sprintf( szIndex, "%lu", static_cast< unsigned long >( index ));
         *psrNameIndex += "[";
         *psrNameIndex += szIndex;
         *psrNameIndex += "]";
      }
   }
}

//===========================================================================//
void TC_FormatStringNameIndex(
      const string& srName,
            size_t  index,
            string* psrNameIndex )
{
   const char* pszName = ( srName.length( ) ? srName.data( ) : 0 );
   TC_FormatStringNameIndex( pszName, index, psrNameIndex );
}

//===========================================================================//
// Function       : TC_ParseStringNameIndex
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/12 jeffr : Original
//===========================================================================//
bool TC_ParseStringNameIndex(
      const char*   pszNameIndex,
            string* psrName,
            size_t* pindex )
{
   string srName( "" );
   size_t index = SIZE_MAX;

   if( pszNameIndex )
   {
      srName = pszNameIndex;
      size_t left = srName.find( '[' );
      size_t right = srName.find( ']' );
      if(( left != string::npos ) && ( right != string::npos ))
      {
         if( left < right )
         {
            string srIndex( srName.substr( left + 1, right - left - 1 ));
            index = atoi( srIndex.data( ));

            srName = srName.substr( 0, left );
         }
      }
   }

   if( psrName )
   {
      *psrName = srName;
   }
   if( pindex )
   {
      *pindex = index;
   }
   return( index != SIZE_MAX ? true : false );
}

//===========================================================================//
bool TC_ParseStringNameIndex(
      const string& srNameIndex,
            string* psrName,
            size_t* pindex )
{
   const char* pszNameIndex = ( srNameIndex.length( ) ? srNameIndex.data( ) : 0 );
   return( TC_ParseStringNameIndex( pszNameIndex, psrName, pindex ));
}

//===========================================================================//
// Function       : TC_ParseStringNameIndices
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/12 jeffr : Original
//===========================================================================//
bool TC_ParseStringNameIndices(
      const char*   pszNameIndices,
            string* psrName,
            size_t* pindex_i,
            size_t* pindex_j )
{
   string srName( "" );
   size_t index_i = SIZE_MAX;
   size_t index_j = SIZE_MAX;

   if( pszNameIndices )
   {
      srName = pszNameIndices;
      size_t left = srName.find( '[' );
      size_t right = srName.find( ']' );
      size_t colon = srName.find( ':' );
      if(( left != string::npos ) && ( right != string::npos ) && ( colon != string::npos))
      {
         if(( left < colon ) && (colon < right ))
         {
            string srIndex_i( srName.substr( left + 1, colon - left - 1 ));
            index_i = atoi( srIndex_i.data( ));

            string srIndex_j( srName.substr( colon + 1, right - colon - 1 ));
            index_j = atoi( srIndex_j.data( ));

            srName = srName.substr( 0, left );
         }
      }
      else if(( left != string::npos ) && ( right != string::npos ))
      {
         if( left < right )
         {
            string srIndex( srName.substr( left + 1, right - left - 1 ));
            index_i = index_j = atoi( srIndex.data( ));

            srName = srName.substr( 0, left );
         }
      }
   }

   if( psrName )
   {
      *psrName = srName;
   }
   if( pindex_i )
   {
      *pindex_i = index_i;
   }
   if( pindex_j )
   {
      *pindex_j = index_j;
   }
   return(( index_i != SIZE_MAX ) && ( index_j != SIZE_MAX ) ? true : false );
}

//===========================================================================//
bool TC_ParseStringNameIndices(
      const string& srNameIndices,
            string* psrName,
            size_t* pindex_i,
            size_t* pindex_j )
{
   const char* pszNameIndices = ( srNameIndices.length( ) ? srNameIndices.data( ) : 0 );
   return( TC_ParseStringNameIndices( pszNameIndices, psrName, pindex_i, pindex_j ));
}

//===========================================================================//
// Function       : TC_ExtractStringSideMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
void TC_ExtractStringSideMode(
      TC_SideMode_t sideMode,
      string*       psrSideMode )
{
   if( psrSideMode )
   {
      *psrSideMode = "";

      switch( sideMode )
      {
      case TC_SIDE_LEFT:   *psrSideMode = "left";   break;
      case TC_SIDE_RIGHT:  *psrSideMode = "right";  break;
      case TC_SIDE_LOWER:  *psrSideMode = "lower";  break;
      case TC_SIDE_UPPER:  *psrSideMode = "upper";  break;

      case TC_SIDE_BOTTOM: *psrSideMode = "bottom"; break;
      case TC_SIDE_TOP:    *psrSideMode = "top";    break;

      case TC_SIDE_PREV:   *psrSideMode = "prev";   break;
      case TC_SIDE_NEXT:   *psrSideMode = "next";   break;

      default:             *psrSideMode = "?";      break;
      }
   }
}

//===========================================================================//
// Function       : TC_ExtractStringTypeMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
void TC_ExtractStringTypeMode(
      TC_TypeMode_t typeMode,
      string*       psrTypeMode )
{
   if( psrTypeMode )
   {
      *psrTypeMode = "";

      switch( typeMode )
      {
      case TC_TYPE_INPUT:  *psrTypeMode = "input";  break;
      case TC_TYPE_OUTPUT: *psrTypeMode = "output"; break;
      case TC_TYPE_SIGNAL: *psrTypeMode = "signal"; break;
      case TC_TYPE_CLOCK:  *psrTypeMode = "clock";  break;
      case TC_TYPE_POWER:  *psrTypeMode = "power";  break;
      case TC_TYPE_GLOBAL: *psrTypeMode = "global"; break;
      default:             *psrTypeMode = "?";      break;
      }
   }
}

//===========================================================================//
// Function       : TC_AddStringPrefix
// Purpose        : Add the given prefix to the given string.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TC_AddStringPrefix(
            char* pszString,
      const char* pszPrefix )
{
   if( pszString && pszPrefix )
   {
      size_t lenString = strlen( pszString );
      size_t lenPrefix = strlen( pszPrefix );

      size_t lenCopy = TCT_Min( lenString, lenPrefix );
      memcpy( pszString, pszPrefix, lenCopy );
   }
   return( pszPrefix && *pszPrefix ? true : false );
}

//===========================================================================//
// Function       : TC_AddStringPostfix
// Purpose        : Add the given postfix to the given string.
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TC_AddStringPostfix(
            char* pszString,
      const char* pszPostfix )
{
   if( pszString && pszPostfix )
   {
      size_t lenString = strlen( pszString );
      size_t lenPostfix = strlen( pszPostfix );

      size_t lenCopy = TCT_Min( lenString, lenPostfix );
      memcpy( pszString + lenString - lenCopy, pszPostfix, lenCopy );
   }
   return( pszPostfix && *pszPostfix ? true : false );
}

//===========================================================================//
// Function       : TC_stricmp
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
int TC_stricmp( 
      const char* pszStringA, 
      const char* pszStringB )
{
   while(( pszStringA && *pszStringA ) && 
         ( pszStringB && *pszStringB ) && 
         ( toupper( *pszStringA ) == toupper( *pszStringB )))
   {
      ++pszStringA;
      ++pszStringB;
   }
   return(( !pszStringA && !pszStringA ) ? 0 : ( *pszStringA - *pszStringB ));
} 

//===========================================================================//
int TC_strnicmp( 
      const string& srStringA, 
      const string& srStringB )
{
   const char* pszStringA = srStringA.data( );
   const char* pszStringB = srStringB.data( );
   
   return( TC_stricmp( pszStringA, pszStringB ));
}

//===========================================================================//
// Function       : TC_strnicmp
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
int TC_strnicmp( 
      const char* pszStringA, 
      const char* pszStringB,
            int   i )
{
   while(( pszStringA && *pszStringA ) && 
         ( pszStringB && *pszStringB ) && 
         ( toupper( *pszStringA ) == toupper( *pszStringB )) &&
         ( i ))
   {
      ++pszStringA;
      ++pszStringB;
      --i;
   }

   pszStringA = ( i > 0 ? pszStringA : 0 );
   pszStringB = ( i > 0 ? pszStringB : 0 );

   return(( !pszStringA && !pszStringA ) ? 0 : ( *pszStringA - *pszStringB ));
} 

//===========================================================================//
int TC_strnicmp( 
      const string& srStringA, 
      const string& srStringB,
            int     i )
{
   const char* pszStringA = srStringA.data( );
   const char* pszStringB = srStringB.data( );
   
   return( TC_strnicmp( pszStringA, pszStringB, i ));
}

//===========================================================================//
// Function       : TC_strdup
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 07/10/12 jeffr : Original
//===========================================================================//
char* TC_strdup(
      const char* pszString )
{
   char* pszDupString = 0;

   if( pszString )
   {
      size_t lenString = strlen( pszString );

      pszDupString = static_cast< char* >( malloc( lenString + 1 ));
      if( pszDupString )
      {
         memcpy( pszDupString, pszString, lenString + 1 );
      }
   }
   return( pszDupString );
}

//===========================================================================//
char* TC_strdup(
      const string& srString )
{
   const char* pszString = srString.data( );
   
   return( TC_strdup( pszString ));
}
