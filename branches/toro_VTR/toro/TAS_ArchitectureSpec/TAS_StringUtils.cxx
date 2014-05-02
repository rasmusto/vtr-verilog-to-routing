//===========================================================================//
// Purpose : Function definitions for miscellaneous helpful string functions.
//
//           Functions include:
//           - TAS_ExtractStringClassType
//           - TAS_ExtractStringPhysicalBlockModelType
//           - TAS_ExtractStringInterconnectMapType
//           - TAS_ExtractStringConnectionBoxType
//           - TAS_ExtractStringPinAssignPatternType
//           - TAS_ExtractStringGridAssignDistrMode
//           - TAS_ExtractStringGridAssignOrientMode
//           - TAS_ExtractStringTimingMode
//           - TAS_ExtractStringTimingType
//           - TAS_ExtractStringArraySizeMode
//           - TAS_ExtractStringSwitchBoxType
//           - TAS_ExtractStringSwitchBoxModelType
//           - TAS_ExtractStringSegmentDirType
//           - TAS_ExtractStringChannelUsageMode
//           - TAS_ExtractStringChannelDistrMode
//           - TAS_ExtractStringPowerMethodMode
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

#include "TAS_StringUtils.h"

//===========================================================================//
// Function       : TAS_ExtractStringClassType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringClassType(
      TAS_ClassType_t type,
      string*         psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_CLASS_LUT:      *psrType = "lut";      break;
      case TAS_CLASS_FLIPFLOP: *psrType = "flipflop"; break;
      case TAS_CLASS_MEMORY:   *psrType = "memory";   break;
      case TAS_CLASS_SUBCKT:   *psrType = "subckt";   break;
      default:                 *psrType = "?";        break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringPhysicalBlockModelType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringPhysicalBlockModelType(
      TAS_PhysicalBlockModelType_t type,
      string*                      psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_PHYSICAL_BLOCK_MODEL_STANDARD: *psrType = "standard"; break;
      case TAS_PHYSICAL_BLOCK_MODEL_CUSTOM:   *psrType = "custom";   break;
      default:                                *psrType = "?";        break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringInterconnectMapType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringInterconnectMapType(
      TAS_InterconnectMapType_t type,
      string*                   psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_INTERCONNECT_MAP_COMPLETE: *psrType = "complete"; break;
      case TAS_INTERCONNECT_MAP_DIRECT:   *psrType = "direct";   break;
      case TAS_INTERCONNECT_MAP_MUX:      *psrType = "mux";      break;
      default:                            *psrType = "?";        break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringConnectionBoxType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringConnectionBoxType(
      TAS_ConnectionBoxType_t type,
      string*                 psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_CONNECTION_BOX_FRACTION: *psrType = "frac"; break;
      case TAS_CONNECTION_BOX_ABSOLUTE: *psrType = "abs";  break;
      case TAS_CONNECTION_BOX_FULL:     *psrType = "full"; break;
      default:                          *psrType = "?";    break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringPinAssignPatternType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringPinAssignPatternType(
      TAS_PinAssignPatternType_t type,
      string*                    psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_PIN_ASSIGN_PATTERN_SPREAD: *psrType = "spread"; break;
      case TAS_PIN_ASSIGN_PATTERN_CUSTOM: *psrType = "custom"; break;
      default:                            *psrType = "?";      break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringGridAssignDistrMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringGridAssignDistrMode(
      TAS_GridAssignDistrMode_t mode,
      string*                   psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_GRID_ASSIGN_DISTR_SINGLE:    *psrMode = "rel";       break;
      case TAS_GRID_ASSIGN_DISTR_MULTIPLE:  *psrMode = "col";       break;
      case TAS_GRID_ASSIGN_DISTR_FILL:      *psrMode = "fill";      break;
      case TAS_GRID_ASSIGN_DISTR_PERIMETER: *psrMode = "perimeter"; break;
      default:                              *psrMode = "?";         break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringGridAssignOrientMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringGridAssignOrientMode(
      TAS_GridAssignOrientMode_t mode,
      string*                    psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_GRID_ASSIGN_ORIENT_COLUMN: *psrMode = "column"; break;
      case TAS_GRID_ASSIGN_ORIENT_ROW:    *psrMode = "row";    break;
      default:                            *psrMode = "?";      break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringTimingMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringTimingMode(
      TAS_TimingMode_t mode,
      string*          psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_TIMING_MODE_DELAY_CONSTANT: *psrMode = "delay";        break;
      case TAS_TIMING_MODE_DELAY_MATRIX:   *psrMode = "delay_matrix"; break;
      case TAS_TIMING_MODE_T_SETUP:        *psrMode = "t_setup";      break;
      case TAS_TIMING_MODE_T_HOLD:         *psrMode = "t_hold";       break;
      case TAS_TIMING_MODE_CLOCK_TO_Q:     *psrMode = "clock_to_q";   break;
      case TAS_TIMING_MODE_CAP_CONSTANT:   *psrMode = "cap";          break;
      case TAS_TIMING_MODE_CAP_MATRIX:     *psrMode = "cap_matrix";   break;
      case TAS_TIMING_MODE_PACK_PATTERN:   *psrMode = "pack_pattern"; break;
      default:                             *psrMode = "?";            break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringTimingType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringTimingType(
      TAS_TimingType_t type,
      string*          psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_TIMING_TYPE_MIN_VALUE:  *psrType = "min_value";  break;
      case TAS_TIMING_TYPE_MAX_VALUE:  *psrType = "max_value";  break;
      case TAS_TIMING_TYPE_MIN_MATRIX: *psrType = "min_matrix"; break;
      case TAS_TIMING_TYPE_MAX_MATRIX: *psrType = "max_matrix"; break;
      default:                         *psrType = "?";          break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringArraySizeMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringArraySizeMode(
      TAS_ArraySizeMode_t mode,
      string*             psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_ARRAY_SIZE_AUTO:   *psrMode = "auto";  break;
      case TAS_ARRAY_SIZE_MANUAL: *psrMode = "fixed"; break;
      default:                    *psrMode = "?";     break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringSwitchBoxType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringSwitchBoxType(
      TAS_SwitchBoxType_t type,
      string*             psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_SWITCH_BOX_BUFFERED: *psrType = "buffered"; break;
      case TAS_SWITCH_BOX_MUX:      *psrType = "mux";      break;
      default:                      *psrType = "?";        break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringSwitchBoxModelType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringSwitchBoxModelType(
      TAS_SwitchBoxModelType_t type,
      string*                  psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_SWITCH_BOX_MODEL_WILTON:    *psrType = "wilton";    break;
      case TAS_SWITCH_BOX_MODEL_SUBSET:    *psrType = "subset";    break;
      case TAS_SWITCH_BOX_MODEL_UNIVERSAL: *psrType = "universal"; break;
      case TAS_SWITCH_BOX_MODEL_CUSTOM:    *psrType = "custom";    break;
      default:                             *psrType = "?";         break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringSegmentDirType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringSegmentDirType(
      TAS_SegmentDirType_t type,
      string*              psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TAS_SEGMENT_DIR_UNIDIRECTIONAL: *psrType = "unidir"; break;
      case TAS_SEGMENT_DIR_BIDIRECTIONAL:  *psrType = "bidir";  break;
      default:                             *psrType = "?";      break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringChannelUsageMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 07/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringChannelUsageMode(
      TAS_ChannelUsageMode_t mode,
      string*                psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_CHANNEL_USAGE_IO: *psrMode = "io"; break;
      case TAS_CHANNEL_USAGE_X:  *psrMode = "x";  break;
      case TAS_CHANNEL_USAGE_Y:  *psrMode = "y";  break;
      default:                   *psrMode = "?";  break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringChannelDistrMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 07/15/12 jeffr : Original
//===========================================================================//
void TAS_ExtractStringChannelDistrMode(
      TAS_ChannelDistrMode_t mode,
      string*                psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_CHANNEL_DISTR_UNIFORM:  *psrMode = "uniform";  break;
      case TAS_CHANNEL_DISTR_GAUSSIAN: *psrMode = "gaussian"; break;
      case TAS_CHANNEL_DISTR_PULSE:    *psrMode = "pulse";    break;
      case TAS_CHANNEL_DISTR_DELTA:    *psrMode = "delta";    break;
      default:                         *psrMode = "?";        break;
      }
   }
}

//===========================================================================//
// Function       : TAS_ExtractStringPowerMethodMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 07/17/13 jeffr : Original
//===========================================================================//
void TAS_ExtractStringPowerMethodMode(
      TAS_PowerMethodMode_t mode,
      string*               psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TAS_POWER_METHOD_IGNORE:          *psrMode = "ignore";          break;
      case TAS_POWER_METHOD_SUM_OF_CHILDREN: *psrMode = "sum-of-children"; break;
      case TAS_POWER_METHOD_AUTO_SIZES:      *psrMode = "auto-sizes";      break;
      case TAS_POWER_METHOD_SPECIFY_SIZES:   *psrMode = "specify-sizes";   break;
      case TAS_POWER_METHOD_PIN_TOGGLE:      *psrMode = "pin-toggle";      break;
      case TAS_POWER_METHOD_CAP_INTERNAL:    *psrMode = "cap-internal";    break;
      case TAS_POWER_METHOD_ABSOLUTE:        *psrMode = "absolute";        break;
      default:                               *psrMode = "?";               break;
      }
   }
}
