volatile unsigned int * const UART0DR = (unsigned int *)0xc280000;//tx2 is 0x3100000
volatile unsigned int * const ULSR = (unsigned int *)0xc280014;//offset 14
#define UTHR 0x00 /* UART Transmit Holding Register */
#define ULSR_THRE (1 << 5) /* Transmit Holding Register Empty */

void print_c(const char s) {
	*UART0DR = (unsigned int)s;
}

void print_char(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
		while ((*ULSR & ULSR_THRE) == 0);
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}

void print_reg(unsigned long temp)
{
	char str[18];
	str[0]='0';
	str[1]='x';

	int count;
	for (count = 17; count >=2; count--)
	{
		unsigned int num = temp % 16;
		char number;
		if(num<10)
		{
			number = num + '0';
		}
		else
		{
			number =(num-10)+'a';
		}
		str[count] = (char)number;
		temp = temp / 16;
	}
	for (count = 0; count <= 17; count++)
	{
		while ((*ULSR & ULSR_THRE) == 0);
		*UART0DR = (unsigned int)(str[count]);
	}
	print_char("\r\n");
}
void print_normal_reg()
{
	unsigned long lr,sp;
	asm(
	"mov %[lr], x30\n\t"
	: [lr] "=r" (lr)
	:
	);
	asm(
	"mov %[sp], sp\n\t"
	: [sp] "=r" (sp)
	:
	);
	print_reg(lr);
	print_reg(sp);
}
void print_all()
{
	unsigned long all;
	asm(
	"mov %[reg], x0\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x0: ");
	print_reg(all);
	asm(
	"mov %[reg], x1\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x1: ");
	print_reg(all);

	asm(
	"mov %[reg], x2\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x2: ");
	print_reg(all);
	asm(
	"mov %[reg], x3\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x3: ");
	print_reg(all);
	asm(
	"mov %[reg], x4\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x4: ");
	print_reg(all);
	asm(
	"mov %[reg], x5\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x5: ");
	print_reg(all);
	asm(
	"mov %[reg], x6\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x6: ");
	print_reg(all);
	asm(
	"mov %[reg], x7\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x7: ");
	print_reg(all);
	asm(
	"mov %[reg], x8\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x8: ");
	print_reg(all);
	asm(
	"mov %[reg], x9\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x9: ");
	print_reg(all);
	asm(
	"mov %[reg], x10\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x10: ");
	print_reg(all);
		asm(
	"mov %[reg], x11\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x11: ");
	print_reg(all);

	asm(
	"mov %[reg], x12\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x12: ");
	print_reg(all);
	asm(
	"mov %[reg], x13\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x13: ");
	print_reg(all);
	asm(
	"mov %[reg], x14\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x14: ");
	print_reg(all);
	asm(
	"mov %[reg], x15\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x15: ");
	print_reg(all);
	asm(
	"mov %[reg], x16\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x16: ");
	print_reg(all);
	asm(
	"mov %[reg], x17\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x17: ");
	print_reg(all);
	asm(
	"mov %[reg], x18\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x18: ");
	print_reg(all);
	asm(
	"mov %[reg], x19\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x19: ");
	print_reg(all);
	asm(
	"mov %[reg], x20\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x20: ");
	print_reg(all);
	asm(
	"mov %[reg], x21\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x21: ");
	print_reg(all);

	asm(
	"mov %[reg], x22\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x22: ");
	print_reg(all);
	asm(
	"mov %[reg], x23\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x23: ");
	print_reg(all);
	asm(
	"mov %[reg], x24\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x24: ");
	print_reg(all);
	asm(
	"mov %[reg], x25\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x25: ");
	print_reg(all);
	asm(
	"mov %[reg], x26\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x26: ");
	print_reg(all);
	asm(
	"mov %[reg], x27\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x27: ");
	print_reg(all);
	asm(
	"mov %[reg], x28\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x28: ");
	print_reg(all);
	asm(
	"mov %[reg], x29\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x29: ");
	print_reg(all);
	asm(
	"mov %[reg], x30\n\t"
	: [reg] "=r" (all)
	:
	);
	print_char("x30: ");
	print_reg(all);
}