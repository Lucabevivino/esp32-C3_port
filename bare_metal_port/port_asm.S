/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*
 * The FreeRTOS kernel's RISC-V port is split between the the code that is
 * common across all currently supported RISC-V chips (implementations of the
 * RISC-V ISA), and code which tailors the port to a specific RISC-V chip:
 *
 * + The code that is common to all RISC-V chips is implemented in
 *   FreeRTOS\Source\portable\GCC\RISC-V\portASM.S.  There is only one
 *   portASM.S file because the same file is used no matter which RISC-V chip is
 *   in use.
 *
 * + The code that tailors the kernel's RISC-V port to a specific RISC-V
 *   chip is implemented in freertos_risc_v_chip_specific_extensions.h.  There
 *   is one freertos_risc_v_chip_specific_extensions.h that can be used with any
 *   RISC-V chip that both includes a standard CLINT and does not add to the
 *   base set of RISC-V registers.  There are additional
 *   freertos_risc_v_chip_specific_extensions.h files for RISC-V implementations
 *   that do not include a standard CLINT or do add to the base set of RISC-V
 *   registers.
 *
 * CARE MUST BE TAKEN TO INCLDUE THE CORRECT
 * freertos_risc_v_chip_specific_extensions.h HEADER FILE FOR THE CHIP
 * IN USE.  To include the correct freertos_risc_v_chip_specific_extensions.h
 * header file ensure the path to the correct header file is in the assembler's
 * include path.
 *
 * This freertos_risc_v_chip_specific_extensions.h is for use on RISC-V chips
 * that include a standard CLINT and do not add to the base set of RISC-V
 * registers.
 *
 */

#include "portContext.h"
#include "port_systimer_regs.h"

.section .iram, "ax"
.global xPortStartFirstTask
.global pxPortInitialiseStack
.global freertos_risc_v_trap_handler
.global freertos_risc_v_exception_handler
.global freertos_risc_v_interrupt_handler
.global freertos_risc_v_mtimer_interrupt_handler

.extern vTaskSwitchContext
.extern xTaskIncrementTick
.extern pullMachineTimerCompareRegister
.extern pullNextTime
.extern uxTimerIncrementsForOneTick /* size_t type so 32-bit on 32-bit core and 64-bits on 64-bit core. */
.extern xTaskReturnAddress

.weak freertos_risc_v_application_exception_handler
.weak freertos_risc_v_application_interrupt_handler
/*-----------------------------------------------------------*/
 .macro CLEAR_SYSTIMER_INTERRUPT
    li t0, SYSTIMER_INT_CLR_REG
    li t1, 1
    sw t1, 0(t0)
    fence io, io
  .endm
/*-----------------------------------------------------------*/

/*
 * Unlike other ports pxPortInitialiseStack() is written in assembly code as it
 * needs access to the portasmADDITIONAL_CONTEXT_SIZE constant.  The prototype
 * for the function is as per the other ports:
 * StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters );
 *
 * As per the standard RISC-V ABI pxTopOfStack is passed in in a0, pxCode in
 * a1, and pvParameters in a2.  The new top of stack is passed out in a0.
 *
 * RISC-V maps registers to ABI names as follows (X1 to X31 integer registers
 * for the 'I' profile, X1 to X15 for the 'E' profile, currently I assumed).
 *
 * Register      ABI Name    Description                       Saver
 * x0            zero        Hard-wired zero                   -
 * x1            ra          Return address                    Caller
 * x2            sp          Stack pointer                     Callee
 * x3            gp          Global pointer                    -
 * x4            tp          Thread pointer                    -
 * x5-7          t0-2        Temporaries                       Caller
 * x8            s0/fp       Saved register/Frame pointer      Callee
 * x9            s1          Saved register                    Callee
 * x10-11        a0-1        Function Arguments/return values  Caller
 * x12-17        a2-7        Function arguments                Caller
 * x18-27        s2-11       Saved registers                   Callee
 * x28-31        t3-6        Temporaries                       Caller
 *
 * The RISC-V context is saved to FreeRTOS tasks in the following stack frame,
 * where the global and thread pointers are currently assumed to be constant so
 * are not saved:
 *
 * xCriticalNesting
 * x31
 * x30
 * x29
 * x28
 * x27
 * x26
 * x25
 * x24
 * x23
 * x22
 * x21
 * x20
 * x19
 * x18
 * x17
 * x16
 * x15
 * x14
 * x13
 * x12
 * x11
 * pvParameters
 * x9
 * x8
 * x7
 * x6
 * x5
 * portTASK_RETURN_ADDRESS
 * [FPU registers (when enabled/available) go here]
 * [VPU registers (when enabled/available) go here]
 * mstatus
 * [chip specific registers go here]
 * pxCode
 */
