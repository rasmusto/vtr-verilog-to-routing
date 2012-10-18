//===========================================================================//
// Purpose : Method definitions for the TAS_Config class.
//
//           Public methods include:
//           - TAS_Config_c, ~TAS_Config_c
//           - operator=
//           - operator==, operator!=
//           - Print
//           - PrintXML
//
//===========================================================================//

#include "TC_MinGrid.h"
#include "TCT_Generic.h"

#include "TIO_PrintHandler.h"

#include "TAS_StringUtils.h"
#include "TAS_Config.h"

//===========================================================================//
// Method         : TAS_Config_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TAS_Config_c::TAS_Config_c( 
      void )
{
   this->layout.sizeMode = TAS_ARRAY_SIZE_UNDEFINED;
   this->layout.autoSize.aspectRatio = 0.0;

   this->device.switchBoxes.modelType = TAS_SWITCH_BOX_MODEL_UNDEFINED;
   this->device.switchBoxes.fs = 0;

   this->device.connectionBoxes.capInput = 0.0;
   this->device.connectionBoxes.delayInput = 0.0;

   this->device.segments.dirType = TAS_SEGMENT_DIR_UNDEFINED;

   this->device.areaModel.resMinWidthNMOS = 0.0;
   this->device.areaModel.resMinWidthPMOS = 0.0;
   this->device.areaModel.sizeInputPinMux = 0.0;
   this->device.areaModel.areaGridTile = 0.0;
} 

//===========================================================================//
TAS_Config_c::TAS_Config_c( 
      const TAS_Config_c& config )
{
   this->layout.sizeMode = config.layout.sizeMode;
   this->layout.autoSize.aspectRatio = config.layout.autoSize.aspectRatio;
   this->layout.manualSize.gridDims = config.layout.manualSize.gridDims;

   this->device.switchBoxes.modelType = config.device.switchBoxes.modelType;
   this->device.switchBoxes.fs = config.device.switchBoxes.fs;

   this->device.connectionBoxes.capInput = config.device.connectionBoxes.capInput;
   this->device.connectionBoxes.delayInput = config.device.connectionBoxes.delayInput;

   this->device.segments.dirType = config.device.segments.dirType;

   this->device.channelWidth.io = config.device.channelWidth.io;
   this->device.channelWidth.x = config.device.channelWidth.x;
   this->device.channelWidth.y = config.device.channelWidth.y;

   this->device.areaModel.resMinWidthNMOS = config.device.areaModel.resMinWidthNMOS;
   this->device.areaModel.resMinWidthPMOS = config.device.areaModel.resMinWidthPMOS;
   this->device.areaModel.sizeInputPinMux = config.device.areaModel.sizeInputPinMux;
   this->device.areaModel.areaGridTile = config.device.areaModel.areaGridTile;
}

