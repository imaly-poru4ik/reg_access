////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2016 Igal Maly. ALL RIGHTS RESERVED.
// Use of this source code is governed by a FreeBSD License that can be found
// in the LICENSE file.
//------------------------------------------------------------------------------
/// \file
/// \author imaly
///
/// \brief HW unit regfile example
///  Register fields are represented as pointers to 2-dimensional arrays
///  as follows:
///  ==========================================================
///  =     field[field bit width][field start bit number*]    =
///  ==========================================================
///             *first bit (LSB) is '1' (and not 0)
///
////////////////////////////////////////////////////////////////////////////////


#ifndef DMA_EXAMPLE_REGS_H_
#define DMA_EXAMPLE_REGS_H_


#include <stdint.h>     // uint32_t


#define MY_DMA_BASE_ADDR            (0xf0000)


typedef union
{
    uint8_t (*start)[1][0+1]; ///< activation bit
    uint8_t (*clr)[1][1+1];   ///< clear the DMA

    /// for debug - complete register value
    uint32_t regVal;
    /// for debug - bit-fields
    struct
    {
        uint32_t start              : 1;
        uint32_t clr                : 1;
        uint32_t RSRVD              : 30;
    } bitFields;
} MY_DMA_CTRL_ut;


typedef union
{
    uint8_t (*mem_ptr)[22][0+1];

    uint32_t regVal;
    struct
    {
        uint32_t mem_ptr            : 22;
        uint32_t RSRVD              : 10;
    } bitFields;
} MY_DMA_MEM_PTR_ut;


typedef union
{
    uint8_t (*mem_len)[18][0+1];

    uint32_t regVal;
    struct
    {
        uint32_t mem_len            : 18;
        uint32_t RSRVD              : 14;
    } bitFields;
} MY_DMA_MEM_LEN_ut;


typedef struct
{
    MY_DMA_MEM_PTR_ut ptr;
    MY_DMA_MEM_LEN_ut len;
} MY_DMA_MEM_ELEM_st;


typedef union
{
    uint8_t (*context_id)[16][0+1];
    uint8_t (*source_id)[8][16+1];
    uint8_t (*fragment_id)[6][24+1];

    uint32_t regVal;
    struct
    {
        uint32_t context_id         : 16;
        uint32_t source_id          : 8;
        uint32_t fragment_id        : 6;
        uint32_t RSRVD              : 2;
    } bitFields;
} MY_DMA_CONTEXT_ut;


typedef union
{
    uint8_t (*payload)[14][0+1];
    uint8_t (*metadata)[12][14+1];

    uint32_t regVal;
    struct
    {
        uint32_t payload            : 14;
        uint32_t metadata           : 12;
        uint32_t RSRVD              : 6;
    } bitFields;
} MY_DMA_DATA_FORMAT_ut;


typedef struct
{
    MY_DMA_CTRL_ut                  ctrl;
    MY_DMA_MEM_ELEM_st              sgl[8];
    MY_DMA_CONTEXT_ut               ctxt;
    MY_DMA_DATA_FORMAT_ut           format;
} MY_DMA_REGS_st;


#endif // Header-file guard

