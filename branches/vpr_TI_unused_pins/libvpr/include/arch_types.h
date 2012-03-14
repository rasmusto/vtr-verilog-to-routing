/*
Data types describing the FPGA architecture.

Date: February 19, 2009
Authors: Jason Luu and Kenneth Kent
*/

#ifndef ARCH_TYPES_H
#define ARCH_TYPES_H

#include "logic_types.h"
#include "physical_types.h"

/* Constant describing architecture library version number */
/* JR-031412 */
#define VPR_VERSION "6.0 Full Release (TI-beta 20120314.1)"

/* Input file parsing. */
#define TOKENS " \t\n"

/* Value for UNDEFINED data */
#define UNDEFINED -1

/* Maximum value for mininum channel width to avoid overflows of short data type.               */
#define MAX_CHANNEL_WIDTH 8000

#endif