//===========================================================================//
// Method         : ~TAS_Config_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
TAS_Config_c::~TAS_Config_c( 
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
TAS_Config_c& TAS_Config_c::operator=( 
      const TAS_Config_c& config )
{
   if( &config != this )
   {
      this->layout.sizeMode = config.layout.sizeMode;
      this->layout.autoSize.aspectRatio = config.layout.autoSize.aspectRatio;
      this->layout.manualSize.gridDims = config.layout.manualSize.gridDims;
      this->device.switchBoxes.modelType = config.device.switchBoxes.modelType;
      this->device.switchBoxes.fs = config.device.switchBoxes.fs;
      this->device.connectionBoxes.capInput = config.device.connectionBoxes.capInput;
      this->device.connectionBoxes.delayInput = config.device.connectionBoxes.delayInput;
      this->device.segments.dirType = config.device.segments.dirType;
      this->device.channelWidth.io = config.device.channelWidth.io;
      this->device.channelWidth.x = config.device.channelWidth.x;
      this->device.channelWidth.y = config.device.channelWidth.y;
      this->device.areaModel.resMinWidthNMOS = config.device.areaModel.resMinWidthNMOS;
      this->device.areaModel.resMinWidthPMOS = config.device.areaModel.resMinWidthPMOS;
      this->device.areaModel.sizeInputPinMux = config.device.areaModel.sizeInputPinMux;
      this->device.areaModel.areaGridTile = config.device.areaModel.areaGridTile;
   }
   return( *this );
}

//===========================================================================//
// Method         : operator==
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TAS_Config_c::operator==( 
      const TAS_Config_c& config ) const
{
   return(( this->layout.sizeMode == config.layout.sizeMode ) &&
          ( TCTF_IsEQ( this->layout.autoSize.aspectRatio, config.layout.autoSize.aspectRatio )) &&
          ( this->layout.manualSize.gridDims == config.layout.manualSize.gridDims ) &&
          ( this->device.switchBoxes.modelType == config.device.switchBoxes.modelType ) &&
          ( this->device.switchBoxes.fs == config.device.switchBoxes.fs ) &&
          ( TCTF_IsEQ( this->device.connectionBoxes.capInput, config.device.connectionBoxes.capInput )) &&
          ( TCTF_IsEQ( this->device.connectionBoxes.delayInput, config.device.connectionBoxes.delayInput )) &&
          ( this->device.segments.dirType == config.device.segments.dirType ) &&
          ( this->device.channelWidth.io == config.device.channelWidth.io ) &&
          ( this->device.channelWidth.x == config.device.channelWidth.x ) &&
          ( this->device.channelWidth.y == config.device.channelWidth.y ) &&
          ( TCTF_IsEQ( this->device.areaModel.resMinWidthNMOS, config.device.areaModel.resMinWidthNMOS )) &&
          ( TCTF_IsEQ( this->device.areaModel.resMinWidthPMOS, config.device.areaModel.resMinWidthPMOS )) &&
          ( TCTF_IsEQ( this->device.areaModel.sizeInputPinMux, config.device.areaModel.sizeInputPinMux )) &&
          ( TCTF_IsEQ( this->device.areaModel.areaGridTile, config.device.areaModel.areaGridTile )) ?
          true : false );
}

//===========================================================================//
// Method         : operator!=
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
bool TAS_Config_c::operator!=( 
      const TAS_Config_c& config ) const
{
   return( !this->operator==( config ) ? true : false );
}

//===========================================================================//
// Method         : Print
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_Config_c::Print( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   printHandler.Write( pfile, spaceLen, "<config>\n" );
   spaceLen += 3;

   if( this->layout.sizeMode != TAS_ARRAY_SIZE_UNDEFINED )
   {
      string srArraySizeMode;
      TAS_ExtractStringArraySizeMode( this->layout.sizeMode, &srArraySizeMode );

      printHandler.Write( pfile, spaceLen, "<size %s ", TIO_SR_STR( srArraySizeMode ));

      if( TCTF_IsGT( this->layout.autoSize.aspectRatio, 0.0 ))
      {
         printHandler.Write( pfile, 0, "ratio = %0.*f ", precision, this->layout.autoSize.aspectRatio );
      }
      if( this->layout.manualSize.gridDims.IsValid( ))
      {
         printHandler.Write( pfile, 0, "width = %u height = %u ", this->layout.manualSize.gridDims.width,
                                                                  this->layout.manualSize.gridDims.height );
      }
      printHandler.Write( pfile, 0, "/>\n" );
   }

   if( TCTF_IsGT( this->device.areaModel.resMinWidthNMOS, 0.0 ) ||
       TCTF_IsGT( this->device.areaModel.resMinWidthPMOS, 0.0 ) ||
       TCTF_IsGT( this->device.areaModel.sizeInputPinMux, 0.0 ) ||
       TCTF_IsGT( this->device.areaModel.areaGridTile, 0.0 ))
   {
      printHandler.Write( pfile, spaceLen, "<est " );

      if( TCTF_IsGT( this->device.areaModel.resMinWidthNMOS, 0.0 ))
      {
         printHandler.Write( pfile, 0, "min_width_nmos_res = %0.*f ", 
                                       precision, this->device.areaModel.resMinWidthNMOS );
      }
      if( TCTF_IsGT( this->device.areaModel.resMinWidthPMOS, 0.0 ))
      {
         printHandler.Write( pfile, 0, "min_width_pmos_res = %0.*f ", 
                                       precision, this->device.areaModel.resMinWidthPMOS );
      }
      if( TCTF_IsGT( this->device.areaModel.sizeInputPinMux, 0.0 ))
      {
         printHandler.Write( pfile, 0, "mux_trans_in_pin_size = %0.*f ", 
                                       precision, this->device.areaModel.sizeInputPinMux );
      }
      if( TCTF_IsGT( this->device.areaModel.areaGridTile, 0.0 ))
      {
         printHandler.Write( pfile, 0, "grid_logic_tile_area = %0.*f ", 
                                       precision, this->device.areaModel.areaGridTile );
      }
      printHandler.Write( pfile, 0, "/>\n" );
   }

   if( this->device.switchBoxes.modelType != TAS_SWITCH_BOX_MODEL_UNDEFINED )
   {
      string srSwitchBoxModel;
      TAS_ExtractStringSwitchBoxModelType( this->device.switchBoxes.modelType, &srSwitchBoxModel );

      printHandler.Write( pfile, spaceLen, "<sb %s fs = %u />\n", TIO_SR_STR( srSwitchBoxModel ),
                                                                  this->device.switchBoxes.fs );

   }

   if( TCTF_IsGT( this->device.connectionBoxes.capInput, 0.0 ) ||
       TCTF_IsGT( this->device.connectionBoxes.delayInput, 0.0 ))
   {
      printHandler.Write( pfile, spaceLen, "<cb cap_in = %0.*e delay_in = %0.*e />\n",
                                           precision + 1, this->device.connectionBoxes.capInput,
                                           precision + 1, this->device.connectionBoxes.delayInput );
   }

   if( this->device.segments.dirType != TAS_SEGMENT_DIR_UNDEFINED )
   {
      string srSegmentDir;
      TAS_ExtractStringSegmentDirType( this->device.segments.dirType, &srSegmentDir );

      printHandler.Write( pfile, spaceLen, "<segments %s />\n", TIO_SR_STR( srSegmentDir ));
   }

   spaceLen -= 3;
   printHandler.Write( pfile, spaceLen, "</config>\n" );
}

//===========================================================================//
// Method         : PrintXML
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
void TAS_Config_c::PrintXML( 
      void ) const
{
   FILE* pfile = 0;
   size_t spaceLen = 0;

   this->PrintXML( pfile, spaceLen );
}

//===========================================================================//
void TAS_Config_c::PrintXML( 
      FILE*  pfile,
      size_t spaceLen ) const
{
   TC_MinGrid_c& MinGrid = TC_MinGrid_c::GetInstance( );
   unsigned int precision = MinGrid.GetPrecision( );

   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );

   if( TCTF_IsGT( this->layout.autoSize.aspectRatio, 0.0 ))
   {
      printHandler.Write( pfile, spaceLen, "<layout auto=\"%0.*f\"/>\n", 
                                           precision, this->layout.autoSize.aspectRatio );
   }
   else if( this->layout.manualSize.gridDims.IsValid( ))
   {
      printHandler.Write( pfile, spaceLen, "<layout width=\"%u\" height=\"%u\"/>\n", 
                                           this->layout.manualSize.gridDims.width, 
                                           this->layout.manualSize.gridDims.height ); 
   }

   printHandler.Write( pfile, spaceLen, "<device>\n" );
   spaceLen += 3;

   printHandler.Write( pfile, spaceLen, "<sizing R_minW_nmos=\"%0.*f\" R_minW_pmos=\"%0.*f\" ipin_mux_trans_size=\"%0.*f\"/>\n",
                                        precision, this->device.areaModel.resMinWidthNMOS,
                                        precision, this->device.areaModel.resMinWidthPMOS,
                                        precision, this->device.areaModel.sizeInputPinMux );
   printHandler.Write( pfile, spaceLen, "<area grid_logic_tile_area=\"%0.*f\"/>\n",
                                        precision, this->device.areaModel.areaGridTile );
   printHandler.Write( pfile, spaceLen, "<timing C_ipin_cblock=\"%0.*e\" T_ipin_cblock=\"%0.*e\"/>\n",
                                        precision + 1, this->device.connectionBoxes.capInput,
                                        precision + 1, this->device.connectionBoxes.delayInput ); 

   string srSwitchBoxModelType;
   TAS_ExtractStringSwitchBoxModelType( this->device.switchBoxes.modelType, &srSwitchBoxModelType );
   printHandler.Write( pfile, spaceLen, "<switch_block type=\"%s\" fs=\"%u\"/>\n",
                                        TIO_SR_STR( srSwitchBoxModelType ),
                                        this->device.switchBoxes.fs );

   printHandler.Write( pfile, spaceLen, "<chan_width_distr>\n" );
   this->device.channelWidth.io.PrintXML( pfile, spaceLen + 3 );
   this->device.channelWidth.x.PrintXML( pfile, spaceLen + 3 );
   this->device.channelWidth.y.PrintXML( pfile, spaceLen + 3 );
   printHandler.Write( pfile, spaceLen, "</chan_width_distr>\n" );

   spaceLen -= 3;
   printHandler.Write( pfile, spaceLen, "</device>\n" );
}
