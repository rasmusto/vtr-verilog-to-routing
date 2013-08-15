//===========================================================================//
// Purpose : Enums, typedefs, and defines for TAS_ArchitectureSpec classes.
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

#ifndef TAS_TYPEDEFS_H
#define TAS_TYPEDEFS_H

#include "TCT_OrderedQueue.h"
#include "TCT_OrderedVector.h"
#include "TCT_OrderedVector2D.h"
#include "TCT_SortedNameDynamicVector.h"

#include "TC_Name.h"
#include "TCT_NameList.h"

//---------------------------------------------------------------------------//
// Define cell constants and typedefs
//---------------------------------------------------------------------------//

class TAS_Cell_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_Cell_c > TAS_CellList_t;

enum TAS_ClassType_e
{
   TAS_CLASS_UNDEFINED = 0,
   TAS_CLASS_LUT,
   TAS_CLASS_FLIPFLOP,
   TAS_CLASS_MEMORY,
   TAS_CLASS_SUBCKT
};
typedef enum TAS_ClassType_e TAS_ClassType_t;

//---------------------------------------------------------------------------//
// Define physical block constants and typedefs
//---------------------------------------------------------------------------//

class TAS_PhysicalBlock_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_PhysicalBlock_c > TAS_PhysicalBlockList_t;
typedef TCT_SortedNameDynamicVector_c< TAS_PhysicalBlock_c > TAS_PhysicalBlockSortedList_t;

class TAS_Mode_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_Mode_c > TAS_ModeList_t;
typedef TCT_SortedNameDynamicVector_c< TAS_Mode_c > TAS_ModeSortedList_t;

class TAS_Interconnect_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_Interconnect_c > TAS_InterconnectList_t;

class TAS_ConnectionFc_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_ConnectionFc_c > TAS_ConnectionFcList_t;

class TAS_PinAssign_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_PinAssign_c > TAS_PinAssignList_t;

class TAS_GridAssign_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_GridAssign_c > TAS_GridAssignList_t;

class TAS_TimingDelay_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_TimingDelay_c > TAS_TimingDelayList_t;

typedef TCT_OrderedQueue_c< double > TAS_TimingValueList_t;
typedef TCT_OrderedVector_c< TAS_TimingValueList_t > TAS_TimingValueTable_t;

typedef TCT_OrderedVector2D_c< double > TAS_TimingValueMatrix_t;

enum TAS_PhysicalBlockModelType_e
{
   TAS_PHYSICAL_BLOCK_MODEL_UNDEFINED = 0,
   TAS_PHYSICAL_BLOCK_MODEL_STANDARD,
   TAS_PHYSICAL_BLOCK_MODEL_CUSTOM
};
typedef enum TAS_PhysicalBlockModelType_e TAS_PhysicalBlockModelType_t;

enum TAS_InterconnectMapType_e
{
   TAS_INTERCONNECT_MAP_UNDEFINED = 0,
   TAS_INTERCONNECT_MAP_COMPLETE,
   TAS_INTERCONNECT_MAP_DIRECT,
   TAS_INTERCONNECT_MAP_MUX
};
typedef TAS_InterconnectMapType_e TAS_InterconnectMapType_t;

enum TAS_ConnectionBoxType_e
{
   TAS_CONNECTION_BOX_UNDEFINED = 0,
   TAS_CONNECTION_BOX_FRACTION,
   TAS_CONNECTION_BOX_ABSOLUTE,
   TAS_CONNECTION_BOX_FULL
};
typedef TAS_ConnectionBoxType_e TAS_ConnectionBoxType_t;

enum TAS_PinAssignPatternType_e
{
   TAS_PIN_ASSIGN_PATTERN_UNDEFINED = 0,
   TAS_PIN_ASSIGN_PATTERN_SPREAD,
   TAS_PIN_ASSIGN_PATTERN_CUSTOM
};
typedef TAS_PinAssignPatternType_e TAS_PinAssignPatternType_t;

