#include "gic.h"
#include "aaa_type.h"

#define GIC_V2_DISTRIBUTOR_PPTR 0x03881000 
volatile struct gic_dist_map *const gic_dist =
    (volatile struct gic_dist_map *)(GIC_V2_DISTRIBUTOR_PPTR);

#define GIC_V2_CONTROLLER_PPTR  0x03882000
volatile struct gic_cpu_iface_map *const gic_cpuiface =
    (volatile struct gic_cpu_iface_map *)(GIC_V2_CONTROLLER_PPTR);


#define IRQ_SET_ALL 0xffffffff

// #define GICD_INT_DEF_PRI		0xa0
// #define GICD_INT_DEF_PRI_X4		((GICD_INT_DEF_PRI << 24) |\
// 					(GICD_INT_DEF_PRI << 16) |\
// 					(GICD_INT_DEF_PRI << 8) |\
// 					GICD_INT_DEF_PRI)

// uint32_t gic_get_cpumask() {
//     uint32_t mask, i;

// 	for (i = mask = 0; i < 8; i ++ ) {
// 		mask = gic_dist->targets[i];
// 		mask |= mask >> 16;
// 		mask |= mask >> 8;
// 		if (mask)
// 			break;
// 	}

// 	return mask;
// }

// static void gic_dist_init()
// {
// 	unsigned int cpumask;
// 	int i;

// 	/* 关闭中断*/
//     gic_dist->enable = 0x0;

// 	/* 设置中断路由：GIC_DIST_TARGET
// 	 *
// 	 * 前32个中断怎么路由是GIC芯片固定的，因此先读GIC_DIST_TARGET前面的值
// 	 * 然后全部填充到 SPI的中断号 */
// 	// cpumask = gic_get_cpumask();
// 	// cpumask |= cpumask << 8;
// 	// cpumask |= cpumask << 16;

// 	// for (i = 8; i < 256; i ++ )
//     //     gic_dist->targets[i] = cpumask; 

// 	// /* Set all global interrupts to be level triggered, active low */
// 	// for (int i = 8; i < 64; i ++ ) {
//     //     gic_dist->config[i] = 0x0;          
//     // }

// 	/* Deactivate and disable all 中断（SGI， PPI， SPI）.
// 	 *
// 	 * 当注册中断的时候才 enable某个一个SPI中断，例如调用gic_unmask_irq()*/
// 	// for (int i = 4; i < 32; i ++ ) {
//     //     gic_dist->res2[i] = 0xffffffff;
//     //     gic_dist->enable_clr[i] = 0xffffffff;
// 	// }

// 	/* 打开中断：Enable group0 and group1 interrupt forwarding.*/
//     gic_dist->enable = 0x1;
// }

// void gic_cpu_init() {
//     int i;
//     // for (i = 0; i < 32; i += 4)
// 	// 	// writel_relaxed(GICD_INT_DEF_PRI_X4, base + GIC_DIST_PRI + i * 4 / 4);
//     //     gic_dist->priority[i] = (0x30 << 24 | 0x30 << 16 | 0x30 << 8 | 0x30);
    
//     // gic_cpuiface->pri_msk_c = 0x80;
// 	gic_cpuiface->icontrol = 1;
// }

static void gic_poke_irq(uint32_t irq)
{
	uint32_t mask = 1 << (irq % 32);
	gic_dist->enable_set[irq / 32] = mask;
}


void gic_timer_init()
{

    uint32_t temp;
    // gic_dist_init();
    // gic_cpu_init();

    gic_dist->enable_clr[0] = IRQ_SET_ALL;
    gic_dist->pending_clr[0] = IRQ_SET_ALL;
    temp=IRQ_SET_ALL;
    gic_dist->enable_set[0]=temp;

    for (int i = 133; i < 137; i ++ )
        gic_poke_irq(32 + i);

    gic_cpuiface->icontrol = 1;
    gic_dist->enable = 1;
}

unsigned int get_dist_state()
{
    return gic_dist->enable;
} 
unsigned int get_cpu_state()
{
    return gic_cpuiface->icontrol;
}
unsigned int get_irq()
{
    return gic_dist->pending_set[0];
}
unsigned int get_iar()
{
    return gic_cpuiface->int_ack;
}
unsigned int get_enable_set()
{
    return gic_dist->enable_set[0];
}

void write_eoi(unsigned int temp)
{
    gic_cpuiface->eoi=temp;
}