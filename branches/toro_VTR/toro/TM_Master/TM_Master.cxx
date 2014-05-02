//===========================================================================//
// Purpose : Method definitions for the TM_Master class.
//
//           Public methods include:
//           - TM_Master_c, ~TM_Master_c
//           - Apply
//
//           Private methods include:
//           - NewHandlerSingletons_
//           - DeleteHandlerSingletons_
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

#include "TIO_PrintHandler.h"

#include "TOS_OptionsStore.h"
#include "TFS_FloorplanStore.h"

#if defined( TM_MASTER_NEW_DELETE_HANDLERS )
   #include "TC_MinGrid.h"
#endif

#include "TI_Input.h"
#include "TP_Process.h"
#include "TO_Output.h"

#include "TM_Master.h"

//===========================================================================//
// Method         : TM_Master_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
TM_Master_c::TM_Master_c( 
      void )
{
}

//===========================================================================//
TM_Master_c::TM_Master_c( 
      const TCL_CommandLine_c& commandLine,
            bool*              pok )
{
   bool ok = this->Apply( commandLine );
   if( pok )
   {
      *pok = ok;
   }
}

//===========================================================================//
// Method         : ~TM_Master_c
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
TM_Master_c::~TM_Master_c( 
      void )
{
}

//===========================================================================//
// Method         : Apply
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
bool TM_Master_c::Apply( 
      const TCL_CommandLine_c& commandLine )
{
   bool ok = true;

   // Initialize various data handler singletons prior to doing any useful work
   this->NewHandlerSingletons_( );

   // And allocate an options store and floorplan (architecture + circuit) store
   TOS_OptionsStore_c optionsStore;
   TFS_FloorplanStore_c floorplanStore;

   // Inform user that "Elvis has entered the building..."
   TIO_PrintHandler_c& printHandler = TIO_PrintHandler_c::GetInstance( );
   printHandler.WriteBanner( );

   // Time to do get serious and do some input-process-output work
   if( ok )
   {
      TI_Input_c input;
      ok = input.Apply( commandLine, &optionsStore, &floorplanStore );
   }
   if( ok )
   {
      TP_Process_c process;
      ok = process.Apply( optionsStore, &floorplanStore );
   }
   if( ok )
   {
      TO_Output_c output;
      ok = output.Apply( commandLine, optionsStore, floorplanStore );
   }
   if( ok )
   {
      ok = ( printHandler.GetErrorCount( ) == 0 &&
             printHandler.GetInternalCount( ) == 0 ?
             true : false );
   }

   // Destroy various data handler singletons after completing our masterpiece
   this->DeleteHandlerSingletons_( );

   return( ok );
} 

//===========================================================================//
// Method         : NewHandlerSingletons_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
void TM_Master_c::NewHandlerSingletons_(
      void ) const
{
   #if defined( TM_MASTER_NEW_DELETE_HANDLERS )

      // Initialize program data handler 'singletons' (one or more)
      TC_MinGrid_c::NewInstance( );

   #endif
}

//===========================================================================//
// Method         : DeleteHandlerSingletons_
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/01/12 jeffr : Original
//===========================================================================//
void TM_Master_c::DeleteHandlerSingletons_(
      void ) const
{
   #if defined( TM_MASTER_NEW_DELETE_HANDLERS )

      // Don't forget, 'Leave No Trace' (mostly to clean-up memory leak warnings)
      TC_MinGrid_c::DeleteInstance( );

   #endif
}
