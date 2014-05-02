//===========================================================================//
// Purpose : Method definitions for the TOS_PlaceOptions class.
//
//           Public methods include:
//           - TOS_PlaceOptions_c, ~TOS_PlaceOptions_c
//           - Print
//
//===========================================================================//

//---------------------------------------------------------------------------//
// Copyright (C) 2012-2013 Jeff Rudolph, Texas Instruments (jrudolph@ti.com) //
//                                                                           //
// This program is free software; you can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; version 3 of the License, or any later version. //
//                                                                           //
// This program is distributed in the hope that it will be useful, but       //
// WITHOUT ANY WARRANTY; without even an implied warranty of MERCHANTABILITY //
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License   //
// for more details.                                                         //
//                                                                           //
// You should have received a copy of the GNU General Public License along   //
// with this program; if not, see <http://www.gnu.org/licenses>.             //
//---------------------------------------------------------------------------//

#include "TC_MinGrid.h"

#include "TIO_PrintHandler.h"

#include "TOS_StringUtils.h"
#include "TOS_PlaceOptions.h"

//===========================================================================//
// Method         : TOS_PlaceOptions_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
// 01/15/13 jeffr : Added support for relativePlace members
// 01/18/13 jeffr : Added support for prePlaced members
//===========================================================================//
TOS_PlaceOptions_c::TOS_PlaceOptions_c( 
      void )
      :
      algorithmMode( TOS_PLACE_ALGORITHM_UNDEFINED ),
      channelWidth( 0 ),
      randomSeed( 0 ),
      initTemp( 0.0 ),
      initTempFactor( 0.0 ),
      initTempEpsilon( 0.0 ),
      exitTemp( 0.0 ),
      exitTempFactor( 0.0 ),
      exitTempEpsilon( 0.0 ),
      reduceTemp( 0.0 ),
      reduceTempFactor( 0.0 ),
      innerNum( 0.0 ),
      searchLimit( 0.0 ),
      costMode( TOS_PLACE_COST_UNDEFINED ),
      fixedDelay( 0.0 ),
      timingCostFactor( 0.0 ),
      timingUpdateInt( 0 ),
      timingUpdateCount( 0 ),
      slackInitWeight( 0.0 ),
      slackFinalWeight( 0.0 )
{
   this->relativePlace.enable = false;
   this->relativePlace.rotateEnable = false;
   this->relativePlace.maxPlaceRetryCt = 0;
   this->relativePlace.maxMacroRetryCt = 0;

   this->prePlaced.enable = false;
}

//===========================================================================//
TOS_PlaceOptions_c::TOS_PlaceOptions_c( 
      TOS_PlaceAlgorithmMode_t algorithmMode_,
      unsigned int             channelWidth_,
      unsigned int             randomSeed_,
      double                   initTemp_,
      double                   initTempFactor_,
      double                   initTempEpsilon_,
      double                   exitTemp_,
      double                   exitTempFactor_,
      double                   exitTempEpsilon_,
      double                   reduceTemp_,
      double                   reduceTempFactor_,
      double                   innerNum_,
      double                   searchLimit_,
      TOS_PlaceCostMode_t      costMode_,
      double                   timingCostFactor_,
      unsigned int             timingUpdateInt_,
      unsigned int             timingUpdateCount_,
      double                   slackInitWeight_,
      double                   slackFinalWeight_,
      bool                     relativePlace_enable_,
      bool                     relativePlace_rotateEnable_,
      unsigned int             relativePlace_maxPlaceRetryCt_,
      unsigned int             relativePlace_maxMacroRetryCt_,
      bool                     prePlaced_enable_ )
      :
      algorithmMode( algorithmMode_ ),
      channelWidth( channelWidth_ ), 
      randomSeed( randomSeed_ ),
      initTemp( initTemp_ ),
      initTempFactor( initTempFactor_ ),
      initTempEpsilon( initTempEpsilon_ ),
      exitTemp( exitTemp_ ),
      exitTempFactor( exitTempFactor_ ),
      exitTempEpsilon( exitTempEpsilon_ ),
      reduceTemp( reduceTemp_ ),
      reduceTempFactor( reduceTempFactor_ ),
      innerNum( innerNum_ ),
      searchLimit( searchLimit_ ),
      costMode( costMode_ ),
      fixedDelay( 0.0 ),
      timingCostFactor( timingCostFactor_ ),
      timingUpdateInt( timingUpdateInt_ ),
      timingUpdateCount( timingUpdateCount_ ),
      slackInitWeight( slackInitWeight_ ),
      slackFinalWeight( slackFinalWeight_ )
{
   this->relativePlace.enable = relativePlace_enable_;
   this->relativePlace.rotateEnable = relativePlace_rotateEnable_;
   this->relativePlace.maxPlaceRetryCt = relativePlace_maxPlaceRetryCt_;
   this->relativePlace.maxMacroRetryCt = relativePlace_maxMacroRetryCt_;

   this->prePlaced.enable = prePlaced_enable_;
}

