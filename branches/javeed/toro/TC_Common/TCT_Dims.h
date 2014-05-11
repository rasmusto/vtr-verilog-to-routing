//===========================================================================//
// Purpose : Template version for a TCT_Dims_c dimensions (dx,dy,dz)
//           class.
//
//           Inline methods include:
//           - TCT_Dims_c, ~TCT_Dims_c
//           - Set, Reset
//           - FindMin, FindMax
//           - FindArea
//           - HasArea
//
//           Public methods include:
//           - operator=
//           - operator==, operator!=
//           - ExtractString
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

#ifndef TCT_DIMS_H
#define TCT_DIMS_H

#include <cstdio>
#include <climits>
#include <cstring>
#include <string>
using namespace std;

#include "TIO_Typedefs.h"

#include "TCT_Generic.h"

#include "TC_Typedefs.h"
#include "TC_MinGrid.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
template< class T > class TCT_Dims_c
{
public:

   TCT_Dims_c( T dx = static_cast< T >( INT_MAX ), 
               T dy = static_cast< T >( INT_MAX ),
               T dz = static_cast< T >( INT_MAX ));
   TCT_Dims_c( const TCT_Dims_c< T >& dims );
   ~TCT_Dims_c( void );

   TCT_Dims_c< T >& operator=( const TCT_Dims_c< T >& dims );
   bool operator==( const TCT_Dims_c< T >& dims ) const;
   bool operator!=( const TCT_Dims_c< T >& dims ) const;

   void ExtractString( TC_DataMode_t mode,
                       string* psrData,
                       size_t precision = SIZE_MAX ) const;

   void Set( const TCT_Dims_c< T >& dims );
   void Set( T dx, T dy, T dz );
   void Set( T dx, T dy );
   void Reset( void );

   T FindMin( void ) const;
   T FindMax( void ) const;
   T FindArea( void ) const;

   bool HasArea( void ) const;

   bool IsValid( void ) const;

public:

   T dx;
   T dy;
   T dz;
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
template< class T > inline TCT_Dims_c< T >::TCT_Dims_c( 
      T dx_,
      T dy_,
      T dz_ )
      :
      dx( dx_ ),
      dy( dy_ ),
      dz( dz_ )
{
}

//===========================================================================//
template< class T > inline TCT_Dims_c< T >::TCT_Dims_c( 
      const TCT_Dims_c< T >& dims )
      :
      dx( dims.dx ),
      dy( dims.dy ),
      dz( dims.dz )
{
}

//===========================================================================//
template< class T > inline TCT_Dims_c< T >::~TCT_Dims_c( 
      void )
{
}

//===========================================================================//
template< class T > inline void TCT_Dims_c< T >::Set(
      const TCT_Dims_c< T >& dims )
{
   this->dx = dims.dx;
   this->dy = dims.dy;
   this->dz = dims.dz;
}

//===========================================================================//
template< class T > inline void TCT_Dims_c< T >::Set(
      T dx_,
      T dy_,
      T dz_ )
{
   this->dx = dx_;
   this->dy = dy_;
   this->dz = dz_;
}

//===========================================================================//
template< class T > inline void TCT_Dims_c< T >::Set(
      T dx_,
      T dy_ )
{
   this->dx = dx_;
   this->dy = dy_;
}

//===========================================================================//
template< class T > inline void TCT_Dims_c< T >::Reset(
      void )
{
   this->dx = static_cast< T >( INT_MAX );
   this->dy = static_cast< T >( INT_MAX );
   this->dz = static_cast< T >( INT_MAX );
}

//===========================================================================//
template< class T > inline T TCT_Dims_c< T >::FindMin( 
      void ) const
{
   return( TCT_Min( this->dx, this->dy ));
}

//===========================================================================//
template< class T > inline T TCT_Dims_c< T >::FindMax( 
      void ) const
{
   return( TCT_Max( this->dx, this->dy ));
}

//===========================================================================//
template< class T > inline T TCT_Dims_c< T >::FindArea( 
      void ) const
{
   return( this->dx * this->dy );
}

//===========================================================================//
template< class T > inline bool TCT_Dims_c< T >::HasArea( 
      void ) const
{
   return(( TCTF_IsGT( this->dx, static_cast< T >( 0 ))) &&
          ( TCTF_IsGT( this->dy, static_cast< T >( 0 ))) ?
          true : false );
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
template< class T > TCT_Dims_c< T >& TCT_Dims_c< T >::operator=( 
      const TCT_Dims_c< T >& dims )
{
   if( &dims != this )
   {
      this->dx = dims.dx;
      this->dy = dims.dy;
      this->dz = dims.dz;
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
template< class T > bool TCT_Dims_c< T >::operator==( 
      const TCT_Dims_c< T >& dims ) const
{
   return(( TCTF_IsEQ( this->dx, dims.dx )) &&
          ( TCTF_IsEQ( this->dy, dims.dy )) &&
          ( TCTF_IsEQ( this->dz, dims.dz )) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
template< class T > bool TCT_Dims_c< T >::operator!=( 
      const TCT_Dims_c< T >& dims ) const
{
   return(( !this->operator==( dims )) ? 
          true : false );
}

//===========================================================================//
// Method         : ExtractString
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
template<class T> void TCT_Dims_c< T >::ExtractString(
      TC_DataMode_t mode,
      string*       psrData,
      size_t        precision ) const
{
   if( psrData )
   {
      if( precision == SIZE_MAX )
      {
         precision = TC_MinGrid_c::GetInstance( ).GetPrecision( );
      }

      TCT_Dims_c< T >* pdims = const_cast< TCT_Dims_c< T >* >( this );

      char szData[TIO_FORMAT_STRING_LEN_DATA];
      memset( szData, 0, sizeof( szData ));

      switch( mode )
      {
      case TC_DATA_INT:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%d %d", 
                             *reinterpret_cast< int* >( &pdims->dx ),
                             *reinterpret_cast< int* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%d %d %d", 
                             *reinterpret_cast< int* >( &pdims->dx ),
                             *reinterpret_cast< int* >( &pdims->dy ),
                             *reinterpret_cast< int* >( &pdims->dz ));
         }
         break;

      case TC_DATA_UINT:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%u %u", 
                             *reinterpret_cast< unsigned int* >( &pdims->dx ),
                             *reinterpret_cast< unsigned int* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%u %u %u", 
                             *reinterpret_cast< unsigned int* >( &pdims->dx ),
                             *reinterpret_cast< unsigned int* >( &pdims->dy ),
                             *reinterpret_cast< unsigned int* >( &pdims->dz ));
         }
         break;

      case TC_DATA_LONG:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%ld %ld", 
                             *reinterpret_cast< long* >( &pdims->dx ),
                             *reinterpret_cast< long* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%ld %ld %ld", 
                             *reinterpret_cast< long* >( &pdims->dx ),
                             *reinterpret_cast< long* >( &pdims->dy ),
                             *reinterpret_cast< long* >( &pdims->dz ));
         }
         break;

      case TC_DATA_ULONG:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%lu %lu", 
                             *reinterpret_cast< unsigned long* >( &pdims->dx ),
                             *reinterpret_cast< unsigned long* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%lu %lu %lu", 
                             *reinterpret_cast< unsigned long* >( &pdims->dx ),
                             *reinterpret_cast< unsigned long* >( &pdims->dy ),
                             *reinterpret_cast< unsigned long* >( &pdims->dz ));
         }
         break;

      case TC_DATA_SIZE:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%lu %lu", 
                             *reinterpret_cast< unsigned long* >( &pdims->dx ),
                             *reinterpret_cast< unsigned long* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%lu %lu %lu", 
                             *reinterpret_cast< unsigned long* >( &pdims->dx ),
                             *reinterpret_cast< unsigned long* >( &pdims->dy ),
                             *reinterpret_cast< unsigned long* >( &pdims->dz ));
         }
         break;

      case TC_DATA_FLOAT:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%0.*f %0.*f", 
                             static_cast< int >( precision ), *reinterpret_cast< double* >( &pdims->dx ),
                             static_cast< int >( precision ), *reinterpret_cast< double* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%0.*f %0.*f %0.*f", 
                             static_cast< int >( precision ), *reinterpret_cast< double* >( &pdims->dx ),
                             static_cast< int >( precision ), *reinterpret_cast< double* >( &pdims->dy ),
                             static_cast< int >( precision ), *reinterpret_cast< double* >( &pdims->dz ));
         }
         break;

      case TC_DATA_EXP:
         if( TCTF_IsEQ( this->dz, static_cast< T >( INT_MAX )))
         {
            sprintf( szData, "%0.*e %0.*e", 
                             static_cast< int >( precision + 1 ), *reinterpret_cast< double* >( &pdims->dx ),
                             static_cast< int >( precision + 1 ), *reinterpret_cast< double* >( &pdims->dy ));
         }
         else
         {
            sprintf( szData, "%0.*e %0.*e %0.*e", 
                             static_cast< int >( precision + 1 ), *reinterpret_cast< double* >( &pdims->dx ),
                             static_cast< int >( precision + 1 ), *reinterpret_cast< double* >( &pdims->dy ),
                             static_cast< int >( precision + 1 ), *reinterpret_cast< double* >( &pdims->dz ));
         }
         break;

      case TC_DATA_STRING:
         break;

      case TC_DATA_UNDEFINED:
         sprintf( szData, "? ? ?" );
         break;
      }  

      *psrData = szData;
   }
}

//===========================================================================//
// Method         : IsValid
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
template< class T > bool TCT_Dims_c< T >::IsValid( 
      void ) const
{
   return(( TCTF_IsLT( this->dx, static_cast< T >( INT_MAX ))) &&
          ( TCTF_IsLT( this->dy, static_cast< T >( INT_MAX ))) ?
          true : false );
}

#endif 
