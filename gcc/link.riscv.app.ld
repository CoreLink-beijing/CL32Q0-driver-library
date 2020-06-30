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
 * 2019-08-08     corelink  	*         
 */


/* required to correctly link newlib */
GROUP( -lc -lm -lgloss -lgcc -lsupc++ )

/* Entry Point */
OUTPUT_ARCH( "riscv" )

MEMORY
{
    m_text		: ORIGIN = 0x00003000, LENGTH = 0x5000
	m_data		: ORIGIN = 0x00100000, LENGTH = 0x4000
}

/* 
 * Stack information variables
 * 2K - minimum stack space to reserve 
 */
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0800;	

/* align each sector to word boundaries. */

SECTIONS
{
    .vectors :
    {
        . = ALIGN(4);
        KEEP(*(.vectors))
    } > m_text

    .text : 
	{
        . = ALIGN(4);
        _stext = .;
        *(.text)
        _etext  =  .;
		*(.rodata);
        *(.rodata.*)
        __CTOR_LIST__ = .;
        LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)
        *(.ctors)
        LONG(0)
        __CTOR_END__ = .;
        __DTOR_LIST__ = .;
        LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)
        *(.dtors)
        LONG(0)
        __DTOR_END__ = .;
        *(.lit)
        *(.shdata)
		
		/* section information for shell */
		. = ALIGN(4);
		__fsymtab_start = .;
		KEEP(*(FSymTab))
		__fsymtab_end = .;
		. = ALIGN(4);
		__vsymtab_start = .;
		KEEP(*(VSymTab))
		__vsymtab_end = .;
		. = ALIGN(4);
		
		/* section information for initial. */
		. = ALIGN(4);
		__rt_init_start = .;
		KEEP(*(SORT(.rti_fn*)))
		__rt_init_end = .;
		. = ALIGN(4);
		
        _endtext = .;
    }  > m_text

    .preinit_array :
    {
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP (*(.preinit_array))
      PROVIDE_HIDDEN (__preinit_array_end = .);
    } > m_text

    .init_array :
    {
      PROVIDE_HIDDEN (__init_array_start = .);
      KEEP (*(SORT(.init_array.*)))
      KEEP (*(.init_array ))
      PROVIDE_HIDDEN (__init_array_end = .);
    } > m_text

    .fini_array :
    {
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP (*(SORT(.fini_array.*)))
      KEEP (*(.fini_array ))
      PROVIDE_HIDDEN (__fini_array_end = .);
    } > m_text
	
	__etext = .;    			/* define a global symbol at end of code */
	__global_pointer = .;   	/* define a global symbol at end of code */
	__DATA_ROM = .; 			/* Symbol is used by startup for data initialization */

    .data : AT(__DATA_ROM)
	{
        . = ALIGN(4);
		__DATA_RAM = .;
		__data_start__ = .;     /* create a global symbol at data start */
		sdata  =  .;
        *(.data);
        *(.data.*)
        edata  =  .;
		. = ALIGN(4);
		__data_end__ = .;       /* define a global symbol at data end */
    } > m_data
	
    /* ensure there is enough room for stack */
	.stack (NOLOAD):
	{
		. = ALIGN(8);
		__StackLimit = .;
		. += STACK_SIZE;
		__StackTop = .;
		__rt_thread_irq_stack_top = .;
		__freertos_irq_stack_top = .;
	} > m_data
	
	/* Initializes stack on the end of block */
	PROVIDE(__stack = __StackTop);

    __bss_start = .;
	.bss :
    {
        . = ALIGN(4);
		__START_BSS = .;
		__bss_start__ = .;
        *(.bss)
        *(.bss.*)
        *(.sbss)
        *(.sbss.*)
        *(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
		__END_BSS = .;
    } > m_data
	__bss_end = .;
}

