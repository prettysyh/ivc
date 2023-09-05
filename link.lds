ENTRY(_Reset)
SECTIONS
{
. = 0x80100000;
.text : { 
    *(.text) 
    . = ALIGN(4K);
    *(.vectors)
    }
.data : { *(.data) }
bss_start=.;
.bss : { 
    bss_start = .;
    *(.bss COMMON) }
bss_end=.;
. = ALIGN(8);
. = . + 0x1000; /* 4kB of stack memory */
stack_top = .;
. = ALIGN(8);
. = . + 0x1000; 
stack_top_a = .;
. = ALIGN(8);
. = . + 0x1000; 
stack_top_b = .;
. = ALIGN(8);
. = . + 0x1000; 
stack_top_el2 = .;
. = ALIGN(8);
. = . + 0x1000; 
stack_top_el1 = .;
. = ALIGN(8);
. = . + 0x1000; 
stack_top_el0 = .;
}