enum TAS_GridAssignDistrMode_e
{
   TAS_GRID_ASSIGN_DISTR_UNDEFINED = 0,
   TAS_GRID_ASSIGN_DISTR_SINGLE,
   TAS_GRID_ASSIGN_DISTR_MULTIPLE,
   TAS_GRID_ASSIGN_DISTR_FILL,
   TAS_GRID_ASSIGN_DISTR_PERIMETER
};
typedef TAS_GridAssignDistrMode_e TAS_GridAssignDistrMode_t;

enum TAS_GridAssignOrientMode_e
{
   TAS_GRID_ASSIGN_ORIENT_UNDEFINED = 0,
   TAS_GRID_ASSIGN_ORIENT_COLUMN,
   TAS_GRID_ASSIGN_ORIENT_ROW
};
typedef TAS_GridAssignOrientMode_e TAS_GridAssignOrientMode_t;

enum TAS_TimingMode_e
{
   TAS_TIMING_MODE_UNDEFINED = 0,
   TAS_TIMING_MODE_DELAY_CONSTANT,
   TAS_TIMING_MODE_DELAY_MATRIX,
   TAS_TIMING_MODE_T_SETUP,
   TAS_TIMING_MODE_T_HOLD,
   TAS_TIMING_MODE_CLOCK_TO_Q,
   TAS_TIMING_MODE_CAP_CONSTANT,
   TAS_TIMING_MODE_CAP_MATRIX,
   TAS_TIMING_MODE_PACK_PATTERN
};
typedef TAS_TimingMode_e TAS_TimingMode_t;

enum TAS_TimingType_e
{
   TAS_TIMING_TYPE_UNDEFINED = 0,
   TAS_TIMING_TYPE_MIN_VALUE,
   TAS_TIMING_TYPE_MAX_VALUE,
   TAS_TIMING_TYPE_MIN_MATRIX,
   TAS_TIMING_TYPE_MAX_MATRIX
};
typedef TAS_TimingType_e TAS_TimingType_t;

typedef TCT_NameList_c< TC_Name_c > TAS_InputNameList_t;
typedef TCT_NameList_c< TC_Name_c > TAS_OutputNameList_t;
typedef TCT_NameList_c< TC_Name_c > TAS_PortNameList_t;

typedef TCT_NameList_c< TC_Name_c > TAS_ModeNameList_t;
typedef TCT_NameList_c< TC_Name_c > TAS_SlotNameList_t;

enum TAS_UsageMode_e
{
   TAS_USAGE_UNDEFINED = 0,
   TAS_USAGE_PHYSICAL_BLOCK,
   TAS_USAGE_INPUT_OUTPUT
};
typedef enum TAS_UsageMode_e TAS_UsageMode_t;

//---------------------------------------------------------------------------//
// Define input/output constants and typedefs
//---------------------------------------------------------------------------//

typedef TAS_PhysicalBlock_c TAS_InputOutput_t;
typedef TCT_OrderedVector_c< TAS_InputOutput_t > TAS_InputOutputList_t;
typedef TCT_SortedNameDynamicVector_c< TAS_InputOutput_t > TAS_InputOutputSortedList_t;

//---------------------------------------------------------------------------//
// Define switch box constants and typedefs
//---------------------------------------------------------------------------//

class TAS_SwitchBox_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_SwitchBox_c > TAS_SwitchBoxList_t;
typedef TCT_SortedNameDynamicVector_c< TAS_SwitchBox_c > TAS_SwitchBoxSortedList_t;

class TC_SideIndex_c; // Forward declaration for subsequent class typedefs
typedef TC_SideIndex_c TAS_SideIndex_t;
typedef TCT_OrderedVector_c< TAS_SideIndex_t > TAS_MapList_t;
typedef TCT_OrderedVector_c< TAS_MapList_t > TAS_MapTable_t;

//---------------------------------------------------------------------------//
// Define segment constants and typedefs
//---------------------------------------------------------------------------//

