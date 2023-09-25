#include "aaa_print.h"
#include "ivc_echo.h"
#include "aaa_type.h"
#include "aaa_timer.h"
#include "hsp.h"

// #define R_WRITE_COUNT     (*(volatile unsigned int*)(0x80000000))      // freertos 总共发送过来的帧数
// #define A_READ_COUNT      (*(volatile unsigned int*)(0x80000040))      // A核总共读取的帧数（注意：读完更新）
// #define WRITE_BUFFER_BASE 0x80000080             // write_buffer起始地址（读的基地址）

// #define A_WRITE_COUNT     (*(volatile unsigned int*)(0x80010000))      // A核总共发送的帧数（注意：写完更新）
// #define R_READ_COUNT      (*(volatile unsigned int*)(0x80010040))      // freertos 总共读取的帧数
// #define READ_BUFFER_BASE  0x80010080              // read_buffer起始地址（写的基地址）

enum ivc_state {
	/*
	 * 对于假定通道最初归零的服务的向后兼容性，此值为零。此类通道处于初始有效状态，但不能异步重置，并且必须始终保持有效状态。
	 *
	 * 发送端在观察到处于ack或建立状态的接收端点时，可以从sync或ack状态进入established状态，表明已经清除了我们rx_channel中的计数器。
	 */
	ivc_state_established = 0,

	/*
	 * 如果观察到端点处于同步状态，则允许远程端点相对于当前端点异步清除其拥有的计数器。因此，不再允许当前端点进行通信。
	 */
	ivc_state_sync,

	/*
	 * 当发送端观察到接收端处于同步状态时，它可以清除w_count和r_count并转换到ack状态。如果远程端点观察到我们处于ack状态，它可以在清除计数器后返回到既定状态。
	 */
	ivc_state_ack
};

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

int tegra_ivc_channel_notified()
{
	enum ivc_state peer_state;

	peer_state = R_STATE;

	if (peer_state == ivc_state_sync) {
		/*
		 * Order observation of ivc_state_sync before stores clearing
		 * tx_channel.
		 */
		ivc_rmb();

		/*
		 * Reset tx_channel counters. The remote end is in the SYNC
		 * state and won't make progress until we change our state,
		 * so the counters are not in use at this time.
		 */
		// ivc->tx_channel->w_count = 0;
        A_WRITE_COUNT = 0;
		// ivc->rx_channel->r_count = 0;
        A_READ_COUNT = 0;

		/*
		 * Ensure that counters appear cleared before new state can be
		 * observed.
		 */
		ivc_wmb();

		/*
		 * Move to ACK state. We have just cleared our counters, so it
		 * is now safe for the remote end to start using these values.
		 */
		A_STATE = ivc_state_ack;

		/*
		 * Notify remote end to observe state transition.
		 */
		ivc_notify_remote();

	} else if (A_STATE == ivc_state_sync && peer_state == ivc_state_ack) {
		/*
		 * Order observation of ivc_state_sync before stores clearing
		 * tx_channel.
		 */
		ivc_rmb();

		/*
		 * Reset tx_channel counters. The remote end is in the ACK
		 * state and won't make progress until we change our state,
		 * so the counters are not in use at this time.
		 */
		// ivc->tx_channel->w_count = 0;
        A_WRITE_COUNT = 0;
		// ivc->rx_channel->r_count = 0;
        A_READ_COUNT = 0;

		/*
		 * Ensure that counters appear cleared before new state can be
		 * observed.
		 */
		ivc_wmb();

		/*
		 * Move to ESTABLISHED state. We know that the remote end has
		 * already cleared its counters, so it is safe to start
		 * writing/reading on this channel.
		 */
		A_STATE = ivc_state_established;

		/*
		 * Notify remote end to observe state transition.
		 */
		ivc_notify_remote();

	} else if (A_STATE == ivc_state_ack) {
		/*
		 * At this point, we have observed the peer to be in either
		 * the ACK or ESTABLISHED state. Next, order observation of
		 * peer state before storing to tx_channel.
		 */
		ivc_rmb();

		/*
		 * Move to ESTABLISHED state. We know that we have previously
		 * cleared our counters, and we know that the remote end has
		 * cleared its counters, so it is safe to start writing/reading
		 * on this channel.
		 */
		A_STATE = ivc_state_established;

		/*
		 * Notify remote end to observe state transition.
		 */
		ivc_notify_remote();

	} else {
		/*
		 * There is no need to handle any further action. Either the
		 * channel is already fully established, or we are waiting for
		 * the remote end to catch up with our current state. Refer
		 * to the diagram in "IVC State Transition Table" above.
		 */
	}

	return A_STATE == ivc_state_established ? 0 : -1;
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
	
	// tegra_hsp_ss_set(2, (1 << 0));
    // tegra_hsp_sm_writel(2, SMBOX_IVC_NOTIFY | HSP_SHRD_MBOX_TAG_FIELD);

	// A_READ_COUNT = 0;
	// A_WRITE_COUNT = 0;
	// R_WRITE_COUNT = 0;
	// R_READ_COUNT = 0;
	ivc_wmb();
    msg_tx();
    // print_char("read count:");
    // print_reg(A_READ_COUNT);
    // print_char("write count:");
    // print_reg(A_WRITE_COUNT);
	ivc_wmb();
    // print_char("tegra_hsp_sm_writel2\r\n");
    ivc_notify_remote();
	set_timer(3000);
    // print_char("msg_tx...\r\n");
	
	// print_char("====================================================\r\n");
	// uint32_t addr = NV_ADDRESS_MAP_AON_HSP_BASE + HSP_COMMON_SIZE + HSP_SHRD_MBOX_OFFSET * 2;
	// print_reg(*(volatile unsigned int *)addr);
	// print_char("====================================================\r\n");
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