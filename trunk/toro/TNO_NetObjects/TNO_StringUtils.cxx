//===========================================================================//
// Purpose : Function definitions for miscellaneous helpful string functions.
//
//           Functions include:
//           - TNO_ExtractStringStatusMode
//           - TNO_ExtractStringNodeType
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

#include "TNO_StringUtils.h"

//===========================================================================//
// Function       : TNO_ExtractStringStatusMode
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/30/12 jeffr : Original
//===========================================================================//
void TNO_ExtractStringStatusMode(
      TNO_StatusMode_t mode,
      string*          psrMode )
{
   if( psrMode )
   {
      *psrMode = "";

      switch( mode )
      {
      case TNO_STATUS_OPEN:    *psrMode = "open";    break;
      case TNO_STATUS_FIXED:   *psrMode = "fixed";   break;
      case TNO_STATUS_GROUTED: *psrMode = "grouted"; break;
      case TNO_STATUS_ROUTED:  *psrMode = "routed";  break;
      default:                 *psrMode = "*";       break;
      }
   }
} 

//===========================================================================//
// Function       : TNO_ExtractStringNodeType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/30/12 jeffr : Original
//===========================================================================//
void TNO_ExtractStringNodeType(
      TNO_NodeType_t type,
      string*         psrType )
{
   if( psrType )
   {
      *psrType = "";

      switch( type )
      {
      case TNO_NODE_INST_PIN:   *psrType = "instpin";   break;
      case TNO_NODE_SEGMENT:    *psrType = "segment";   break;
      case TNO_NODE_SWITCH_BOX: *psrType = "switchbox"; break;
      default:                  *psrType = "*";         break;
      }
   }
} 