class TAS_Segment_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_Segment_c > TAS_SegmentList_t;

enum TAS_SegmentDirType_e
{
   TAS_SEGMENT_DIR_UNDEFINED = 0,
   TAS_SEGMENT_DIR_UNIDIRECTIONAL,
   TAS_SEGMENT_DIR_BIDIRECTIONAL
};
typedef enum TAS_SegmentDirType_e TAS_SegmentDirType_t;

class TC_Bit_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedQueue_c< TC_Bit_c > TAS_BitPattern_t;

//---------------------------------------------------------------------------//
// Define carry chain constants and typedefs
//---------------------------------------------------------------------------//

class TAS_CarryChain_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_CarryChain_c > TAS_CarryChainList_t;

//---------------------------------------------------------------------------//
// Define power constants and typedefs
//---------------------------------------------------------------------------//

enum TAS_PowerMethodMode_e
{
   TAS_POWER_METHOD_UNDEFINED = 0,
   TAS_POWER_METHOD_IGNORE,
   TAS_POWER_METHOD_SUM_OF_CHILDREN,
   TAS_POWER_METHOD_AUTO_SIZES,
   TAS_POWER_METHOD_SPECIFY_SIZES,
   TAS_POWER_METHOD_PIN_TOGGLE,
   TAS_POWER_METHOD_CAP_INTERNAL,
   TAS_POWER_METHOD_ABSOLUTE
};
typedef TAS_PowerMethodMode_e TAS_PowerMethodMode_t;

//---------------------------------------------------------------------------//
// Define clock constants and typedefs
//---------------------------------------------------------------------------//

class TAS_Clock_c; // Forward declaration for subsequent class typedefs
typedef TCT_OrderedVector_c< TAS_Clock_c > TAS_ClockList_t;

//---------------------------------------------------------------------------//
// Define config constants and typedefs
//---------------------------------------------------------------------------//

enum TAS_ArraySizeMode_e
{
   TAS_ARRAY_SIZE_UNDEFINED = 0,
   TAS_ARRAY_SIZE_AUTO,
   TAS_ARRAY_SIZE_MANUAL
};
typedef enum TAS_ArraySizeMode_e TAS_ArraySizeMode_t;

enum TAS_SwitchBoxType_e
{
   TAS_SWITCH_BOX_UNDEFINED = 0,
   TAS_SWITCH_BOX_BUFFERED,
   TAS_SWITCH_BOX_MUX
};
typedef enum TAS_SwitchBoxType_e TAS_SwitchBoxType_t;

enum TAS_SwitchBoxModelType_e
{
   TAS_SWITCH_BOX_MODEL_UNDEFINED = 0,
   TAS_SWITCH_BOX_MODEL_WILTON,
   TAS_SWITCH_BOX_MODEL_SUBSET,
   TAS_SWITCH_BOX_MODEL_UNIVERSAL,
   TAS_SWITCH_BOX_MODEL_CUSTOM
};
typedef enum TAS_SwitchBoxModelType_e TAS_SwitchBoxModelType_t;

enum TAS_ChannelUsageMode_e
{
  TAS_CHANNEL_USAGE_UNDEFINED = 0,
  TAS_CHANNEL_USAGE_IO,
  TAS_CHANNEL_USAGE_X,
  TAS_CHANNEL_USAGE_Y
};
typedef enum TAS_ChannelUsageMode_e TAS_ChannelUsageMode_t;

enum TAS_ChannelDistrMode_e
{
   TAS_CHANNEL_DISTR_UNDEFINED = 0,
   TAS_CHANNEL_DISTR_UNIFORM,
   TAS_CHANNEL_DISTR_GAUSSIAN,
   TAS_CHANNEL_DISTR_PULSE,
   TAS_CHANNEL_DISTR_DELTA
};
typedef enum TAS_ChannelDistrMode_e TAS_ChannelDistrMode_t;

#endif