pxPortInitialiseStack:
    addi a0, a0, -portCONTEXT_SIZE
    store_x a1, 0(a0)
    
    addi t0, x0, 0x188
    slli t0, t0, 4
    store_x t0, 1 * portWORD_SIZE(a0)

    load_x t0, xTaskReturnAddress
    store_x t0, 2 * portWORD_SIZE(a0)

    store_x a2, 8 * portWORD_SIZE(a0)

    store_x x0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE(a0)

    ret
/*-----------------------------------------------------------*/

xPortStartFirstTask:
    load_x  sp, pxCurrentTCB            /* Load pxCurrentTCB. */
    load_x  sp, 0( sp )                 /* Read sp from first TCB member. */

    load_x  x1, 0( sp ) /* Note for starting the scheduler the exception return address is used as the function return address. */

    load_x  x5, 1 * portWORD_SIZE( sp ) /* Initial mstatus into x5 (t0). */
    addi    x5, x5, 0x08                /* Set MIE bit so the first task starts with interrupts enabled - required as returns with ret not eret. */
    csrw    mstatus, x5                 /* Interrupts enabled from here! */

    load_x  x7,  5  * portWORD_SIZE( sp )   /* t2 */
    load_x  x8,  6  * portWORD_SIZE( sp )   /* s0/fp */
    load_x  x9,  7  * portWORD_SIZE( sp )   /* s1 */
    load_x  x10, 8  * portWORD_SIZE( sp )   /* a0 */
    load_x  x11, 9  * portWORD_SIZE( sp )   /* a1 */
    load_x  x12, 10 * portWORD_SIZE( sp )   /* a2 */
    load_x  x13, 11 * portWORD_SIZE( sp )   /* a3 */
    load_x  x14, 12 * portWORD_SIZE( sp )   /* a4 */
    load_x  x15, 13 * portWORD_SIZE( sp )   /* a5 */
    load_x  x16, 14 * portWORD_SIZE( sp )   /* a6 */
    load_x  x17, 15 * portWORD_SIZE( sp )   /* a7 */
    load_x  x18, 16 * portWORD_SIZE( sp )   /* s2 */
    load_x  x19, 17 * portWORD_SIZE( sp )   /* s3 */
    load_x  x20, 18 * portWORD_SIZE( sp )   /* s4 */
    load_x  x21, 19 * portWORD_SIZE( sp )   /* s5 */
    load_x  x22, 20 * portWORD_SIZE( sp )   /* s6 */
    load_x  x23, 21 * portWORD_SIZE( sp )   /* s7 */
    load_x  x24, 22 * portWORD_SIZE( sp )   /* s8 */
    load_x  x25, 23 * portWORD_SIZE( sp )   /* s9 */
    load_x  x26, 24 * portWORD_SIZE( sp )   /* s10 */
    load_x  x27, 25 * portWORD_SIZE( sp )   /* s11 */
    load_x  x28, 26 * portWORD_SIZE( sp )   /* t3 */
    load_x  x29, 27 * portWORD_SIZE( sp )   /* t4 */
    load_x  x30, 28 * portWORD_SIZE( sp )   /* t5 */
    load_x  x31, 29 * portWORD_SIZE( sp )   /* t6 */

    load_x  x5, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( sp )    /* Obtain xCriticalNesting value for this task from task's stack. */
    load_x  x6, pxCriticalNesting           /* Load the address of xCriticalNesting into x6. */
    store_x x5, 0( x6 )                     /* Restore the critical nesting value for this task. */

    load_x  x5, 3 * portWORD_SIZE( sp )     /* Initial x5 (t0) value. */
    load_x  x6, 4 * portWORD_SIZE( sp )     /* Initial x6 (t1) value. */

    addi    sp, sp, portCONTEXT_SIZE
    ret
