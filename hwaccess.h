////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2016 Igal Maly. ALL RIGHTS RESERVED.
// Use of this source code is governed by a FreeBSD License that can be found
// in the LICENSE file.
//------------------------------------------------------------------------------
/// \file
/// \author imaly
///
/// \brief Preprocessor macros to access (read & write) and manipulate
/// hardware resources: registers, fields and bits inside registers.
///
////////////////////////////////////////////////////////////////////////////////


#ifndef HW_ACCESS_H_
#define HW_ACCESS_H_


#include <stdint.h>     // uint32_t
#include "bitops.h"


/// Type designated for accessing data residing in an internal memory buffer
typedef uint32_t MEM_ADDR_t;


#if _TEST_PURE_SW_ENV
extern MEM_ADDR_t arrSimulateMemoryChunk[0x1fff];    //5 bit base + 8 bit offset

#define HW_ADDR(regAddr) \
    (arrSimulateMemoryChunk[(((regAddr) >> 11) & 0x1f00) | ((regAddr) & 0xff)])
#else

/// Any bus access should be done with this macro (HW memory/registers access)
#define HW_ADDR(regAddr)        (*((volatile MEM_ADDR_t*)(regAddr)))
#endif



/// hardware bus access (memory / register) - both for reading and assignment
/// doesn't include printing
/// \usage Read: a = HW_MEM(REG_ADDR); Write: HW_MEM(REG_ADDR) = 5;
#define HW_MEM(regAddr)     HW_ADDR(regAddr)


//:NOTE: Read field and bit are macros to allow different types passing for read value (8/16bit, bool)
#define HW_FIELD_GET(regAddr, fieldShift, fieldSize) \
    BITS_GET(HW_ADDR(regAddr), (fieldShift), (fieldSize))

#define HW_BIT_GET(regAddr, bitShift) \
    BIT_GET(HW_ADDR(regAddr), (bitShift))

#define HW_FIELD_WR(regAddr, fieldShift, fieldSize, val) \
    HW_ADDR(regAddr) = BITS_WRS(HW_ADDR(regAddr), fieldShift, fieldSize, val)

#define HW_BIT_WR(regAddr, bitShift, bitVal) \
    HW_ADDR(regAddr) = BIT_WRS(HW_ADDR(regAddr), bitShift, bitVal)


#define HW_BIT_CLR(regAddr, bitShift)   HW_BIT_WR(regAddr, bitShift, 0)
#define HW_BIT_SET(regAddr, bitShift)   HW_BIT_WR(regAddr, bitShift, 1)


#endif // Header-file guard

