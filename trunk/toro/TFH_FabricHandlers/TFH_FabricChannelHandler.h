//===========================================================================//
// Purpose : Declaration and inline definitions for the universal 
//           TFH_FabricChannelHandler singleton class. This class supports
//           channel width overrides for the VPR placement tool
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

#ifndef TFH_FABRIC_CHANNEL_HANDLER_H
#define TFH_FABRIC_CHANNEL_HANDLER_H

#include <cstdio>
using namespace std;

#include "TC_IndexCount.h"

#include "TFH_Typedefs.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 01/15/13 jeffr : Original
//===========================================================================//
class TFH_FabricChannelHandler_c
{
public:

   static void NewInstance( void );
   static void DeleteInstance( void );
   static TFH_FabricChannelHandler_c& GetInstance( bool newInstance = true );
   static bool HasInstance( void );

   size_t GetLength( TFH_SelectChannelMode_t selectChannel ) const;
   const TFH_ChannelWidthList_t* GetChannelWidthList( TFH_SelectChannelMode_t selectChannel ) const;

   const TFH_ChannelWidth_t* At( TFH_SelectChannelMode_t selectChannel,
                                 size_t i ) const;

   void Clear( TFH_SelectChannelMode_t selectChannel = TFH_SELECT_CHANNEL_UNDEFINED );

   void Add( TFH_SelectChannelMode_t selectChannel,
             int index, 
             size_t count );
   void Add( TFH_SelectChannelMode_t selectChannel,
             const TFH_ChannelWidth_t& channelWidth );

   bool IsValid( void ) const;

protected:

   TFH_FabricChannelHandler_c( void );
   ~TFH_FabricChannelHandler_c( void );

private:

   TFH_ChannelWidthList_t xChannelWidthList_;
   TFH_ChannelWidthList_t yChannelWidthList_;

   static TFH_FabricChannelHandler_c* pinstance_; // Define ptr for singleton instance

private:

   enum TFH_DefCapacity_e 
   { 
      TFH_CHANNEL_WIDTH_LIST_DEF_CAPACITY = 1
   };
};

#endif
