#include "aaa_print.h"
#include "ivc_echo.h"
#include "aaa_type.h"
#include "aaa_timer.h"
#include "hsp.h"

#define barrier() asm volatile("" ::: "memory")
#define mb()	__asm__ __volatile__("mb": : :"memory")
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#define smp_mb()	mb()

static inline void ivc_rmb(void)
{
	smp_rmb();
}

static inline void ivc_wmb(void)
{
	smp_wmb();
}

static inline void ivc_mb(void)
{
	smp_mb();
}

void ivc_notify_remote() {
    tegra_hsp_ss_set(3, 0x1);
    tegra_hsp_sm_writel(3, SMBOX_IVC_NOTIFY | HSP_SHRD_MBOX_TAG_FIELD);
}


void msg_rx() {
    char* ptr;
    while (A_READ_COUNT < R_WRITE_COUNT) {
		ptr = (char*)(WRITE_BUFFER_BASE + A_READ_COUNT % nframes * frame_size);
        print_char(ptr);
        A_READ_COUNT ++ ;
    }
}

void msg_tx() {
    int bytes = 10;
    char buffer[10] = {'W', 'e', 'l', 'c', 'o', 'm', 'e', 't', 'o', '\0'};
    char *ptr = buffer;
    int total_frame;
    if (bytes % frame_size) {
        total_frame = bytes / frame_size + 1;
    } else {
        total_frame = bytes / frame_size;
    }

    while (A_WRITE_COUNT + total_frame - R_READ_COUNT > nframes);

    char *p = (char*)(READ_BUFFER_BASE + A_WRITE_COUNT % nframes * frame_size);

    // print_char("msg is transmitting\r\n");
    for (int i = 0; i < total_frame; i ++ ) {
        for (int j = 0; j < frame_size; j ++ ) {
            if ( *ptr != '\0') {
				*p = *ptr;
            	ptr ++ ;
            	p ++ ;
			} else {
				*p = 0;
				p ++ ;
			}
        }
        A_WRITE_COUNT ++ ;
    }
}

void ivc_echo() {
    // print_char("now we will start!\r\n");
    // print_hsp_register_number();
	ivc_wmb();
    tegra_hsp_ss_set(0, ivc_carveout_base_ss);
    tegra_hsp_ss_set(1, ivc_carveout_size_ss);
    ivc_wmb();
    tegra_hsp_sm_full_enable(2);
    
    // print_char("tegra_hsp_sm_writel1\r\n");
	ivc_wmb();
    tegra_hsp_sm_writel(3, SMBOX_IVC_READY_MSG | HSP_SHRD_MBOX_TAG_FIELD);
	ivc_wmb();
	for (int i = 0; i < ivc_carveout_size_ss; i ++ ) {
		*((char*)Buffer + i) = 0x0;
	}

	ivc_wmb();
    msg_tx();
	ivc_wmb();
    ivc_notify_remote();
	set_timer(3000);
	// int cnt = 0;
    while (1) {
		// uint32_t addr = NV_ADDRESS_MAP_AON_HSP_BASE + HSP_COMMON_SIZE + HSP_SHRD_MBOX_OFFSET * 2;
		// if (*(volatile unsigned int *)addr == 0x8000AABB) {
		// 	printf("ok\r\n");
		// } else {
		// 	printf("%u\r\n", *(volatile unsigned int *)addr);
		// }
        // if (A_READ_COUNT < R_WRITE_COUNT)
        //     msg_rx();
        // msg_tx();
		// if (cnt % 1000000 == 0) {
		// 	print_char("cnt: ");
		// 	print_reg(cnt);
		// }
		// cnt ++ ;
    }
}