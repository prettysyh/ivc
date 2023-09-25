
extern unsigned int get_reg ();
#include "aaa_print.h"
#include "ivc_echo.h"
#include "gic.h"
#include "aaa_timer.h"

void test()
{
	ivc_echo();
}

void el1_entry() {
	// print_char("enter el1...\r\n");
	
	unsigned long* bss_start,bss_end;
	unsigned long zero=0U;
	asm(
	"ldr %[sa], =bss_start\n\t"
	: [sa] "=r" (bss_start)
	:
	);
	// print_char("bss_start :");
	// print_reg((unsigned long )bss_start);
	asm(
	"ldr %[sa], =bss_end\n\t"
	: [sa] "=r" (bss_end)
	:
	);
	// print_char("bss_end :");
	// print_reg((unsigned long )bss_end);
	do
	{
		*(bss_start++)=zero;
	}
	while((unsigned long)bss_start!=(unsigned long)bss_end);
	// print_char("current el is :");
	// print_reg(get_reg());
	// for(int i=0;i<100000;i++)
	// {}
	// emmc_test();
	// while (1)
	// {
	// 	/* code */
	// }
	

	// print_char("testing_task\r\n");
	// print_char("initiallising gic...\r\n");
	gic_timer_init();
	
	// while (1)
	// {
	// 	/* code */
	// }
	// for(int i = 0; i < 100000; i ++ ) {}
	// print_char("current el is :");
	// print_reg(get_reg());
	// print_char("current el is :");
	// print_reg(get_reg());
	// set_timer(3000);
	test();
}

void el0_entry()
{
	print_char("enter el0!\r\n");
	
	//task_test();
	unsigned long mode=1;
	ivc_echo();
	while(1)
	{

	}
	print_char("finish\r\n");
}