//===========================================================================//
// Purpose : Enums, typedefs, and defines for TPO_PhysicalObjects classes.
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

#ifndef TPO_TYPEDEFS_H
#define TPO_TYPEDEFS_H

#include "TCT_OrderedQueue.h"
#include "TCT_OrderedVector.h"
#include "TCT_SortedVector.h"
#include "TCT_SortedNameDynamicVector.h"

#include "TCT_NameList.h"

//---------------------------------------------------------------------------//
// Define instance constants and typedefs
//---------------------------------------------------------------------------//

class TPO_Inst_c; // Forward declaration for subsequent class typedefs
typedef TCT_SortedNameDynamicVector_c< TPO_Inst_c > TPO_InstList_t;

typedef TPO_Inst_c TPO_Port_t; 
typedef TCT_SortedNameDynamicVector_c< TPO_Port_t > TPO_PortList_t;

class TC_Name_c; // Forward declaration for subsequent class typedefs
typedef TCT_NameList_c< TC_Name_c > TPO_NameList_t;

enum TPO_InstSource_e
{
   TPO_INST_SOURCE_UNDEFINED = 0,
   TPO_INST_SOURCE_INPUT,
   TPO_INST_SOURCE_OUTPUT,
   TPO_INST_SOURCE_NAMES,
   TPO_INST_SOURCE_LATCH,
   TPO_INST_SOURCE_SUBCKT
};
typedef enum TPO_InstSource_e TPO_InstSource_t;

enum TPO_LatchType_e
{
   TPO_LATCH_TYPE_UNDEFINED = 0,
   TPO_LATCH_TYPE_FALLING_EDGE, // corresponds to BLIF type 'FE'
   TPO_LATCH_TYPE_RISING_EDGE,  // corresponds to BLIF type 'RE'
   TPO_LATCH_TYPE_ACTIVE_HIGH,  // corresponds to BLIF type 'AH'
   TPO_LATCH_TYPE_ACTIVE_LOW,   // corresponds to BLIF type 'AL'
   TPO_LATCH_TYPE_ASYNCHRONOUS  // corresponds to BLIF type 'AS'
};
typedef enum TPO_LatchType_e TPO_LatchType_t;

enum TPO_LatchState_e
{
   TPO_LATCH_STATE_UNDEFINED = 0,
   TPO_LATCH_STATE_TRUE,        // corresponds to BLIF init-val '1'
   TPO_LATCH_STATE_FALSE,       // corresponds to BLIF init-val '0'
   TPO_LATCH_STATE_DONT_CARE,   // corresponds to BLIF init-val '2'
   TPO_LATCH_STATE_UNKNOWN      // corresponds to BLIF init-val '3'
};
typedef enum TPO_LatchState_e TPO_LatchState_t;

class TC_Bit_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedQueue_c< TC_Bit_c > TPO_LogicBits_t;
typedef TCT_OrderedVector_c< TPO_LogicBits_t > TPO_LogicBitsList_t;

enum TPO_StatusMode_e
{
   TPO_STATUS_UNDEFINED = 0,
   TPO_STATUS_FLOAT,
   TPO_STATUS_FIXED,
   TPO_STATUS_PLACED
};
typedef enum TPO_StatusMode_e TPO_StatusMode_t;

class TPO_HierInstMap_c; // Forward declaration for subsequent class typedefs
typedef TCT_SortedVector_c< TPO_HierInstMap_c > TPO_HierInstMapList_t;

class TPO_InstHierMap_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TPO_InstHierMap_c > TPO_InstHierMapList_t;

class TPO_PlaceRelative_c; // Forward declaration for subsequent class typedefs
typedef TCT_SortedNameDynamicVector_c< TPO_PlaceRelative_c > TPO_PlaceRelativeList_t;

class TPO_PlaceRegions_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TPO_PlaceRegions_c > TPO_PlaceRegionsList_t;

#define TPO_INST_INDEX_INVALID SIZE_MAX

//---------------------------------------------------------------------------//
// Define pin constants and typedefs
//---------------------------------------------------------------------------//

class TC_NameType_c; // Forward declaration for subsequent class typedefs
typedef TC_NameType_c TPO_Pin_t;
typedef TCT_SortedNameDynamicVector_c< TPO_Pin_t > TPO_PinList_t;

//---------------------------------------------------------------------------//
// Define instance pin constants and typedefs
//---------------------------------------------------------------------------//

class TPO_PinMap_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TPO_PinMap_c > TPO_PinMapList_t;

#endif
