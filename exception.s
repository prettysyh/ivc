.macro ventry label
.align 7
    b       \label
.endm

.section .vectors

.global arm_vector_table
arm_vector_table:
    ventry  cur_el_sync                     // Synchronous EL1t/EL2t
    ventry  cur_irq_handler           // IRQ EL1t/EL2t
    ventry  invalid_vector_entry           // FIQ EL1t/EL2t
    ventry  invalid_vector_entry           // SError EL1t/EL2t

    ventry  cur_el_sync                    // Current EL Synchronous (EL1/2)
    ventry  cur_irq_handler                    // IRQ
    ventry  invalid_vector_entry           // FIQ
    ventry  invalid_vector_entry                    // SError

    ventry  lower_el_sync                  // Synchronous 64-bit EL0/EL1
    ventry  low_irq_handler                   // IRQ 64-bit EL0/EL1
    ventry  invalid_vector_entry           // FIQ 64-bit EL0/EL1
    ventry  invalid_vector_entry                  // SError 64-bit EL0/EL1

    ventry  lower_el_sync           // Synchronous 32-bit EL0/EL1
    ventry  cur_irq_handler           // IRQ 32-bit EL0/EL1
    ventry  invalid_vector_entry           // FIQ 32-bit EL0/EL1
    ventry  invalid_vector_entry           // SError 32-bit EL0/EL1

.macro irq_enter
    STP		X0, X1, [SP, #-0x10]!
	STP		X2, X3, [SP, #-0x10]!
	STP		X4, X5, [SP, #-0x10]!
	STP		X6, X7, [SP, #-0x10]!
	STP		X8, X9, [SP, #-0x10]!
    STP		X10, X11, [SP, #-0x10]!
	STP		X12, X13, [SP, #-0x10]!
	STP		X14, X15, [SP, #-0x10]!
	STP		X16, X17, [SP, #-0x10]!
	STP		X18, X19, [SP, #-0x10]!
    STP		X20, X21, [SP, #-0x10]!
	STP		X22, X23, [SP, #-0x10]!
	STP		X24, X25, [SP, #-0x10]!
	STP		X26, X27, [SP, #-0x10]!
	STP		X28, X29, [SP, #-0x10]!
    STP		X30, XZR, [SP, #-0x10]!
    MRS		X3, SPSR_EL1
	MRS		X2, ELR_EL1
	STP 	X2, X3, [SP, #-0x10]!
.endm


.macro irq_exit
    LDP 	X4, X5, [SP], #0x10  
	MSR		SPSR_EL1, X5
	MSR		ELR_EL1, X4
    LDP		X30, X1, [SP], #0x10
    LDP		X28, X29, [SP], #0x10
	LDP		X26, X27, [SP], #0x10
	LDP		X24, X25, [SP], #0x10
	LDP		X22, X23, [SP], #0x10
	LDP		X20, X21, [SP], #0x10
	LDP		X18, X19, [SP], #0x10
	LDP		X16, X17, [SP], #0x10
	LDP		X14, X15, [SP], #0x10
	LDP		X12, X13, [SP], #0x10
	LDP		X10, X11, [SP], #0x10
	LDP		X8, X9, [SP], #0x10
	LDP		X6, X7, [SP], #0x10
	LDP		X4, X5, [SP], #0x10
	LDP		X2, X3, [SP], #0x10
	LDP		X0, X1, [SP], #0x10
.endm

.global invalid_vector_entry
invalid_vector_entry:
    bl hal
    b .

.global cur_el_sync
cur_el_sync:
    msr spsel, #0
    bl cur_syn
    eret

.global cur_irq_handler
cur_irq_handler:
    irq_enter
    bl irq_hal_c
    irq_exit
    eret
    b .

.global low_irq_handler
low_irq_handler:
    irq_enter
    bl irq_hal_c
    irq_exit
    eret

.global lower_el_sync
lower_el_sync:
    eret