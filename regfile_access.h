////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2016 Igal Maly. ALL RIGHTS RESERVED.
// Use of this source code is governed by a FreeBSD License that can be found
// in the LICENSE file.
//------------------------------------------------------------------------------
/// \file
/// \author imaly
///
/// \brief Regfile accessing/parsing
///  Registers are Engine's struct members, pointing to physical bus address
///  Register fields are register members represented as 2-dimensional arrays
///     as follows:
///  ==========================================================
///  ||   field[field bit width][field start bit number*]    ||
///  ==========================================================
///             *counting from 1 - first bit (LSB) is '1' (and not '0')
///
////////////////////////////////////////////////////////////////////////////////


#ifndef REGFILE_ACCESS_H_
#define REGFILE_ACCESS_H_


#include "hwaccess.h"


/// HW engine register access (for reading and assignment)
/// \regPath represents a register following this project's regfile convention
/// \usage  Read:   a = REG(pHwRegFileEng->someRegister);
///         Write:  REG(pHwRegFileEng->someRegister) = 5;
#define REG(regPath)                    HW_MEM(REG_ADDR(regPath))

/// HW engine field-in-a-register reading
/// 'Field' is a continous set of bits in a register with a specific function
/// \fieldPath represents a field member following this project's regfile convention
/// \usage a = REG_FIELD_GET(pHwRegFileEng->someRegister.someField);
#define REG_FIELD_GET(fieldPath)        HW_FIELD_GET(REG_ADDR(fieldPath), FIELD_BIT_OFFSET(fieldPath), FIELD_BIT_SIZE(fieldPath))

/// HW engine field-in-a-register writing
/// 'Field' is a continous set of bits in a register with a specific function
/// \fieldPath represents a field member following this project's regfile convention
/// \usage REG_FIELD_WR(pHwRegFileEng->someRegister.someField, 5);
#define REG_FIELD_WR(fieldPath, val)    HW_FIELD_WR(REG_ADDR(fieldPath), FIELD_BIT_OFFSET(fieldPath), FIELD_BIT_SIZE(fieldPath), val)

/// Alias for HW engine bit-field-in-a-register clearing
#define REG_BIT_CLR(fieldPath)          REG_FIELD_WR(fieldPath, 0)
/// Alias for HW engine bit-field-in-a-register setting
#define REG_BIT_SET(fieldPath)          REG_FIELD_WR(fieldPath, 1)


//------------------------------------------------------------
// For manipulating shadow registers (not on HW bus)
//------------------------------------------------------------

/// Same as REG(), but with no volatile casting
#define SHADOW(path)                    (*((MEM_ADDR_t*)(&(path))))

#define SHADOW_FIELD_GET(fieldPath) \
    BITS_GET(SHADOW(fieldPath), FIELD_BIT_OFFSET(fieldPath), FIELD_BIT_SIZE(fieldPath))

#define SHADOW_FIELD_WR(fieldPath, val) \
    BITS_WR(SHADOW(fieldPath), FIELD_BIT_OFFSET(fieldPath), FIELD_BIT_SIZE(fieldPath), val)

#define SHADOW_FIELD_WR_NO_CLR(fieldPath, val) \
    BITS_WR_NO_CLR(SHADOW(fieldPath), FIELD_BIT_OFFSET(fieldPath), FIELD_BIT_SIZE(fieldPath), val)

#define SHADOW_BIT_CLR(fieldPath)   BIT_CLR(SHADOW(fieldPath), FIELD_BIT_OFFSET(fieldPath))
#define SHADOW_BIT_SET(fieldPath)   BIT_SET(SHADOW(fieldPath), FIELD_BIT_OFFSET(fieldPath))


//---- INTERNAL, USE MACROS ABOVE -----
#define FIELD_BIT_OFFSET(fieldPath)     (sizeof((*fieldPath)[0]) - 1)
#define FIELD_BIT_SIZE(fieldPath)       (sizeof(*fieldPath) / sizeof((*fieldPath)[0]))
#define REG_ADDR(path)                  ((MEM_ADDR_t)(&(path)))



#endif // Header-file guard

