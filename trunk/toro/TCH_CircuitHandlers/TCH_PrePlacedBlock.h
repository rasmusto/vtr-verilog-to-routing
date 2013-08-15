//===========================================================================//
// Purpose : Declaration and inline definitions for the TCH_PrePlacedBlock 
//           class.
//
//           Inline methods include:
//           - SetName (required for TCT_SortedNameDynamicVector_c class)
//           - GetName (required for TCT_SortedNameDynamicVector_c class)
//           - GetCompare (required for TCT_BSearch and TCT_QSort classes)
//           - SetBlockName
//           - SetStatus, SetOrigin
//           - SetVPR_Index, SetVPR_Type
//           - GetBlockName
//           - GetStatus, GetOrigin
//           - GetVPR_Index, GetVPR_Type
//           - IsValid
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

#ifndef TCH_RELATIVE_BLOCK_H
#define TCH_RELATIVE_BLOCK_H

#include <cstdio>
using namespace std;

#include "TGO_Point.h"

#include "TCH_Typedefs.h"

#include "vpr_api.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
class TCH_PrePlacedBlock_c
{
public:

   TCH_PrePlacedBlock_c( void );
   TCH_PrePlacedBlock_c( const string& srName );
   TCH_PrePlacedBlock_c( const char* pszName );
   TCH_PrePlacedBlock_c( const string& srName,
                         TCH_PlaceStatusMode_t status,
                         const TGO_Point_c& origin,
                         int vpr_index = -1,
                         const t_type_descriptor* vpr_type = 0 );
   TCH_PrePlacedBlock_c( const char* pszName,
                         TCH_PlaceStatusMode_t status,
                         const TGO_Point_c& origin,
                         int vpr_index = -1,
                         const t_type_descriptor* vpr_type = 0 );
   TCH_PrePlacedBlock_c( const TCH_PrePlacedBlock_c& relativeBlock );
   ~TCH_PrePlacedBlock_c( void );

   TCH_PrePlacedBlock_c& operator=( const TCH_PrePlacedBlock_c& relativeBlock );
   bool operator<( const TCH_PrePlacedBlock_c& relativeBlock ) const;
   bool operator==( const TCH_PrePlacedBlock_c& relativeBlock ) const;
   bool operator!=( const TCH_PrePlacedBlock_c& relativeBlock ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void SetName( const string& srName );
   void SetName( const char* pszName );
   const char* GetName( void ) const;
   const char* GetCompare( void ) const;

   void SetBlockName( const string& srName );
   void SetBlockName( const char* pszName );
   void SetStatus( TCH_PlaceStatusMode_t status );
   void SetOrigin( const TGO_Point_c& origin );
   void SetVPR_Index( int vpr_index );
   void SetVPR_Type( const t_type_descriptor* vpr_type );

   const char* GetBlockName( void ) const;
   TCH_PlaceStatusMode_t GetStatus( void ) const;
   const TGO_Point_c& GetOrigin( void ) const;
   int GetVPR_Index( void ) const;
   const t_type_descriptor* GetVPR_Type( void ) const;

   bool IsValid( void ) const;

private:

   string                srName_; // Name asso. with this pre-placed block
   TCH_PlaceStatusMode_t status_; // Status asso. with this pre-placed block
   TGO_Point_c           origin_; // Origin asso. with this pre-placed block

   int                vpr_index_; // VPR index asso. with this pre-placed block
   t_type_descriptor* vpr_type_;  // VPR type asso. with this pre-placed block
};

//===========================================================================//
// Purpose        : Class inline definition(s)
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/18/13 jeffr : Original
//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetName( 
      const string& srName )
{
   this->srName_ = srName;
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetName( 
      const char* pszName )
{
   this->srName_ = TIO_PSZ_STR( pszName );
}

//===========================================================================//
inline const char* TCH_PrePlacedBlock_c::GetName( 
      void ) const
{
   return( TIO_SR_STR( this->srName_ ));
}

//===========================================================================//
inline const char* TCH_PrePlacedBlock_c::GetCompare( 
      void ) const 
{
   return( TIO_SR_STR( this->srName_ ));
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetBlockName( 
      const string& srBlockName )
{
   this->srName_ = srBlockName;
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetBlockName( 
      const char* pszBlockName )
{
   this->srName_ = TIO_PSZ_STR( pszBlockName );
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetStatus(
      TCH_PlaceStatusMode_t status )
{
   this->status_ = status;
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetOrigin(
      const TGO_Point_c& origin )
{
   this->origin_ = origin;
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetVPR_Index(
      int vpr_index )
{
   this->vpr_index_ = vpr_index;
}

//===========================================================================//
inline void TCH_PrePlacedBlock_c::SetVPR_Type( 
      const t_type_descriptor* vpr_type )
{
   this->vpr_type_ = const_cast< t_type_descriptor* >( vpr_type );
} 

//===========================================================================//
inline const char* TCH_PrePlacedBlock_c::GetBlockName( 
      void ) const
{
   return( TIO_SR_STR( this->srName_ ));
}

//===========================================================================//
inline TCH_PlaceStatusMode_t TCH_PrePlacedBlock_c::GetStatus( 
      void ) const
{
   return( this->status_ );
}

//===========================================================================//
inline const TGO_Point_c& TCH_PrePlacedBlock_c::GetOrigin( 
      void ) const
{
   return( this->origin_ );
}

//===========================================================================//
inline int TCH_PrePlacedBlock_c::GetVPR_Index( 
      void ) const
{
   return( this->vpr_index_ );
}

//===========================================================================//
inline const t_type_descriptor* TCH_PrePlacedBlock_c::GetVPR_Type( 
      void ) const
{
   return( this->vpr_type_ );
} 

//===========================================================================//
inline bool TCH_PrePlacedBlock_c::IsValid( 
      void ) const
{
   return( this->srName_.length( ) ? true : false );
}

#endif