/*-----------------------------------------------------------*/

freertos_risc_v_application_exception_handler:
    csrr t0, mcause     /* For viewing in the debugger only. */
    csrr t1, mepc       /* For viewing in the debugger only */
    csrr t2, mstatus    /* For viewing in the debugger only */
    j .
/*-----------------------------------------------------------*/

freertos_risc_v_application_interrupt_handler:
    csrr t0, mcause     /* For viewing in the debugger only. */
    csrr t1, mepc       /* For viewing in the debugger only */
    csrr t2, mstatus    /* For viewing in the debugger only */
    j .
/*-----------------------------------------------------------*/

//.section .text.freertos_risc_v_exception_handler
freertos_risc_v_exception_handler:
    portcontextSAVE_EXCEPTION_CONTEXT
    /* a0 now contains mcause. */
    li t0, 11                           /* 11 == environment call. */
    bne a0, t0, other_exception         /* Not an M environment call, so some other exception. */
    call vTaskSwitchContext
    portcontextRESTORE_CONTEXT

other_exception:
    call freertos_risc_v_application_exception_handler
    portcontextRESTORE_CONTEXT
/*-----------------------------------------------------------*/

//.section .text.freertos_risc_v_interrupt_handler
freertos_risc_v_interrupt_handler:
    portcontextSAVE_INTERRUPT_CONTEXT
    call freertos_risc_v_application_interrupt_handler
    portcontextRESTORE_CONTEXT
/*-----------------------------------------------------------*/

//.section .text.freertos_risc_v_mtimer_interrupt_handler
freertos_risc_v_mtimer_interrupt_handler:
    portcontextSAVE_INTERRUPT_CONTEXT
    portUPDATE_MTIMER_COMPARE_REGISTER
    call xTaskIncrementTick
    beqz a0, exit_without_context_switch    /* Don't switch context if incrementing tick didn't unblock a task. */
    call vTaskSwitchContext
exit_without_context_switch:
    portcontextRESTORE_CONTEXT
/*-----------------------------------------------------------*/

//.section .text.freertos_risc_v_trap_handler
.align 8
freertos_risc_v_trap_handler:
    portcontextSAVE_CONTEXT_INTERNAL

    csrr a0, mcause
    csrr a1, mepc

    bge a0, x0, synchronous_exception

asynchronous_interrupt:
    store_x a1, 0( sp )                 /* Asynchronous interrupt so save unmodified exception return address. */
    load_x sp, xISRStackTop             /* Switch to ISR stack. */
    j handle_interrupt

synchronous_exception:
    addi a1, a1, 4                      /* Synchronous so update exception return address to the instruction after the instruction that generated the exeption. */
    store_x a1, 0( sp )                 /* Save updated exception return address. */
    load_x sp, xISRStackTop             /* Switch to ISR stack. */
    j handle_exception

handle_interrupt:

    test_if_mtimer:                     /* If there is a CLINT then the mtimer is used to generate the tick interrupt. */
        addi t0, x0, 1
        slli t0, t0, __riscv_xlen - 1   /* LSB is already set, shift into MSB.  Shift 31 on 32-bit or 63 on 64-bit cores. */
        addi t1, t0, 1                  /* 0x8000[]0001 == SYSTIMER interrupt. */
        bne a0, t1, application_interrupt_handler

        CLEAR_SYSTIMER_INTERRUPT        /* Clear the SYSTIMER interrupt to avoid the timer interrupt loop*/

        call xTaskIncrementTick
        beqz a0, processed_source       /* Don't switch context if incrementing tick didn't unblock a task. */
        call vTaskSwitchContext
        j processed_source

application_interrupt_handler:
    call freertos_risc_v_application_interrupt_handler
    j processed_source

handle_exception:
    /* a0 contains mcause. */
    li t0, 11                                   /* 11 == environment call. */
    bne a0, t0, application_exception_handler   /* Not an M environment call, so some other exception. */
    call vTaskSwitchContext
    j processed_source

application_exception_handler:
    call freertos_risc_v_application_exception_handler
    j processed_source                  /* No other exceptions handled yet. */

processed_source:
    portcontextRESTORE_CONTEXT
/*-----------------------------------------------------------*/
