//===========================================================================//
// Purpose : Declaration and inline definitions for a TFV_FabricPlane class.
//
//           Inline methods include:
//           - Clear
//           - GetRegion
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

#ifndef TFV_FABRIC_PLANE_H
#define TFV_FABRIC_PLANE_H

#include <cstdio>
#include <string>
using namespace std;

#include "TTPT_TilePlane.h"

#include "TFV_Typedefs.h"
#include "TFV_FabricData.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
class TFV_FabricPlane_c 
      : 
      public TTPT_TilePlane_c< TFV_FabricData_c >
{
public:

   TFV_FabricPlane_c( void );
   TFV_FabricPlane_c( const TGS_Region_c& region );
   TFV_FabricPlane_c( const TFV_FabricPlane_c& fabricPlane );
   ~TFV_FabricPlane_c( void );

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;
   void PrintLaff( void ) const;
   void PrintLaff( const char* pszFileName ) const;

   bool Init( const TGS_Region_c& region );
   bool Init( const TFV_FabricPlane_t& fabricPlane );

   void Clear( void );

   const TGS_Region_c& GetRegion( void ) const;

   bool Add( const TGS_Region_c& region,
             TFV_AddMode_t addMode );
   bool Add( const TGS_Region_c& region,
             const TFV_FabricData_c& fabricData,
             TFV_AddMode_t addMode );

   bool Delete( const TGS_Region_c& region,
                TFV_DeleteMode_t deleteMode );
   bool Delete( const TGS_Region_c& region,
                const TFV_FabricData_c& fabricData,
                TFV_DeleteMode_t deleteMode );

   bool Find( const TGS_Point_c& point,
              TFV_FabricFigure_t** ppfabricFigure = 0 ) const;
   bool Find( const TGS_Region_c& region,
              TFV_FabricFigure_t** ppfabricFigure = 0 ) const;
   bool Find( const TGS_Region_c& region,
              TFV_FindMode_t findMode,
              TFV_FigureMode_t figureMode,
              TFV_FabricFigure_t** ppfabricFigure = 0 ) const;
   bool Find( const TGS_Region_c& region,
              TFV_FindMode_t findMode,
              TFV_FigureMode_t figureMode,
              TGS_Region_c* pfabricRegion ) const;
   bool Find( const TGS_Region_c& region,
              TFV_FabricData_c** ppfabricData = 0 ) const;
   bool Find( const TGS_Region_c& region,
              TFV_FindMode_t findMode,
              TFV_FigureMode_t figureMode,
              TFV_FabricData_c** ppfabricData = 0 ) const;

   bool FindNearest( const TGS_Region_c& region,
                     TFV_FabricFigure_t** ppfabricFigure,
                     double maxDistance = TC_FLT_MAX ) const;
   bool FindNearest( const TGS_Region_c& region,
                     TC_SideMode_t sideMode,
                     TFV_FabricFigure_t** ppfabricFigure ) const;

   bool IsClear( const TGS_Point_c& point ) const;
   bool IsClear( TFV_IsClearMode_t isClearMode,
                 const TGS_Region_c& region ) const;

   bool IsSolid( const TGS_Point_c& point,
                 TFV_FabricFigure_t** ppfabricFigure = 0 ) const;
   bool IsSolid( TFV_IsSolidMode_t isSolidMode,
                 const TGS_Region_c& region,
                 TFV_FabricFigure_t** ppfabricFigure = 0 ) const;

   bool IsValid( void ) const;

private:

   TFV_FabricPlaneIter_t fabricPlaneIter_; // Local cached fabric plane iterator
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 08/15/12 jeffr : Original
//===========================================================================//
inline void TFV_FabricPlane_c::Clear( 
      void )
{
   TFV_FabricPlane_t::Clear( );
} 

//===========================================================================//
inline const TGS_Region_c& TFV_FabricPlane_c::GetRegion( 
      void ) const
{
   return( TFV_FabricPlane_t::GetRegion( ));
} 

//===========================================================================//
inline bool TFV_FabricPlane_c::IsValid( 
      void ) const
{
   return( TFV_FabricPlane_t::IsValid( ));
} 

#endif
