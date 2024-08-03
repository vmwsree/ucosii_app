#ifndef __INTEL_NIOSV_M_H__
#define __INTEL_NIOSV_M_H__

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

#include "string.h"

#include "alt_types.h"
#include "sys/alt_alarm.h"

#include "system.h"

/*
 * This header provides processor specific macros for accessing the Intel Nios V/m
 * control registers.
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

typedef void (*alt_niosv_timer_isr_t)(alt_u32 cause, alt_u32 epc, alt_u32 tval);
typedef void (*alt_niosv_sw_isr_t)(alt_u32 cause, alt_u32 epc, alt_u32 tval);

#define ALT_VOLATILE_ACCESS(x) (*(__typeof__(*x) volatile *)(x))

// Handy stringification marcos
#define ALT_MKSTR(s) #s
#define ALT_MKSTR_EXPAND(s) ALT_MKSTR(s)

#define ABBOTTS_LAKE_IC_ID 0
#define ABBOTTS_LAKE_SOFTWARE_IRQ  3
#define ABBOTTS_LAKE_TIMER_IRQ     7
#define ABBOTTS_LAKE_EXTERNAL_IRQ  11

#define ABBOTTS_LAKE_NUM_SYNCH_EXCEPTIONS 16


/*
 * Memory mapped control registers
 */
#define ABBOTTS_LAKE_MSIP_ADDR     (ABBOTTSLAKE_MTIME_OFFSET + 0x10)
#define ABBOTTS_LAKE_MTIMECMP_ADDR ABBOTTSLAKE_MTIME_OFFSET
#define ABBOTTS_LAKE_MTIME_ADDR    (ABBOTTSLAKE_MTIME_OFFSET + 0x8)
#define MTIMECMP_MAX_VALUE         0xFFFFFFFFFFFFFFFF

/*
 * Control registers (CSRs)
 */
#define ABBOTTS_LAKE_MSTATUS_CSR 0x300
#define ABBOTTS_LAKE_MIE_CSR     0x304
#define ABBOTTS_LAKE_MTVEC_CSR   0x305
#define ABBOTTS_LAKE_MEPC_CSR    0x341
#define ABBOTTS_LAKE_MCAUSE_CSR  0x342
#define ABBOTTS_LAKE_MIP_CSR     0x344

/*
 * Control register (CSR) fields
 */
#define ABBOTTS_LAKE_MSTATUS_MIE_MASK 0x8
#define ABBOTTS_LAKE_MIE_MASK 0xFFFFFFFF
#define ABBOTTS_LAKE_MCAUSE_INTERRUPT_MASK 0x80000000

/*
 * Number of available IRQs in internal interrupt controller.
 * Note: For Abbott's Lake, the top 16 bits of mie / mip are
 *       available for general purpose interrupts, and the bottom
 *       16 are reserved.  We are only exposing the top 16 bits.
 */
#define ABBOTTS_LAKE_NIRQ 16

/*
 * Macros to access control registers.
 */

/* Read CSR regNum and set dst to its value. */
#define ABBOTTS_LAKE_READ_CSR(regNum, dst)                         \
  do { asm volatile ("csrr %[dstReg], %[imm]"               \
    : [dstReg] "=r"(dst)                                    \
    : [imm] "i"(regNum)); } while (0)

/* Writes CSR regNum with src */
#define ABBOTTS_LAKE_WRITE_CSR(regNum, src)                        \
  do { asm volatile ("csrw %[imm], %[srcReg]"               \
    :                                                       \
    : [imm] "i"(regNum), [srcReg] "r"(src)); } while (0)

/* Read CSR regNum, set dst to its value, and write CSR regNum with src. */
#define ABBOTTS_LAKE_SWAP_CSR(regNum, src, dst)                    \
  do { asm volatile ("csrrw %[dstReg], %[imm], %[srcReg]"   \
    : [dstReg] "=r"(dst)                                    \
    : [imm] "i"(regNum), [srcReg] "r"(src)); } while (0)

/* 
 * Bit-wise OR the CSR with bitmask.
 * Any bit that is 1 in bitmask causes the corresponding bit in CSR to be set to 1.
 */
#define ABBOTTS_LAKE_SET_CSR(regNum, bitmask)                      \
  do { asm volatile ("csrs %[imm], %[srcReg]"               \
    :                                                       \
    : [imm] "i"(regNum), [srcReg] "r"(bitmask)); } while (0)

/* 
 * Read CSR regNum, set dst to its value, and bit-wise OR the CSR with bitmask.
 * Any bit that is 1 in bitmask causes the corresponding bit in CSR to be set to 1.
 */
#define ABBOTTS_LAKE_READ_AND_SET_CSR(regNum, dst, bitmask)        \
  do { asm volatile ("csrrs %[dstReg], %[imm], %[srcReg]"   \
    : [dstReg] "=r"(dst)                                    \
    : [imm] "i"(regNum), [srcReg] "r"(bitmask)); } while (0)

/* 
 * Bit-wise AND the CSR with ~bitmask.
 * Any bit that is 1 in bitmask causes the corresponding bit in CSR to be cleared to 0.
 */
#define ABBOTTS_LAKE_CLR_CSR(regNum, bitmask)                      \
  do { asm volatile ("csrc %[imm], %[srcReg]"              \
    :                                                       \
    : [imm] "i"(regNum), [srcReg] "r"(bitmask)); } while (0)