//===========================================================================//
// Method         : ~TOS_PlaceOptions_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
TOS_PlaceOptions_c::~TOS_PlaceOptions_c( 
      void )
{
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
// 01/15/13 jeffr : Added support for relativePlace members
// 01/18/13 jeffr : Added support for prePlaced members
//===========================================================================//
void TOS_PlaceOptions_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   string srAlgorithmMode, srCostMode;
   TOS_ExtractStringPlaceAlgorithmMode( this->algorithmMode, &srAlgorithmMode );
   TOS_ExtractStringPlaceCostMode( this->costMode, &srCostMode );

   printHandler.Write( pfile, spaceLen, "PLACE_ALGORITHM            = %s\n", TIO_SR_STR( srAlgorithmMode ));

   printHandler.Write( pfile, spaceLen, "PLACE_CHANNEL_WIDTH        = %u\n", this->channelWidth );

   printHandler.Write( pfile, spaceLen, "PLACE_RANDOM_SEED          = %u\n", this->randomSeed );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_INIT            = %0.*f\n", precision, this->initTemp );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_INIT_FACTOR     = %0.*f\n", precision, this->initTempFactor );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_INIT_EPSILON    = %0.*f\n", precision, this->initTempEpsilon );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_EXIT            = %0.*f\n", precision, this->exitTemp );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_EXIT_FACTOR     = %0.*f\n", precision, this->exitTempFactor );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_EXIT_EPSILON    = %0.*f\n", precision, this->exitTempEpsilon );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_REDUCE          = %0.*f\n", precision, this->reduceTemp );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_REDUCE_FACTOR   = %0.*f\n", precision, this->reduceTempFactor );
   printHandler.Write( pfile, spaceLen, "PLACE_TEMP_INNER_NUM       = %0.*f\n", precision, this->innerNum );
   printHandler.Write( pfile, spaceLen, "PLACE_SEARCH_LIMIT         = %0.*f\n", precision, this->searchLimit );

   printHandler.Write( pfile, spaceLen, "PLACE_COST_MODE            = %s\n", TIO_SR_STR( srCostMode ));

   printHandler.Write( pfile, spaceLen, "PLACE_TIMING_COST_FACTOR   = %0.*f\n", precision, this->timingCostFactor );
   printHandler.Write( pfile, spaceLen, "PLACE_TIMING_UPDATE_INT    = %u\n", this->timingUpdateInt );
   printHandler.Write( pfile, spaceLen, "PLACE_TIMING_UPDATE_COUNT  = %u\n", this->timingUpdateCount );
   printHandler.Write( pfile, spaceLen, "PLACE_SLACK_INIT_WEIGHT    = %0.*f\n", precision, this->slackInitWeight );
   printHandler.Write( pfile, spaceLen, "PLACE_SLACK_FINAL_WEIGHT   = %0.*f\n", precision, this->slackFinalWeight );

   printHandler.Write( pfile, spaceLen, "\n" );
   printHandler.Write( pfile, spaceLen, "PLACE_RELATIVE_ENABLE      = %s\n", TIO_BOOL_STR( this->relativePlace.enable ));
   printHandler.Write( pfile, spaceLen, "PLACE_RELATIVE_ROTATE      = %s\n", TIO_BOOL_STR( this->relativePlace.rotateEnable ));
   printHandler.Write( pfile, spaceLen, "PLACE_RELATIVE_INIT_PLACE  = %u\n", this->relativePlace.maxPlaceRetryCt );
   printHandler.Write( pfile, spaceLen, "PLACE_RELATIVE_INIT_MACRO  = %u\n", this->relativePlace.maxMacroRetryCt );

   printHandler.Write( pfile, spaceLen, "PLACE_PREPLACED_ENABLE     = %s\n", TIO_BOOL_STR( this->prePlaced.enable ));
}
