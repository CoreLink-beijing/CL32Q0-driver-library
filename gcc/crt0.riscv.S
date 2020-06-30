/* Copyright (c) 2019-2020, corelink inc., www.corelink.vip
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-12     corelink		*
 */


#ifdef __FreeRTOS__
	#define sys_irq_handler 	freertos_risc_v_trap_handler
	#define sys_ecall_handler 	freertos_risc_v_trap_handler
	#define c_entry				main
#else
	#ifdef __RT_Thread__
		.extern IRQ_Handler
		#define sys_irq_handler 	IRQ_Handler
		#define sys_ecall_handler 	IRQ_Handler
		#define c_entry				rtthread_startup
	#else
		#define sys_irq_handler 	irq_handler
		#define sys_ecall_handler 	ecall_insn_handler
		#define c_entry				main
	#endif
#endif


#define EXCEPTION_STACK_SIZE 	0x58


	.section .text

reset_handler:
	# Disable global interrupt.
    csrci mstatus, 8

	# set 0 in mtvec (base for IVT)
	csrrw x0, mtvec, x0

	# set all registers to zero
	mv  x1, x0
	mv  x2, x1
	mv  x3, x1
	mv  x4, x1
	mv  x5, x1
	mv  x6, x1
	mv  x7, x1
	mv  x8, x1
	mv  x9, x1
	mv x10, x1
	mv x11, x1
	mv x12, x1
	mv x13, x1
	mv x14, x1
	mv x15, x1
	mv x16, x1
	mv x17, x1
	mv x18, x1
	mv x19, x1
	mv x20, x1
	mv x21, x1
	mv x22, x1
	mv x23, x1
	mv x24, x1
	mv x25, x1
	mv x26, x1
	mv x27, x1
	mv x28, x1
	mv x29, x1
	mv x30, x1
	mv x31, x1

	# initialize stack pointer
    la sp, __StackTop

    # initialize global pointer
    la gp, __global_pointer

	jal system_init;

	# Run global initialization functions
	call __libc_init_array

main_entry:
	# Enable global interrupt .
    csrsi mstatus, 8

	# jump to main program entry
	jal c_entry
	mv a0, s0

	# *if program exits call exit routine from library
	jal x1, exit
  
irq_handler:
    addi x2, x2, -EXCEPTION_STACK_SIZE
    sw x1, 0x54(x2)
    jal x1, store_regs
    la x1, end_except
    csrr a0, mcause
    jal x0, system_irq_handler
    .size irq_handler, . - irq_handler

illegal_insn_handler:
	addi x2, x2, -EXCEPTION_STACK_SIZE
	sw x1, 0x5C(x2)
	jal x1, store_regs
	la x1, end_except
	jal x0, illegal_insn_handler_c

ecall_insn_handler:
	addi x2, x2, -EXCEPTION_STACK_SIZE
	sw x1, 0x5C(x2)
	jal x1, store_regs
	la x1, end_except
	jal x0, ecall_insn_handler_c

// saves all caller-saved registers (except return address)
store_regs:
    sw  x3, 0x00(x2)  // gp
    sw  x4, 0x04(x2)  // tp
    sw  x5, 0x08(x2)  // t0
    sw  x6, 0x0c(x2)  // t1
    sw  x7, 0x10(x2)  // t2
    sw x10, 0x14(x2)  // a0
    sw x11, 0x18(x2)  // a1
    sw x12, 0x1c(x2)  // a2
    sw x13, 0x20(x2)  // a3
    sw x14, 0x24(x2)  // a4
    sw x15, 0x28(x2)  // a5
    sw x16, 0x2c(x2)  // a6
    sw x17, 0x30(x2)  // a7

    csrr a0, 0x7B0
    csrr a1, 0x7B1
    csrr a2, 0x7B2
    sw a0, 0x34(x2)  // lpstart[0]
    sw a1, 0x38(x2)  // lpend[0]
    sw a2, 0x3c(x2)  // lpcount[0]
    csrr a0, 0x7B4
    csrr a1, 0x7B5
    csrr a2, 0x7B6
    sw a0, 0x40(x2)  // lpstart[1]
    sw a1, 0x44(x2)  // lpend[1]
    sw a2, 0x48(x2)  // lpcount[1]

    csrr a0, 0x341
    sw a0, 0x4c(x2)  // mepc
    csrr a1, 0x300
    sw a1, 0x50(x2)  // mstatus
    jalr x0, x1

    // load back registers from stack
end_except:
    lw a1, 0x50(x2)  // mstatus
    csrrw x0, 0x300, a1
    lw a0, 0x4c(x2)  // mepc
    csrrw x0, 0x341, a0

    lw a0, 0x40(x2)  // lpstart[1]
    lw a1, 0x44(x2)  // lpend[1]
    lw a2, 0x48(x2)  // lpcount[1]
    csrrw x0, 0x7B4, a0
    csrrw x0, 0x7B5, a1
    csrrw x0, 0x7B6, a2
    lw a0, 0x34(x2)  // lpstart[0]
    lw a1, 0x38(x2)  // lpend[0]
    lw a2, 0x3c(x2)  // lpcount[0]
    csrrw x0, 0x7B0, a0
    csrrw x0, 0x7B1, a1
    csrrw x0, 0x7B2, a2

    lw  x3, 0x00(x2)  // gp
    lw  x4, 0x04(x2)  // tp
    lw  x5, 0x08(x2)  // t0
    lw  x6, 0x0c(x2)  // t1
    lw  x7, 0x10(x2)  // t2
    lw x10, 0x14(x2)  // a0
    lw x11, 0x18(x2)  // a1
    lw x12, 0x1c(x2)  // a2
    lw x13, 0x20(x2)  // a3
    lw x14, 0x24(x2)  // a4
    lw x15, 0x28(x2)  // a5
    lw x16, 0x2c(x2)  // a6
    lw x17, 0x30(x2)  // a7

    lw  x1, 0x54(x2)
    addi x2, x2, EXCEPTION_STACK_SIZE
    mret

	.global _init
	.global _fini
_init:
_fini:
	# These don't have to do anything since we use init_array/fini_array.
	ret

.section .vectors, "ax"
	.option norvc;

	.org 0x00

	// interrupt vector
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler
	jal x0, sys_irq_handler

	// reset vector
	.org 0x80
	jal x0, reset_handler

	// illegal instruction exception
	.org 0x84
	jal x0, illegal_insn_handler

	// ecall handler
	.org 0x88
	jal x0, sys_ecall_handler

