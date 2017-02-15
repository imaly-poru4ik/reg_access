////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2016 Igal Maly. ALL RIGHTS RESERVED.
// Use of this source code is governed by a FreeBSD License that can be found
// in the LICENSE file.
//------------------------------------------------------------------------------
/// \file
/// \author imaly
///
/// \brief Usage example and unit-test
///
////////////////////////////////////////////////////////////////////////////////

#define _TEST_PURE_SW_ENV   1   // testing with no real HW platform

#include "regfile_access.h"

#include <string.h>
#include <assert.h>
#include "dma_example.h"
#include "sysregfile.h"



// for hw_access.h
MEM_ADDR_t arrSimulateMemoryChunk[0x1fff];


//-------- HW REGFILE PARSING ------

#ifdef _FOR_EDITOR_VISIBILITY
volatile const MY_DMA_REGS_st* const g_pHwRegFileDma;
#else
#define g_pHwRegFileDma ((const MY_DMA_REGS_st* const)(MY_DMA_BASE_ADDR))
#endif


/// test via g_pHwRegFileDma
void TestDma_RegAccess(void)
{
    volatile uint32_t b[10];

    REG_FIELD_WR(g_pHwRegFileDma->sgl[2].len.mem_len, 15);
    REG(g_pHwRegFileDma->sgl[2].ptr) = 45;
    REG(g_pHwRegFileDma->format) = 1000;
    REG(g_pHwRegFileDma->ctxt) = 0;
    REG_FIELD_WR(g_pHwRegFileDma->ctxt.fragment_id, 7);
    REG(g_pHwRegFileDma->ctrl) = 2;
    REG_BIT_CLR(g_pHwRegFileDma->ctrl.clr);
    REG_BIT_SET(g_pHwRegFileDma->ctrl.start);

    b[0] = REG_FIELD_GET(g_pHwRegFileDma->sgl[2].len.mem_len);
    b[1] = REG(g_pHwRegFileDma->sgl[2].ptr);
    b[2] = REG(g_pHwRegFileDma->format);
    b[3] = REG_FIELD_GET(g_pHwRegFileDma->ctxt.fragment_id);
    b[4] = REG(g_pHwRegFileDma->ctrl);
    b[5] = REG_FIELD_GET(g_pHwRegFileDma->ctrl.clr);
    b[6] = REG_FIELD_GET(g_pHwRegFileDma->ctrl.start);

    assert(15 == b[0]);
    assert(45 == b[1]);
    assert(1000 == b[2]);
    assert(7 == b[3]);
    assert(1 == b[4]);
    assert(0 == b[5]);
    assert(1 == b[6]);
}


/// test via g_sysRegs
void TestDma_SystemRegAccess(void)
{
    volatile uint32_t b[10];

    REG_FIELD_WR(g_sysRegs.pDma->sgl[2].len.mem_len, 15);
    REG(g_sysRegs.pDma->sgl[2].ptr) = 45;
    REG(g_sysRegs.pDma->format) = 1000;
    REG(g_sysRegs.pDma->ctxt) = 0;
    REG_FIELD_WR(g_sysRegs.pDma->ctxt.fragment_id, 7);
    REG(g_sysRegs.pDma->ctrl) = 2;
    REG_BIT_CLR(g_sysRegs.pDma->ctrl.clr);
    REG_BIT_SET(g_sysRegs.pDma->ctrl.start);

    b[0] = REG_FIELD_GET(g_sysRegs.pDma->sgl[2].len.mem_len);
    b[1] = REG(g_sysRegs.pDma->sgl[2].ptr);
    b[2] = REG(g_sysRegs.pDma->format);
    b[3] = REG_FIELD_GET(g_sysRegs.pDma->ctxt.fragment_id);
    b[4] = REG(g_sysRegs.pDma->ctrl);
    b[5] = REG_FIELD_GET(g_sysRegs.pDma->ctrl.clr);
    b[6] = REG_FIELD_GET(g_sysRegs.pDma->ctrl.start);

    assert(15 == b[0]);
    assert(45 == b[1]);
    assert(1000 == b[2]);
    assert(7 == b[3]);
    assert(1 == b[4]);
    assert(0 == b[5]);
    assert(1 == b[6]);
}


// -- Test via Prepare-Commit Shadow registers --

/// Prepare configuration on shadow registers (HW registers not accessed)
void TestDma_ShadowPrepare(MY_DMA_REGS_st* const o_pDmaRegs)
{
    volatile uint32_t b[10];

    memset(o_pDmaRegs, 0, sizeof(*o_pDmaRegs));

    SHADOW_FIELD_WR_NO_CLR(o_pDmaRegs->sgl[2].len.mem_len, 15);

    o_pDmaRegs->sgl[2].ptr.regVal = 45;
    o_pDmaRegs->format.regVal = 1000;
    SHADOW(o_pDmaRegs->ctxt) = 0;
    SHADOW_FIELD_WR_NO_CLR(o_pDmaRegs->ctxt.fragment_id, 7);
    SHADOW(o_pDmaRegs->ctrl) = 3;
    SHADOW_BIT_CLR(o_pDmaRegs->ctrl.clr);
    SHADOW_BIT_CLR(o_pDmaRegs->ctrl.start);

    b[0] = SHADOW_FIELD_GET(o_pDmaRegs->sgl[2].len.mem_len);
    b[1] = SHADOW(o_pDmaRegs->sgl[2].ptr);
    b[2] = SHADOW(o_pDmaRegs->format);
    b[3] = SHADOW_FIELD_GET(o_pDmaRegs->ctxt.fragment_id);
    b[4] = SHADOW(o_pDmaRegs->ctrl);
    b[5] = SHADOW_FIELD_GET(o_pDmaRegs->ctrl.clr);
    b[6] = SHADOW_FIELD_GET(o_pDmaRegs->ctrl.start);

    assert(15 == b[0]);
    assert(45 == b[1]);
    assert(1000 == b[2]);
    assert(7 == b[3]);
    assert(0 == b[4]);
    assert(0 == b[5]);
    assert(0 == b[6]);
}


/// Commit previously prepared shadow registers to HW registers
void TestDma_ShadowCommit(const MY_DMA_REGS_st* const i_pDmaRegs)
{
    memcpy((void*)&HW_MEM(MY_DMA_BASE_ADDR), i_pDmaRegs, sizeof(*i_pDmaRegs));

    SHADOW_BIT_SET(i_pDmaRegs->ctrl.start);
    // activate
    REG(g_pHwRegFileDma->ctrl) = SHADOW(i_pDmaRegs->ctrl);
}


void TestDma_ShadowRegAccess(void)
{
    MY_DMA_REGS_st dmaRegs;
    TestDma_ShadowPrepare(&dmaRegs);
    TestDma_ShadowCommit(&dmaRegs);
}


//---- Main Test entry point -----

int main(void)
{
    // testing different examples of engine configuration

    memset(arrSimulateMemoryChunk, 0xba, sizeof(arrSimulateMemoryChunk));
    TestDma_RegAccess();

    memset(arrSimulateMemoryChunk, 0xab, sizeof(arrSimulateMemoryChunk));
    TestDma_SystemRegAccess();

    memset(arrSimulateMemoryChunk, 0xfc, sizeof(arrSimulateMemoryChunk));
    TestDma_ShadowRegAccess();

    return 0;
}

