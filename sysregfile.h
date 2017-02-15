////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2016 Igal Maly. ALL RIGHTS RESERVED.
// Use of this source code is governed by a FreeBSD License that can be found
// in the LICENSE file.
//------------------------------------------------------------------------------
/// \file
/// \author imaly
///
/// \brief System bus representation for registers accessing and debug
///
////////////////////////////////////////////////////////////////////////////////


#ifndef SYSREGFILE_H_
#define SYSREGFILE_H_


#include "dma_example.h"



//=== EXAMPLE

#define ENG_GLOB_BASE_ADDR          0x100
#define ENG_F_BASE_ADDR             0x200
#define CLK_BASE_ADDR               0x600
#define GLB_BASE_ADDR               0x700
#define ENG_F_ADDR_SIZE             24


typedef struct
{
    uint32_t reg1;
    uint32_t reg2;
} EXAMPLE_REGS_st;


typedef EXAMPLE_REGS_st IFU_REGS_st;
typedef EXAMPLE_REGS_st IFC_REGS_st;
typedef EXAMPLE_REGS_st CLK_REGS_st;
typedef EXAMPLE_REGS_st GLB_REGS_st;
typedef EXAMPLE_REGS_st ENG_GLOB_REGS_st;
typedef EXAMPLE_REGS_st ENG_F_REGS_st;

typedef struct
{
    volatile IFU_REGS_st*   ifu;
    volatile IFC_REGS_st*   arrIfc[2];
} IF_REGS_st;

typedef struct
{
    volatile ENG_GLOB_REGS_st*  eng_glob;
    volatile ENG_F_REGS_st*     arrEng_f[2];
} ENG_REGS_st;

// ====


typedef struct
{
    volatile ENG_REGS_st        eng;
    volatile IF_REGS_st         arrIf[2];
    volatile CLK_REGS_st*       pClocks;
    volatile GLB_REGS_st*       pGlob;

    volatile MY_DMA_REGS_st*    pDma;
} SYSTEM_BUS_st;


extern SYSTEM_BUS_st g_sysRegs;

#endif // Header-file guard

