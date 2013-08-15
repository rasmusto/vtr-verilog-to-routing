//===========================================================================//
// Purpose : Method definitions for the TPO_Inst class.
//
//           Public methods include:
//           - TPO_Inst_c, ~TPO_Inst_c
//           - operator=, operator<
//           - operator==, operator!=
//           - Print
//           - PrintBLIF
//           - SetInputOutputType
//           - SetNamesLogicBitsList
//           - SetLatchClockType, SetLatchInitState
//           - SetSubcktPinMapList
//           - SetPackInstHierMapList
//           - SetPlaceFabricName
//           - SetPlaceStatus, SetPlaceOrigin
//           - SetPlaceRegionList, SetPlaceRelativeList
//           - AddPin
//           - FindPin
//           - FindPinCount
//           - Clear
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

#include "TC_StringUtils.h"

#include "TIO_PrintHandler.h"

#include "TPO_StringUtils.h"
#include "TPO_Inst.h"

//===========================================================================//
// Method         : TPO_Inst_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      void )
{
   this->Clear( );
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const string& srName )
{
   this->Clear( );

   this->srName_ = srName;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const char* pszName )
{
   this->Clear( );

   this->srName_ = TIO_PSZ_STR( pszName );
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const string&              srName,
      const string&              srCellName,
      const TPO_PinList_t&       pinList,
            TPO_InstSource_t     source,
      const TPO_LogicBitsList_t& logicBitsList )
{
   this->Clear( );

   this->srName_ = srName;
   this->srCellName_ = srCellName;
   this->pinList_ = pinList;
   this->source_ = source;

   this->names_.logicBitsList = logicBitsList;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const char*                pszName,
      const char*                pszCellName,
      const TPO_PinList_t&       pinList,
            TPO_InstSource_t     source,
      const TPO_LogicBitsList_t& logicBitsList )
{
   this->Clear( );

   this->srName_ = TIO_PSZ_STR( pszName );
   this->srCellName_ = TIO_PSZ_STR( pszCellName );
   this->pinList_ = pinList;
   this->source_ = source;

   this->names_.logicBitsList = logicBitsList;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const string&          srName,
      const string&          srCellName,
      const TPO_PinList_t&   pinList,
            TPO_InstSource_t source,
            TPO_LatchType_t  clockType,
            TPO_LatchState_t initState )
{
   this->Clear( );

   this->srName_ = srName;
   this->srCellName_ = srCellName;
   this->pinList_ = pinList;
   this->source_ = source;

   this->latch_.clockType = clockType;
   this->latch_.initState = initState;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const char*            pszName,
      const char*            pszCellName,
      const TPO_PinList_t&   pinList,
            TPO_InstSource_t source,
            TPO_LatchType_t  clockType,
            TPO_LatchState_t initState )
{
   this->Clear( );

   this->srName_ = TIO_PSZ_STR( pszName );
   this->srCellName_ = TIO_PSZ_STR( pszCellName );
   this->pinList_ = pinList;
   this->source_ = source;

   this->latch_.clockType = clockType;
   this->latch_.initState = initState;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const string&           srName,
      const string&           srCellName,
            TPO_InstSource_t  source,
      const TPO_PinMapList_t& pinMapList )
{
   this->Clear( );

   this->srName_ = srName;
   this->srCellName_ = srCellName;
   this->source_ = source;

   this->subckt_.pinMapList = pinMapList;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const char*             pszName,
      const char*             pszCellName,
            TPO_InstSource_t  source,
      const TPO_PinMapList_t& pinMapList )
{
   this->Clear( );

   this->srName_ = TIO_PSZ_STR( pszName );
   this->srCellName_ = TIO_PSZ_STR( pszCellName );
   this->source_ = source;

   this->subckt_.pinMapList = pinMapList;
} 