/* 
 * Read CSR regNum, set dst to its value, and bit-wise AND the CSR with ~bitmask.
 * Any bit that is 1 in bitmask causes the corresponding bit in CSR to be cleared to 0.
 */
#define ABBOTTS_LAKE_READ_AND_CLR_CSR(regNum, dst, bitmask)        \
  do { asm volatile ("csrrc %[dstReg], %[imm], %[srcReg]"   \
    : [dstReg] "=r"(dst)                                    \
    : [imm] "i"(regNum), [srcReg] "r"(bitmask)); } while (0)

/* set specific bit to 1 */
#define ABBOTTS_LAKE_CSR_SET_BIT(regNum, bit)        \
  do { asm volatile ("csrsi %[imm], %[srcReg]"   \
    :                                                       \
    : [imm] "i"(regNum), [srcReg] "r"(bit)); } while (0)

/* set specific bit to 0 */
#define ABBOTTS_LAKE_CSR_CLR_BIT(regNum, bit)        \
  do { asm volatile ("csrci %[imm], %[srcReg]"   \
    :                                                       \
    : [imm] "i"(regNum), [srcReg] "r"(bit)); } while (0)
/*
 * Macros for accessing select Abbott's Lake general-purpose registers.
 */

/* SP (Stack Pointer) register */ 
#define ABBOTTS_LAKE_READ_SP(sp) \
    do { __asm ("mv %0, sp" : "=r" (sp) ); } while (0)

/*
 * Macros for useful processor instructions.
 */
#define ABBOTTS_LAKE_EBREAK() \
    do { __asm volatile ("ebreak"); } while (0)

/* TODO: Figure this out for Abbott's Lake. No optional immediate for RISC-V ebreak supported. */
#define ABBOTTS_LAKE_REPORT_STACK_OVERFLOW() \
    do { __asm volatile("ebreak"); } while (0)


// Functions for getting mtime, and get/set mtimecmp
alt_u64 alt_niosv_mtime_get();
void alt_niosv_mtimecmp_set(alt_u64 time);
alt_u64 alt_niosv_mtimecmp_get();
void alt_niosv_mtimecmp_interrupt_init();
extern alt_niosv_timer_isr_t alt_niosv_timer_interrupt_handler;

static ALT_INLINE void ALT_ALWAYS_INLINE alt_niosv_register_mtimecmp_interrupt_handle(alt_niosv_timer_isr_t handle) {
    alt_niosv_timer_interrupt_handler = handle;
}

static ALT_INLINE void ALT_ALWAYS_INLINE alt_niosv_enable_timer_interrupt() {
    ABBOTTS_LAKE_SET_CSR(ABBOTTS_LAKE_MIE_CSR, (0x1 << ABBOTTS_LAKE_TIMER_IRQ));
}

static ALT_INLINE void ALT_ALWAYS_INLINE alt_niosv_disable_timer_interrupt() {
    ABBOTTS_LAKE_CLR_CSR(ABBOTTS_LAKE_MIE_CSR, (0x1 << ABBOTTS_LAKE_TIMER_IRQ));
}

static ALT_INLINE int ALT_ALWAYS_INLINE alt_niosv_is_timer_interrupt_enabled() {
    alt_u32 mie_val;
    ABBOTTS_LAKE_READ_CSR(ABBOTTS_LAKE_MIE_CSR, mie_val);   
    return mie_val & (0x1 << ABBOTTS_LAKE_TIMER_IRQ);
}

/*
 * Functions for use as system clock driver and timestamp driver.
 */
void alt_niosv_timer_sc_isr(alt_u32 cause, alt_u32 epc, alt_u32 tval);

extern alt_u64 alt_niosv_timestamp_offset;

static ALT_INLINE int ALT_ALWAYS_INLINE alt_niosv_timer_timestamp_start() {
    alt_niosv_timestamp_offset = alt_niosv_mtime_get();
    return 0;
}

static ALT_INLINE alt_u64 ALT_ALWAYS_INLINE alt_niosv_timer_timestamp() {
    return alt_niosv_mtime_get() - alt_niosv_timestamp_offset;
}

static ALT_INLINE alt_u32 ALT_ALWAYS_INLINE alt_niosv_timer_timestamp_freq() {
    return ALT_CPU_CPU_FREQ;
}

// Determine amount to add to mtimecmp to trigger a subsequent interrupt at
#define MTIMECMP_DELTA_AMT (ALT_CPU_CPU_FREQ / ALT_CPU_TICKS_PER_SEC)

// System initialization macros
#define INTEL_NIOSV_M_INSTANCE(name, dev) extern int alt_no_storage

// This is what's called from alt_sys_init.v
#define INTEL_NIOSV_M_INIT(name, dev)                                        \
  if (strcmp(ALT_MKSTR_EXPAND(name), ALT_MKSTR_EXPAND(ALT_SYS_CLK)) == 0)    \
  {                                                                          \
      alt_sysclk_init(ALT_CPU_TICKS_PER_SEC);                                \
      alt_niosv_register_mtimecmp_interrupt_handle(alt_niosv_timer_sc_isr);  \
      alt_niosv_mtimecmp_interrupt_init();                                   \
      alt_niosv_timer_sc_isr(0, 0, 0);                                       \
  }                                                                          \

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __INTEL_NIOSV_M_H__ */
