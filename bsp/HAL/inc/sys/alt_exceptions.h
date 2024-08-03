#ifndef __ALT_EXCEPTIONS_H__
#define __ALT_EXCEPTIONS_H__

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2008 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
******************************************************************************/
#include "alt_types.h"
#include "system.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * This file defines instruction-generated exception handling and registry
 * API, exception type enumeration, and handler return value enumeration for
 * Nios V.
 */

/*
 * The following enumeration describes the value in the mcause CSR.
 */
enum alt_exception_cause_e {
    ABBOTTS_LAKE_UNDEFINED_CAUSE                = -1,
    ABBOTTS_LAKE_INSTRUCTION_ADDRESS_MISALIGNED = 0,
    ABBOTTS_LAKE_INSTRUCTION_ACCESS_FAULT       = 1,
    ABBOTTS_LAKE_ILLEGAL_INSTRUCTION            = 2,
    ABBOTTS_LAKE_BREAKPOINT                     = 3,
    ABBOTTS_LAKE_LOAD_ADDRESS_MISALIGNED        = 4,
    ABBOTTS_LAKE_LOAD_ACCESS_FAULT              = 5,
    ABBOTTS_LAKE_STORE_AMO_ADDRESS_MISALIGNED   = 6,
    ABBOTTS_LAKE_STORE_AMO_ACCESS_FAULT         = 7,
    ABBOTTS_LAKE_ENVIRONMENT_CALL_FROM_U_MODE   = 8,
    ABBOTTS_LAKE_ENVIRONMENT_CALL_FROM_S_MODE   = 9,
    ABBOTTS_LAKE_RESERVED_BIT_10                = 10,
    ABBOTTS_LAKE_ENVIRONMENT_CALL_FROM_M_MODE   = 11,
    ABBOTTS_LAKE_INSTRUCTION_PAGE_FAULT         = 12,
    ABBOTTS_LAKE_LOAD_PAGE_FAULT                = 13,
    ABBOTTS_LAKE_RESERVED_BIT_14                = 14,
    ABBOTTS_LAKE_STORE_AMO_PAGE_FAULT           = 15
};
typedef enum alt_exception_cause_e alt_exception_cause;

/*
 * These define valid return values for a user-defined instruction-generated
 * exception handler. The handler should return one of these to indicate
 * whether to re-issue the instruction that triggered the exception, or to
 * skip it.
 */
enum alt_exception_result_e {
  ABBOTTS_LAKE_EXCEPTION_RETURN_REISSUE_INST = 0,
  ABBOTTS_LAKE_EXCEPTION_RETURN_SKIP_INST    = 1
};
typedef enum alt_exception_result_e alt_exception_result;

/*
 * alt_instruction_exception_register() can be used to register an exception
 * handler for instruction-generated exceptions that are not handled by the
 * built-in exception handler (i.e. for interrupts).
 *
 * The registry API is optionally enabled through the "Enable
 * Instruction-related Exception API" HAL BSP setting, which will
 * define the macro below.
 */
#ifdef ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
void alt_instruction_exception_register (
  alt_exception_result (*exception_handler)(
    alt_exception_cause cause,
    alt_u32 exception_pc,
    alt_u32 bad_addr) );
#endif /*ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API */

/*
 * alt_exception_cause_generated_bad_addr() indicates whether a particular
 * exception cause value was from an exception-type that generated a valid
 * address in the BADADDR register. The contents of BADADDR is passed to
 * a user-registered exception handler in all cases, whether valid or not.
 * This routine should be called to validate the bad_addr argument to
 * your exception handler.
 *
 * Note that this routine will return false (0) for causes 
 * NIOS2_EXCEPTION_TLB_MISS and NIOS2_EXCEPTION_ECC_TLB_ERR.
 * You must read the TLBMISC.D field to determine if BADADDR
 * is valid for these (valid if TLBMISC.D = 1).
 */
int alt_exception_cause_generated_bad_addr(alt_exception_cause cause);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ALT_EXCEPTIONS_H__ */
