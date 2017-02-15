**reg_access provides hardware drivers developers a great way to handle hardware registers (e.g. in embedded projects).**

Author: Igal Maly (poru4ik@gmail.com)

# Preamble
As an embedded/firmware engineer I always encountered the need to operate HW engines and was never satisfied with the methods to access HW (peripheral) registers and their fields.
I tried a bunch of classic approaches and was not fully satisfied with any of them. 

For example:

  * Using structures with bitfields is highly non-portable and inefficient (and sometimes efficiency metters this much)
    * [A discussion on the matter](http://embeddedgurus.com/stack-overflow/2009/10/effective-c-tip-6-creating-a-flags-variable/#comment-2390)
  * Using bit-wise operators and plain defines for field offset and length is cumbersome, mix-up prone, doesn't allow easy constructs such as an array of same engine.

I wanted:

  * Less error prone - e.g. association between register's field name and it's offset and length attributes (like in bitfields approach)
  * Portability and efficiency
  * Allowing easy instantiation of registers/groups of registers/units 
  * Visibility - accessing the whole system regfile in a structured manner.

I believe I was able to achieve all that.

Unfortunately the world of embedded programming is built around proprietary solutions with close to none sharing of ideas and available material. 
Everybody is trying to reinvent the wheel, so I decided to share my approach, which I find to be very convinient.

# Description

## Convention and the basic idea
1. Regfiles are structures and individual registers within a unit are accessed as structure members
  * Such structures take no real memory space and are only pointers or even cast of memory address like this:
  
    `#define g_pHwRegFileDma ((const MY_DMA_REGS_st* const)(MY_DMA_BASE_ADDR))`
2. Each member representing a register has the exact address of a HW register it refers to.
3. Every field within a register is described as a pointer to 2 dimensional array, with one of the dimensions sizes indicating field's bit-offset and the other bit-length
4. Every register containing fields is a union of pointers to 2 dimensional arrays as described above.
Example (here first dimension is field size and second is bit-shift number starting from 1)
```C
typedef union
{
    uint8_t (*context_id)[16][1];
    uint8_t (*source_id)[8][17];
    uint8_t (*fragment_id)[6][25];
} MY_DMA_CONTEXT_ut;
```
Having the array sizes, for each field name it's size and offset can be implicitly derrived.
For example `context_id` in the above example is the first (least significant) 16 bits and `source_id` is the following 8 bits

## Usage
Example from `TestDma_RegAccess()` in  **unit_test_main.c**

Writing to a register:
`REG(g_pHwRegFileDma->sgl[2].ptr) = 45;`

Reading a register:
`a = REG(g_pHwRegFileDma->sgl[2].ptr);`

Field setting with a value:
`REG_FIELD_WR(g_pHwRegFileDma->ctxt.fragment_id, 7);`

Setting a single-bit field to 1:
`REG_BIT_SET(g_pHwRegFileDma->ctrl.start);`

Reading a field from a register:
`a = REG_FIELD_GET(g_pHwRegFileDma->sgl[2].len.mem_len);`

## Contents
* **regfile_access.h**: Macros for accessing registers and fields according to described convention: **This is the reg_access API**
* **dma_example.h**: Example of a HW unit (DMA having a scatter-gather list configuration) regfile mapping used for testing and usage illustration: See here the convention of regfile description
* **unit_test_main.c**: Usage example and basic testing: Start here to get usage feel

* **sysregfile.c**: Initialization of base address for the full system regfile example
* **sysregfile.h**: Example of full regfile mapping
* **hwaccess.h**: Macros for accessing HW peripheral memories
* **bitops.h**: General bitwise operation macros used for accessing HW peripherals

Note that regfiles also include bit-field representation (`bitFields` member) and full register representation (`regVal` member), these are for debug (e.g. viewing memory in debugger) and can be omitted.

## Testing and compilation
Source code compiles on GCC and is unit-tested. The approach was also tried on a real HW platform. 
The code is a standard C with no compiler specific tricks, so should compile with any other C compiler.

# Contribution
If you have **any** comments, ideas for improving this method, or know a different great approach of handling HW registers, please share with me.

### What I'm not fully happy about
1. This approach requires understanding the convention, especially when creating the regfiles description.
  * Although I expect it should be more or less automatic process for most projects.
2. The need to count the field offset from 1 to not create zero length arrays

# Embedding in your projects
I **encourage** anyone to use this method or source code, just provide the credit please.

I used standard FreeBSD licensing, see LICENSE file for details. Contact me if there's a problem with the license.
