////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2016 Igal Maly. ALL RIGHTS RESERVED.
// Use of this source code is governed by a FreeBSD License that can be found
// in the LICENSE file.
//------------------------------------------------------------------------------
/// \file
/// \author imaly
///
/// \brief Collection of useful bit operations
///
////////////////////////////////////////////////////////////////////////////////


#ifndef BITOPS_H_
#define BITOPS_H_


////////////////////////////////////////////////////////////////////////////////
//      Various bit manipulation snippets
////////////////////////////////////////////////////////////////////////////////

#define BITS_PER_LONG                   (sizeof(long) * 8) // 32

/// Create a contiguous bitmask of \bitsCnt starting from LSB
//same as ((1UL << (bitsCnt)) - 1) but can work on bitsCnt == 32
#define GEN_MASK(bitsCnt)               (~0UL >> (BITS_PER_LONG - (bitsCnt)))

/// Create a contiguous bitmask starting at bit position \bitAddr and of \bitsCnt length
/// \example GEN_MASK_SHIFT(2, 3) gives 0x1C
#define GEN_MASK_SHIFT(bitAddr, bitsCnt)    (GEN_MASK(bitsCnt) << (bitAddr))

#define BIT_MASK(bitAddr)               (1UL << (bitAddr))

#define GEN_FIELD(val, bitAddr, bitsCnt) (((val) & GEN_MASK(bitsCnt)) << (bitAddr))

#define BITS_GET(var, bitAddr, bitsCnt) (((var) >> (bitAddr)) & GEN_MASK(bitsCnt))

#define BIT_GET(var, bitAddr)           (((var) >> (bitAddr)) & 0x1)

/// Clear (set to '0') bit in \var
#define BIT_CLRS(var, bitAddr)          ((var) & ~BIT_MASK(bitAddr))

/// Assert bit and assign result to \var
#define BIT_SET(var, bitAddr)           ((var) |= BIT_MASK(bitAddr))
/// Clear bit and assign result to \var
#define BIT_CLR(var, bitAddr)           ((var) &= ~BIT_MASK(bitAddr))

#define BITS_CLRS(var, bitAddr, bitsCnt) ((var) & ~GEN_MASK_SHIFT(bitAddr, bitsCnt))

/// Set or clear a single bit depending on \val
#define BIT_WRS(var, bitAddr, val) \
    (BIT_CLRS(var, bitAddr) | GEN_FIELD(val, bitAddr, 1))
/// Set or clear a single bit depending on \val and assign result to \var
#define BIT_WR(var, bitAddr, val) \
    ((var) = BIT_CLRS(var, bitAddr) | GEN_FIELD(val, bitAddr, 1))

/// Set contiguous bits to \val, assuming relevant field bits are zeros
/// more efficient than BITS_WRS version if setting bits to variable initialized to '0'
#define BITS_WRS_NO_CLR(var, bitAddr, bitsCnt, val) \
    ((var) | GEN_FIELD(val, bitAddr, bitsCnt))
#define BITS_WR_NO_CLR(var, bitAddr, bitsCnt, val) \
    ((var) |= GEN_FIELD(val, bitAddr, bitsCnt))

/// Set contiguous bits to \val
#define BITS_WRS(var, bitAddr, bitsCnt, val) \
    BITS_WRS_NO_CLR(BITS_CLRS(var, bitAddr, bitsCnt), bitAddr, bitsCnt, val)
#define BITS_WR(var, bitAddr, bitsCnt, val) \
    ((var) = BITS_WRS_NO_CLR(BITS_CLRS(var, bitAddr, bitsCnt), bitAddr, bitsCnt, val))

#endif // Header-file guard

