//===========================================================================//
// Purpose : Declaration and inline definitions for a TAS_TimingDelayLists 
//           class.
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

#ifndef TAS_TIMING_DELAY_LISTS_H
#define TAS_TIMING_DELAY_LISTS_H

#include <cstdio>
using namespace std;

#include "TAS_TimingDelay.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
class TAS_TimingDelayLists_c
{
public:

   TAS_TimingDelayLists_c( void );
   TAS_TimingDelayLists_c( const TAS_TimingDelayLists_c& timingDelayLists );
   ~TAS_TimingDelayLists_c( void );

   TAS_TimingDelayLists_c& operator=( const TAS_TimingDelayLists_c& timingDelayLists );
   bool operator==( const TAS_TimingDelayLists_c& timingDelayLists ) const;
   bool operator!=( const TAS_TimingDelayLists_c& timingDelayLists ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void PrintXML( void ) const;
   void PrintXML( FILE* pfile, size_t spaceLen ) const;

   bool IsValid( void ) const;

public:

   TAS_TimingDelayList_t delayList; 
                             // Defines common min/max delays from all pins
                             // asso. with the given input port to all pins
                             // asso. with the given output port
   TAS_TimingDelayList_t delayMatrixList;    
                             // Defines unique min/max delays from each pin 
                             // asso. with the given input port to each pin 
                             // asso. with the given output port
   TAS_TimingDelayList_t tSetupList;     
                             // Defines clock setup time for all pins 
                             // asso. with the given port
                             // Applies to sequential blocks (see flipflops)
   TAS_TimingDelayList_t tHoldList;     
                             // Defines clock hold time for all pins 
                             // asso. with the given port
                             // Applies to sequential blocks (see flipflops)
   TAS_TimingDelayList_t clockToQList;
                             // Defines clock-to-Q delay for all pins 
                             // asso. with the given output port 
                             // Applies to sequential blocks (see flipflops)
   TAS_TimingDelayList_t capList; 
                             // Defines common min/max caps from all pins
                             // asso. with the given input port to all pins
                             // asso. with the given output port
   TAS_TimingDelayList_t capMatrixList;    
                             // Defines unique min/max caps from each pin 
                             // asso. with the given input port to each pin 
                             // asso. with the given output port
   TAS_TimingDelayList_t packPatternList; 
                             // Defines pack pattern (for molecule support)

private:

   enum TAS_DefCapacity_e 
   { 
      TAS_DELAY_LIST_DEF_CAPACITY = 1,
      TAS_DELAY_MATRIX_LIST_DEF_CAPACITY = 1,
      TAS_T_SETUP_LIST_DEF_CAPACITY = 1,
      TAS_T_HOLD_LIST_DEF_CAPACITY = 1,
      TAS_CLOCK_TO_Q_LIST_DEF_CAPACITY = 1,
      TAS_CAP_LIST_DEF_CAPACITY = 1,
      TAS_CAP_MATRIX_LIST_DEF_CAPACITY = 1,
      TAS_PACK_PATTERN_LIST_DEF_CAPACITY = 1
   };
};

#endif
