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


#include "sysregfile.h"



SYSTEM_BUS_st g_sysRegs = {
    .eng = {
        .eng_glob = (volatile ENG_GLOB_REGS_st*)ENG_GLOB_BASE_ADDR,
        .arrEng_f[0] = (volatile ENG_F_REGS_st*)( ENG_F_BASE_ADDR + ( 0 * ENG_F_ADDR_SIZE ) ),
        .arrEng_f[1] = (volatile ENG_F_REGS_st*)( ENG_F_BASE_ADDR + ( 1 * ENG_F_ADDR_SIZE ) ),
    },
    .arrIf[0] = {
        .ifu = (volatile IFU_REGS_st*)0xFE842000,
        .arrIfc =
        {
            (volatile IFC_REGS_st*)0xFE800000,
            (volatile IFC_REGS_st*)0xFE804000
        },
    },
    .arrIf[1] = {
        .ifu = (volatile IFU_REGS_st*)0xFE842000,
        .arrIfc = {
            (volatile IFC_REGS_st*)0xFE808000,
            (volatile IFC_REGS_st*)0xFE80C000
        },
    },
    .pClocks = (volatile CLK_REGS_st*)CLK_BASE_ADDR,
    .pGlob = (volatile GLB_REGS_st*)GLB_BASE_ADDR,

    .pDma = (volatile MY_DMA_REGS_st*)MY_DMA_BASE_ADDR
};


