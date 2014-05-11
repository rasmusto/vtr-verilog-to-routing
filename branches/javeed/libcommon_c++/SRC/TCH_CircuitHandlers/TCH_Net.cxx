//===========================================================================//
// Purpose : Method definitions for the TCH_Net class.
//
//           Public methods include:
//           - TCH_Net_c, ~TCH_Net_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - Reset
//           - ResetRoutePathList
//           - ResetRoutePathNodeList
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
#include "TCH_Net.h"

//===========================================================================//
// Method         : TCH_Net_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
TCH_Net_c::TCH_Net_c( 
      void )
      :
      srName_( "" ),
      status_( TCH_ROUTE_STATUS_UNDEFINED ),
      routePathList_( TCH_ROUTE_PATH_LIST_DEF_CAPACITY ),
      isLegal_( false )
{
   this->vpr_.netIndex = -1;
} 

//===========================================================================//
TCH_Net_c::TCH_Net_c( 
      const string&               srName,
            TCH_RouteStatusMode_t status,
            int                   vpr_netIndex )
      :
      srName_( srName ),
      status_( status ),
      routePathList_( TCH_ROUTE_PATH_LIST_DEF_CAPACITY ),
      isLegal_( false )
{
   this->vpr_.netIndex = vpr_netIndex;
} 

//===========================================================================//
TCH_Net_c::TCH_Net_c( 
      const char*                 pszName,
            TCH_RouteStatusMode_t status,
            int                   vpr_netIndex )
      :
      srName_( TIO_PSZ_STR( pszName )),
      status_( status ),
      routePathList_( TCH_ROUTE_PATH_LIST_DEF_CAPACITY ),
      isLegal_( false )
{
   this->vpr_.netIndex = vpr_netIndex;
} 

//===========================================================================//
TCH_Net_c::TCH_Net_c( 
      const TCH_Net_c& net )
      :
      srName_( net.srName_ ),
      status_( net.status_ ),
      routePathList_( net.routePathList_ ),
      isLegal_( net.isLegal_ )
{
   this->vpr_.netIndex = net.vpr_.netIndex;
} 

//===========================================================================//
// Method         : ~TCH_Net_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
TCH_Net_c::~TCH_Net_c( 
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
TCH_Net_c& TCH_Net_c::operator=( 
      const TCH_Net_c& net )
{
   if( &net != this )
   {
      this->srName_ = net.srName_;
      this->status_ = net.status_;
      this->vpr_.netIndex = net.vpr_.netIndex;
      this->routePathList_ = net.routePathList_;
      this->isLegal_ = net.isLegal_;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
bool TCH_Net_c::operator==( 
      const TCH_Net_c& net ) const
{
   return(( this->srName_ == net.srName_ ) &&
          ( this->status_ == net.status_ ) &&
          ( this->vpr_.netIndex == net.vpr_.netIndex ) &&
          ( this->routePathList_ == net.routePathList_ ) &&
          ( this->isLegal_ == net.isLegal_ ) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
bool TCH_Net_c::operator!=( 
      const TCH_Net_c& net ) const
{
   return( !this->operator==( net ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
void TCH_Net_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<net" );
   printHandler.Write( pfile, 0, " isLegal=%s", TIO_BOOL_VAL( this->isLegal_ ));
   printHandler.Write( pfile, 0, " name=\"%s\"", TIO_SR_STR( this->srName_ ));
   if( this->status_ != TCH_ROUTE_STATUS_UNDEFINED )
   {
      string srStatus;
      TCH_ExtractStringStatusMode( this->status_, &srStatus );
      printHandler.Write( pfile, 0, " status=%s", TIO_SR_STR( srStatus ));
   }
   printHandler.Write( pfile, 0, " <vpr" );
   printHandler.Write( pfile, 0, " netIndex=%d", this->vpr_.netIndex );
   printHandler.Write( pfile, 0, ">" );
   printHandler.Write( pfile, 0, " routePathList=[%d]", this->routePathList_.GetLength( ));
   printHandler.Write( pfile, 0, ">\n" );

   this->routePathList_.Print( pfile, spaceLen + 3 );
} 

//===========================================================================//
// Method         : Reset
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
void TCH_Net_c::Reset(
      void )
{
   this->SetLegal( false );

   this->ResetRoutePathList( );
   this->ResetRoutePathNodeList( );
}

//===========================================================================//
// Method         : ResetRoutePathList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
void TCH_Net_c::ResetRoutePathList(
      void )
{
   for( size_t i = 0; i < this->routePathList_.GetLength( ); ++i )
   {
      TCH_RoutePath_c* proutePath = this->routePathList_[i];
      proutePath->SetLegal( false );
   }
   this->ResetRoutePathNodeList( );
}

//===========================================================================//
// Method         : ResetRoutePathNodeList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 02/21/13 jeffr : Original
//===========================================================================//
void TCH_Net_c::ResetRoutePathNodeList(
      void )
{
   for( size_t i = 0; i < this->routePathList_.GetLength( ); ++i )
   {
      TCH_RoutePath_c* proutePath = this->routePathList_[i];
      const TCH_RouteNodeList_t* prouteNodeList = proutePath->GetRouteNodeList( );
      for( size_t j = 0; j < prouteNodeList->GetLength( ); ++j )
      {
         TCH_RouteNode_c* prouteNode = (*prouteNodeList)[j];
         prouteNode->SetLegal( false );
      }
   }
}
