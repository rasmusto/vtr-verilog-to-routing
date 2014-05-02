//===========================================================================//
// Purpose : Declaration and inline definitions for a TAS_Cell class.
//
//           Inline methods include:
//           - SetName (required for TCT_SortedNameDynamicVector_c class)
//           - GetName (required for TCT_SortedNameDynamicVector_c class)
//           - GetCompare (required for TCT_BSearch and TCT_QSort classes)
//           - GetPortList
//           - GetSource
//           - AddPort
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

#ifndef TAS_CELL_H
#define TAS_CELL_H

#include <cstdio>
#include <string>
using namespace std;

#include "TGS_Point.h"

#include "TLO_Typedefs.h"
#include "TLO_Cell.h"

#include "TAS_Typedefs.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
class TAS_Cell_c
      :
      public TLO_Cell_c
{
public:

   TAS_Cell_c( void );
   TAS_Cell_c( const string& srName,
               TLO_CellSource_t source = TLO_CELL_SOURCE_UNDEFINED );
   TAS_Cell_c( const char* pszName,
               TLO_CellSource_t source = TLO_CELL_SOURCE_UNDEFINED );
   TAS_Cell_c( const TAS_Cell_c& cell );
   ~TAS_Cell_c( void );

   TAS_Cell_c& operator=( const TAS_Cell_c& cell );
   bool operator==( const TAS_Cell_c& cell ) const;
   bool operator!=( const TAS_Cell_c& cell ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void PrintXML( void ) const;
   void PrintXML( FILE* pfile, size_t spaceLen ) const;

   void SetName( const string& srName );
   void SetName( const char* pszName );
   const char* GetName( void ) const;
   const char* GetCompare( void ) const;

   const TLO_PortList_t& GetPortList( void ) const;
   TLO_CellSource_t GetSource( void ) const;

   void AddPort( const string& srName, 
                 TC_TypeMode_t type );
   void AddPort( const char* pszName,
                 TC_TypeMode_t type );
   void AddPort( const TLO_Port_c& port );

   bool IsValid( void ) const;

public:

   TAS_ClassType_t classType; // Selects pre-defined class type 
                              // (eg. "lut", "flipflop", or "memory")

private:

   TGS_FloatDims_t dims_;     // Defines width/height dimensions (dx,dy)
   TGS_Point_c     origin_;   // Defines reference origin (x,y)
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
inline void TAS_Cell_c::SetName( 
      const string& srName )
{
   TLO_Cell_c::SetName( srName );
}

//===========================================================================//
inline void TAS_Cell_c::SetName( 
      const char* pszName )
{
   TLO_Cell_c::SetName( pszName );
}

//===========================================================================//
inline const char* TAS_Cell_c::GetName( 
      void ) const
{
   return( TLO_Cell_c::GetName( ));
}

//===========================================================================//
inline const char* TAS_Cell_c::GetCompare( 
      void ) const 
{
   return( TLO_Cell_c::GetCompare( ));
}

//===========================================================================//
inline const TLO_PortList_t& TAS_Cell_c::GetPortList( 
      void ) const
{
   return( TLO_Cell_c::GetPortList( ));
}

//===========================================================================//
inline TLO_CellSource_t TAS_Cell_c::GetSource( 
      void ) const
{
   return( TLO_Cell_c::GetSource( ));
}

//===========================================================================//
inline void TAS_Cell_c::AddPort( 
      const string&       srName, 
            TC_TypeMode_t type )
{
   TLO_Cell_c::AddPort( srName, type );
}

//===========================================================================//
inline void TAS_Cell_c::AddPort( 
      const char*         pszName,
            TC_TypeMode_t type )
{
   TLO_Cell_c::AddPort( pszName, type );
}

//===========================================================================//
inline void TAS_Cell_c::AddPort( 
      const TLO_Port_c& port )
{
   TLO_Cell_c::AddPort( port );
}

//===========================================================================//
inline bool TAS_Cell_c::IsValid( 
      void ) const
{
   return( TLO_Cell_c::IsValid( ));
}

#endif