//===========================================================================//
TPO_Inst_c::TPO_Inst_c( 
      const TPO_Inst_c& inst )
{
   this->Clear( );

   this->srName_ = inst.srName_;
   this->srCellName_ = inst.srCellName_;
   this->pinList_ = inst.pinList_;
   this->source_ = inst.source_;

   this->inputOutput_.type = inst.inputOutput_.type;

   this->names_.logicBitsList = inst.names_.logicBitsList;

   this->latch_.clockType = inst.latch_.clockType;
   this->latch_.initState = inst.latch_.initState;

   this->subckt_.pinMapList = inst.subckt_.pinMapList;

   this->pack_.instHierMapList = inst.pack_.instHierMapList;

   this->place_.srFabricName = inst.place_.srFabricName;
   this->place_.status = inst.place_.status;
   this->place_.origin = inst.place_.origin;

   this->place_.relativeList = inst.place_.relativeList;
   this->place_.regionList = inst.place_.regionList;
} 

//===========================================================================//
// Method         : ~TPO_Inst_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_Inst_c::~TPO_Inst_c( 
      void )
{
}

//===========================================================================//
// Method         : operator=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TPO_Inst_c& TPO_Inst_c::operator=( 
      const TPO_Inst_c& inst )
{
   if( &inst != this )
   {
      this->srName_ = inst.srName_;
      this->srCellName_ = inst.srCellName_;
      this->pinList_ = inst.pinList_;
      this->source_ = inst.source_;
      this->inputOutput_.type = inst.inputOutput_.type;
      this->names_.logicBitsList = inst.names_.logicBitsList;
      this->latch_.clockType = inst.latch_.clockType;
      this->latch_.initState = inst.latch_.initState;
      this->subckt_.pinMapList = inst.subckt_.pinMapList;
      this->pack_.instHierMapList = inst.pack_.instHierMapList;
      this->place_.srFabricName = inst.place_.srFabricName;
      this->place_.status = inst.place_.status;
      this->place_.origin = inst.place_.origin;
      this->place_.relativeList = inst.place_.relativeList;
      this->place_.regionList = inst.place_.regionList;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator<
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TPO_Inst_c::operator<( 
      const TPO_Inst_c& inst ) const
{
   return(( TC_CompareStrings( this->srName_, inst.srName_ ) < 0 ) ? 
          true : false );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TPO_Inst_c::operator==( 
      const TPO_Inst_c& inst ) const
{
   return(( this->srName_ == inst.srName_ ) ? true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TPO_Inst_c::operator!=( 
      const TPO_Inst_c& inst ) const
{
   return( !this->operator==( inst ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::Print( 
            FILE*  pfile,
            size_t spaceLen,
      const char*  pszUsage ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   pszUsage = ( pszUsage && *pszUsage ? pszUsage : "inst" );

   printHandler.Write( pfile, spaceLen, "<%s", 
                                        TIO_PSZ_STR( pszUsage ));

   printHandler.Write( pfile, 0, " name=\"%s\"", 
                                 TIO_SR_STR( this->srName_ ));
   if( this->srCellName_.length( ))
   {
      printHandler.Write( pfile, 0, " master=\"%s\"", 
                                    TIO_SR_STR( this->srCellName_ ));
   }
   else if( this->source_ != TPO_INST_SOURCE_UNDEFINED )
   {
      string srSource;
      TPO_ExtractStringInstSource( this->source_, &srSource );
      printHandler.Write( pfile, 0, " source=\".%s\"", 
                                    TIO_SR_STR( srSource ));
   }

   if( this->place_.status != TPO_STATUS_UNDEFINED )
   {
      string srStatus;
      TPO_ExtractStringStatusMode( this->place_.status, &srStatus );
      printHandler.Write( pfile, 0, " status=\"%s\"", 
                                    TIO_SR_STR( srStatus ));
   }
   printHandler.Write( pfile, 0, ">\n" );
   spaceLen += 3;

   if( this->source_ == TPO_INST_SOURCE_LATCH )
   {
      printHandler.Write( pfile, spaceLen, "<clock" ); 
      if( this->latch_.clockType != TPO_LATCH_TYPE_UNDEFINED )
      {
         string srClockType;
         TPO_ExtractStringLatchType( this->latch_.clockType, &srClockType );
         printHandler.Write( pfile, 0, " type=\"%s\"", 
                                       TIO_SR_STR( srClockType ));
      }
      if( this->latch_.initState != TPO_LATCH_STATE_UNDEFINED )
      {
         string srInitState;
         TPO_ExtractStringLatchState( this->latch_.initState, &srInitState );
         printHandler.Write( pfile, 0, " state=\"%s\"", 
                                       TIO_SR_STR( srInitState ));
      }
      printHandler.Write( pfile, 0, "/>\n" );
   }

   if( this->pinList_.IsValid( ))
   {
      for( size_t i = 0; i < this->pinList_.GetLength( ); ++i )
      {
         const TPO_Pin_t& pin = *this->pinList_[i];
         printHandler.Write( pfile, spaceLen, "<pin name=\"%s\"", 
                                              TIO_PSZ_STR( pin.GetName( )));
         if( pin.GetType( ) != TC_TYPE_UNDEFINED )
         {
            string srType;
            TC_ExtractStringTypeMode( this->pinList_[i]->GetType( ), &srType );
            printHandler.Write( pfile, 0, " type=\"%s\"",
                                          TIO_SR_STR( srType ));
         }
         printHandler.Write( pfile, 0, "/>\n" );
      }
   }

   if( this->pack_.instHierMapList.IsValid( ))
   {
      this->pack_.instHierMapList.Print( pfile, spaceLen );
   }

   if( this->place_.srFabricName.length( ) || 
       this->place_.origin.IsValid( ))
   {
      printHandler.Write( pfile, spaceLen, "<placement" );

      if( this->place_.srFabricName.length( ))
      {
         printHandler.Write( pfile, 0, " name=\"%s\"", 
                                       TIO_SR_STR( this->place_.srFabricName ));
      }
      if( this->place_.origin.IsValid( ))
      {
         const TGO_Point_c& origin = this->place_.origin;
         printHandler.Write( pfile, 0, " x=\"%d\" y=\"%d\" z=\"%d\"", 
                                       origin.x, origin.y, origin.z );
      }
      printHandler.Write( pfile, 0, "/>\n" );
   }

   for( size_t i = 0; i < this->place_.relativeList.GetLength( ); ++i )
   {
      const TPO_PlaceRelative_c& relative = *this->place_.relativeList[i];

      printHandler.Write( pfile, spaceLen, "<relative name=\"%s\"", 
                                           TIO_PSZ_STR( relative.GetName( )));
      if( relative.GetSide( ) != TC_SIDE_UNDEFINED )
      {
         string srSide;
         TC_ExtractStringSideMode( this->place_.relativeList[i]->GetSide( ), &srSide );
         printHandler.Write( pfile, 0, " side=\"%s\"",
                                       TIO_SR_STR( srSide ));
      }
      if(( relative.GetDx( ) != INT_MAX ) && 
         ( relative.GetDy( ) != INT_MAX ))
      {
         printHandler.Write( pfile, 0, " dx=\"%d\" dy=\"%d\"",
                                       relative.GetDx( ),
                                       relative.GetDy( ));
      }
      if( relative.GetRotateEnable( ))
      {
         printHandler.Write( pfile, 0, " rotate=\"true\"" );
      }
      printHandler.Write( pfile, 0, "/>\n" );
   }

   for( size_t i = 0; i < this->place_.regionList.GetLength( ); ++i )
   {
      string srRegion;
      this->place_.regionList[i]->ExtractString( &srRegion );
      printHandler.Write( pfile, spaceLen, "<region> %s </region>\n", 
                                           TIO_SR_STR( srRegion ));
   }

   spaceLen -= 3;
   printHandler.Write( pfile, spaceLen, "</%s>\n",
                                        TIO_PSZ_STR( pszUsage ));
}

//===========================================================================//
// Method         : PrintBLIF
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::PrintBLIF( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   string srInputPin, srOutputPin, srClockPin;

   switch( this->source_ )
   {
   case TPO_INST_SOURCE_NAMES:

      printHandler.Write( pfile, spaceLen, ".names" );
      for( size_t i = 0; i < this->pinList_.GetLength( ); ++i )
      {
         const TPO_Pin_t& pin = *this->pinList_[i];
         printHandler.Write( pfile, spaceLen, " %s",
                                              TIO_PSZ_STR( pin.GetName( )));

      }
      printHandler.Write( pfile, 0, "\n" );

      for( size_t i = 0; i < this->names_.logicBitsList.GetLength( ); ++i )
      {
         string srLogicBits;
         const TPO_LogicBits_t& logicBits = *this->names_.logicBitsList[i];
         for( size_t j = 0; j < logicBits.GetLength( ); ++j )
         {
            string srLogicBit;
            logicBits[j]->ExtractString( &srLogicBit );

            srLogicBits += srLogicBit;
            srLogicBits += ( j + 2 == logicBits.GetLength( ) ? " " : "" );
         }
         printHandler.Write( pfile, spaceLen, "%s\n",
                                              TIO_SR_STR( srLogicBits ));
      }
      break;

   case TPO_INST_SOURCE_LATCH:

      srInputPin = ( this->pinList_[0] ? this->pinList_[0]->GetName( ) : "" );
      srOutputPin = ( this->pinList_[1] ? this->pinList_[1]->GetName( ) : "" );
      srClockPin = ( this->pinList_[2] ? this->pinList_[2]->GetName( ) : "" );

      printHandler.Write( pfile, spaceLen, ".latch %s %s",
                                           TIO_SR_STR( srInputPin ),
                                           TIO_SR_STR( srOutputPin ));
      if( this->latch_.clockType != TPO_LATCH_TYPE_UNDEFINED )
      {
         string srClockType;
         TPO_ExtractStringLatchType( this->latch_.clockType, &srClockType );
         printHandler.Write( pfile, spaceLen, " %s %s",
                                              TIO_SR_STR( srClockType ),
                                               TIO_SR_STR( srClockPin ));
      }
      if( this->latch_.initState != TPO_LATCH_STATE_UNDEFINED )
      {
         string srInitState;
         TPO_ExtractStringLatchState( this->latch_.initState, &srInitState );
         printHandler.Write( pfile, spaceLen, " %s",
                                              TIO_SR_STR( srInitState ));
      }
      printHandler.Write( pfile, 0, "\n" );
      break;

   case TPO_INST_SOURCE_SUBCKT:

      printHandler.Write( pfile, spaceLen, ".subckt %s",
                                           TIO_SR_STR( this->srCellName_ ));

      for( size_t i = 0; i < this->subckt_.pinMapList.GetLength( ); ++i )
      {
         const TPO_PinMap_c& pinMap = *this->subckt_.pinMapList[i];
         printHandler.Write( pfile, 0, " %s=%s",
                                       TIO_PSZ_STR( pinMap.GetCellPinName( )),
                                       TIO_PSZ_STR( pinMap.GetInstPinName( )));
      }
      printHandler.Write( pfile, 0, "\n" );
      break;

   default:

      printHandler.Write( pfile, spaceLen, "// " );
      this->Print( pfile, spaceLen );
      break;
   }
}

//===========================================================================//
// Method         : SetInputOutputType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetInputOutputType( 
      TC_TypeMode_t type )
{
   switch( type )
   {
   case TC_TYPE_INPUT:  this->source_ = TPO_INST_SOURCE_OUTPUT;    break;
   case TC_TYPE_OUTPUT: this->source_ = TPO_INST_SOURCE_INPUT;     break;
   default:             this->source_ = TPO_INST_SOURCE_UNDEFINED; break;
   }
   this->inputOutput_.type = type;
}

//===========================================================================//
// Method         : SetNamesLogicBitsList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetNamesLogicBitsList( 
      const TPO_LogicBitsList_t& logicBitsList )
{
   this->source_ = TPO_INST_SOURCE_NAMES;
   this->names_.logicBitsList = logicBitsList;
}

//===========================================================================//
// Method         : SetLatchClockType
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetLatchClockType( 
      TPO_LatchType_t clockType )
{
   this->source_ = TPO_INST_SOURCE_LATCH;
   this->latch_.clockType = clockType;
}

//===========================================================================//
// Method         : SetLatchInstState
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetLatchInitState( 
      TPO_LatchState_t initState )
{
   this->source_ = TPO_INST_SOURCE_LATCH;
   this->latch_.initState = initState;
}

//===========================================================================//
// Method         : SetSubcktPinMapList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetSubcktPinMapList( 
      const TPO_PinMapList_t& pinMapList )
{
   this->source_ = TPO_INST_SOURCE_SUBCKT;
   this->subckt_.pinMapList = pinMapList;
}

//===========================================================================//
// Method         : SetPackInstHierMapList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetPackInstHierMapList(
      const TPO_InstHierMapList_t& instHierMapList )
{
   this->pack_.instHierMapList = instHierMapList;
}

//===========================================================================//
// Method         : SetPlaceFabricName
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetPlaceFabricName( 
      const string& srFabricName )
{
   this->place_.srFabricName = srFabricName;
}

//===========================================================================//
void TPO_Inst_c::SetPlaceFabricName( 
      const char* pszFabricName )
{
   this->place_.srFabricName = TIO_PSZ_STR( pszFabricName );
}

//===========================================================================//
// Method         : SetPlaceStatus
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetPlaceStatus( 
      TPO_StatusMode_t status )
{
   this->place_.status = status;
}

//===========================================================================//
// Method         : SetPlaceOrigin
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetPlaceOrigin( 
      const TGO_Point_c& origin )
{
   this->place_.origin = origin;
}

//===========================================================================//
// Method         : SetPlaceRegionList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetPlaceRegionList(
      const TGS_RegionList_t& regionList )
{
   this->place_.regionList = regionList;
}

//===========================================================================//
// Method         : SetPlaceRelativeList
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::SetPlaceRelativeList(
      const TPO_PlaceRelativeList_t& relativeList )
{
   this->place_.relativeList = relativeList;
}

//===========================================================================//
// Method         : AddPin
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::AddPin(
      const TPO_Pin_t& pin )
{
   this->pinList_.Add( pin );
}

//===========================================================================//
// Method         : FindPin
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
const TPO_Pin_t* TPO_Inst_c::FindPin(
      TC_TypeMode_t type ) const
{
   const TPO_Pin_t* ppin = 0;

   for( size_t i = 0; i < this->pinList_.GetLength( ); ++i )
   {
      const TPO_Pin_t& pin = *this->pinList_[i];
      if( pin.GetType( ) == type )
      {
         ppin = this->pinList_[i];
         break;
      }
   }
   return( ppin );
}

//===========================================================================//
// Method         : FindPinCount
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
size_t TPO_Inst_c::FindPinCount(
            TC_TypeMode_t   type,
      const TLO_CellList_t& cellList ) const
{
   size_t pinCount = 0;

   if( cellList.IsMember( this->srCellName_ ))
   {
      const TLO_Cell_c& cell = *cellList.Find( this->srCellName_ );
      pinCount = cell.FindPortCount( type );
   }
   return( pinCount );
}

//===========================================================================//
// Method         : Clear
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TPO_Inst_c::Clear( 
      void )
{
   this->srName_ = "";
   this->srCellName_ = "";

   this->pinList_.Clear( );
   this->pinList_.SetCapacity( TPO_PIN_LIST_DEF_CAPACITY );

   this->source_ = TPO_INST_SOURCE_UNDEFINED;

   this->inputOutput_.type = TC_TYPE_UNDEFINED;

   this->names_.logicBitsList.Clear( );
   this->names_.logicBitsList.SetCapacity( TPO_LOGIC_BITS_LIST_DEF_CAPACITY );

   this->latch_.clockType = TPO_LATCH_TYPE_UNDEFINED;
   this->latch_.initState = TPO_LATCH_STATE_UNDEFINED;

   this->subckt_.pinMapList.Clear( );
   this->subckt_.pinMapList.SetCapacity( TPO_PIN_MAP_LIST_DEF_CAPACITY );

   this->pack_.instHierMapList.Clear( );
   this->pack_.instHierMapList.SetCapacity( TPO_INST_HIER_MAP_LIST_DEF_CAPACITY );

   this->place_.srFabricName = "";
   this->place_.status = TPO_STATUS_UNDEFINED;
   this->place_.origin.Reset( );

   this->place_.regionList.Clear( );
   this->place_.regionList.SetCapacity( TPO_REGION_LIST_DEF_CAPACITY );
   this->place_.relativeList.Clear( );
   this->place_.relativeList.SetCapacity( TPO_RELATIVE_LIST_DEF_CAPACITY );
}
