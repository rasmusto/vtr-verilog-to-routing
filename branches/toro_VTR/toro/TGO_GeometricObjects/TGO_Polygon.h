//===========================================================================//
// Purpose : Declaration and inline definitions for a TGO_Polygon geometric 
//           object 2D polygon class.
//
//           Inline methods include:
//           - GetLength
//           - GetRegion
//           - IsValid
//
//
//===========================================================================//

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

#ifndef TGO_POLYGON_H
#define TGO_POLYGON_H

#include <cstdio>
using namespace std;

#include "TGO_Typedefs.h"
#include "TGO_Point.h"
#include "TGO_Region.h"
#include "TGO_Line.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/13 jeffr : Original
//===========================================================================//
class TGO_Polygon_c
{
public:

   TGO_Polygon_c( void );
   TGO_Polygon_c( const TGO_PointList_t& pointList );
   TGO_Polygon_c( const TGO_Region_c& region );
   TGO_Polygon_c( const TGO_Polygon_c& polygon );
   ~TGO_Polygon_c( void );

   TGO_Polygon_c& operator=( const TGO_Polygon_c& polygon );
   bool operator==( const TGO_Polygon_c& polygon ) const;
   bool operator!=( const TGO_Polygon_c& polygon ) const;

   TGO_Point_c* operator[]( size_t i );
   const TGO_Point_c* operator[]( size_t i ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void ExtractString( string* psrPolygon ) const;

   size_t GetLength( void ) const;
   const TGO_Region_c& GetRegion( void ) const;

   void Set( const TGO_PointList_t& pointList );
   void Set( const TGO_Region_c& region );
   void Set( const TGO_Polygon_c& polygon );
   void Reset( void );

   void Add( const TGO_PointList_t& pointList );
   void Add( const TGO_Point_c& point );
   void Add( const TGO_Point_c& pointA,
             const TGO_Point_c& pointB );
   void Add( const TGO_Line_c& edge );
   void Add( int x, int y );

   void Delete( const TGO_Point_c& point );

   void Replace( const TGO_Point_c& point,
                 const TGO_Point_c& pointPrime );

   size_t FindArea( void ) const;

   void FindNearest( const TGO_Point_c& refPoint, 
                     TGO_Point_c* pthisNearestPoint ) const;
   void FindNearest( const TGO_Point_c& refPoint, 
                     TGO_Line_c* pthisNearestEdge ) const;

   TC_SideMode_t FindSide( const TGO_Line_c& edge ) const;

   bool IsEdge( const TGO_PointList_t& pointList ) const;
   bool IsEdge( const TGO_Point_c& point ) const;
   bool IsEdge( const TGO_Line_c& edge ) const;
   bool IsEdge( int x, int y ) const;

   bool IsWithin( const TGO_PointList_t& pointList ) const;
   bool IsWithin( const TGO_Point_c& point ) const;
   bool IsWithin( int x, int y ) const;

   bool IsIntersecting( const TGO_Line_c& edge, 
                        TGO_Line_c* pedge = 0 ) const;
   bool IsIntersecting( const TGO_Line_c& edge, 
                        TGO_Point_c* ppoint ) const;

   bool IsCorner( const TGO_Point_c& point ) const;
   bool IsCorner( int x, int y ) const;
   bool IsConvexCorner( const TGO_Point_c& point ) const;
   bool IsConvexCorner( int x, int y ) const;
   bool IsConcaveCorner( const TGO_Point_c& point ) const;
   bool IsConcaveCorner( int x, int y ) const;

   bool IsOrthogonal( TGO_Point_c* ppointA = 0, 
                      TGO_Point_c* ppointB = 0 ) const;
   bool IsRectilinear( void ) const;

   bool IsValid( void ) const;

private:

   bool IsLastLinear_( const TGO_Line_c& edge ) const;
   bool IsLastConnected_( const TGO_Line_c& edge ) const;

private:

   TGO_PointList_t pointList_;
   TGO_Region_c region_;

   enum TGO_DefCapacity_e 
   { 
      TGO_POINT_LIST_DEF_CAPACITY = 4
   };
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/13 jeffr : Original
//===========================================================================//
inline size_t TGO_Polygon_c::GetLength(
      void ) const
{
   return( this->pointList_.GetLength( ));
}

//===========================================================================//
inline const TGO_Region_c& TGO_Polygon_c::GetRegion(
      void ) const
{
   return( this->region_ );
}

//===========================================================================//
inline bool TGO_Polygon_c::IsValid( 
      void ) const
{
   return( this->pointList_.IsValid( ) ? true : false );
}

#endif
