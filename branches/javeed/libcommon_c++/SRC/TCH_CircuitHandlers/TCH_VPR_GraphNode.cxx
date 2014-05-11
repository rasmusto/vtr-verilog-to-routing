//===========================================================================//
// Purpose : Method definitions for the TCH_VPR_GraphNode class.
//
//           Public methods include:
//           - TCH_VPR_GraphNode_c, ~TCH_VPR_GraphNode_c
//           - operator=, operator<
//           - operator==, operator!=
//           - Print
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

#include "TIO_PrintHandler.h"

#include "TCH_StringUtils.h"
#include "TCH_VPR_GraphNode.h"

//===========================================================================//
// Method         : TCH_VPR_GraphNode_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
TCH_VPR_GraphNode_c::TCH_VPR_GraphNode_c( 
      void )
{
   this->vpr_.type = static_cast< t_rr_type >( -1 );
   this->vpr_.ptcNum = -1;
   this->vpr_.rrNodeIndex = -1;
} 

//===========================================================================//
TCH_VPR_GraphNode_c::TCH_VPR_GraphNode_c( 
            t_rr_type    vpr_type,
            int          vpr_ptcNum,
      const TGO_Point_c& vpr_gridPoint,
            int          vpr_rrNodeIndex )
{
   this->vpr_.type = vpr_type;
   this->vpr_.ptcNum = vpr_ptcNum;
   this->vpr_.gridPoint = vpr_gridPoint;
   this->vpr_.rrNodeIndex = vpr_rrNodeIndex;
} 

//===========================================================================//
TCH_VPR_GraphNode_c::TCH_VPR_GraphNode_c( 
      const TCH_VPR_GraphNode_c& vpr_graphNode )
{
   this->vpr_.type = vpr_graphNode.vpr_.type;
   this->vpr_.ptcNum = vpr_graphNode.vpr_.ptcNum;
   this->vpr_.gridPoint = vpr_graphNode.vpr_.gridPoint;
   this->vpr_.rrNodeIndex = vpr_graphNode.vpr_.rrNodeIndex;
} 

//===========================================================================//
// Method         : ~TCH_VPR_GraphNode_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
TCH_VPR_GraphNode_c::~TCH_VPR_GraphNode_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
TCH_VPR_GraphNode_c& TCH_VPR_GraphNode_c::operator=( 
      const TCH_VPR_GraphNode_c& vpr_graphNode )
{
   if( &vpr_graphNode != this )
   {
      this->vpr_.type = vpr_graphNode.vpr_.type;
      this->vpr_.ptcNum = vpr_graphNode.vpr_.ptcNum;
      this->vpr_.gridPoint = vpr_graphNode.vpr_.gridPoint;
      this->vpr_.rrNodeIndex = vpr_graphNode.vpr_.rrNodeIndex;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator<
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
bool TCH_VPR_GraphNode_c::operator<( 
      const TCH_VPR_GraphNode_c& vpr_graphNode ) const
{
   bool isLessThan = false;
   
   if( this->vpr_.type < vpr_graphNode.vpr_.type )
   {
      isLessThan = true;
   }
   else if( this->vpr_.type == vpr_graphNode.vpr_.type )
   {
      if( this->vpr_.gridPoint < vpr_graphNode.vpr_.gridPoint )
      {
         isLessThan = true;
      }
      else if( this->vpr_.gridPoint == vpr_graphNode.vpr_.gridPoint )
      {
         if( this->vpr_.ptcNum< vpr_graphNode.vpr_.ptcNum )
         {
            isLessThan = true;
         }
      }
   }
   return( isLessThan );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
bool TCH_VPR_GraphNode_c::operator==( 
      const TCH_VPR_GraphNode_c& vpr_graphNode ) const
{
   return(( this->vpr_.type == vpr_graphNode.vpr_.type ) &&
          ( this->vpr_.ptcNum == vpr_graphNode.vpr_.ptcNum ) &&
          ( this->vpr_.gridPoint == vpr_graphNode.vpr_.gridPoint ) &&
          ( this->vpr_.rrNodeIndex == vpr_graphNode.vpr_.rrNodeIndex ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
bool TCH_VPR_GraphNode_c::operator!=( 
      const TCH_VPR_GraphNode_c& vpr_graphNode ) const
{
   return( !this->operator==( vpr_graphNode ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
void TCH_VPR_GraphNode_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   string srVPR_Type;
   TCH_ExtractStringVPR_Type( this->vpr_.type, &srVPR_Type );

   string srGridPoint;
   this->vpr_.gridPoint.ExtractString( &srGridPoint );

   printHandler.Write( pfile, spaceLen, "<vpr_graphNode" );
   printHandler.Write( pfile, 0, " <vpr" );
   printHandler.Write( pfile, 0, " type=%s", TIO_SR_STR( srVPR_Type ));
   printHandler.Write( pfile, 0, " ptcNum=%d", this->vpr_.ptcNum );
   printHandler.Write( pfile, 0, " gridPoint=(%s)", TIO_SR_STR( srGridPoint ));
   printHandler.Write( pfile, 0, " rrNodeIndex=%d", this->vpr_.rrNodeIndex );
   printHandler.Write( pfile, 0, ">" );
   printHandler.Write( pfile, 0, ">\n" );
} 
