//===========================================================================//
// Purpose : Declaration and inline definitions for a TCD_CircuitDesign 
//           class.
//
//===========================================================================//

#ifndef TCD_CIRCUIT_DESIGN_H
#define TCD_CIRCUIT_DESIGN_H

#include <cstdio>
#include <string>
using namespace std;

#include "TPO_Typedefs.h"
#include "TPO_Inst.h"

#include "TLO_Typedefs.h"
#include "TLO_Cell.h"

#include "TNO_Typedefs.h"
#include "TNO_NetList.h"

//===========================================================================//
// Purpose        : Class declaration
// Author         : Jeff Rudolph
//---------------------------------------------------------------------------//
// Version history
// 05/15/12 jeffr : Original
//===========================================================================//
class TCD_CircuitDesign_c
{
public:

   TCD_CircuitDesign_c( void );
   TCD_CircuitDesign_c( const TCD_CircuitDesign_c& circuitDesign );
   ~TCD_CircuitDesign_c( void );

   TCD_CircuitDesign_c& operator=( const TCD_CircuitDesign_c& circuitDesign );
   bool operator==( const TCD_CircuitDesign_c& circuitDesign ) const;
   bool operator!=( const TCD_CircuitDesign_c& circuitDesign ) const;

   void Print( FILE* pfile = stdout, size_t spaceLen = 0 ) const;

   void PrintBLIF( void ) const;
   void PrintBLIF( FILE* pfile, size_t spaceLen = 0 ) const;

   bool InitDefaults( const string& srDefaultBaseName );
   bool InitValidate( void );

   bool IsValid( void ) const;

private:

   void InitDefaultsNetList_( const TPO_InstList_t& instList,
                              const TPO_PortList_t& portList,
                              const TLO_CellList_t& cellList,
                              TNO_NetList_c* pnetList ) const;
   void InitDefaultsNetNameList_( const TPO_InstList_t& instList,
                                  const TPO_PortList_t& portList,
                                  TNO_NetList_c* pnetList,
                                  TNO_NameList_t* pnetNameList ) const;

   bool InitValidateNetList_( TNO_NetList_c* pnetList ) const;
   bool InitValidateInstList_( const TPO_InstList_t& instList,
                               const TLO_CellList_t& cellList ) const;

public:

   string srName;   // Define design's top-level name

   TPO_InstList_t blockList; // Define design's physical block list
   TPO_PortList_t portList;  // Define design's input/output port list
   TPO_InstList_t instList;  // Define design's primatives (based on BLIF)
   TLO_CellList_t cellList;  // Define design's non-standard cell list
                             // (see BLIF ".subckt")
   TNO_NetList_c  netList;   // Define design's net list
   TNO_NameList_t netNameList;

private:

   enum TCD_DefCapacity_e 
   { 
      TCD_BLOCK_LIST_DEF_CAPACITY = 64,
      TCD_PORT_LIST_DEF_CAPACITY = 64,
      TCD_INST_LIST_DEF_CAPACITY = 64,
      TCD_CELL_LIST_DEF_CAPACITY = 64,
      TCD_NET_LIST_DEF_CAPACITY = 1024,
      TCD_NET_NAME_LIST_DEF_CAPACITY = 1024
   };
};

#endif
