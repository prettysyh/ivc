#include "gic.h"
#include "aaa_type.h"

#define GIC_V2_DISTRIBUTOR_PPTR 0x03881000 
volatile struct gic_dist_map *const gic_dist =
    (volatile struct gic_dist_map *)(GIC_V2_DISTRIBUTOR_PPTR);

#define GIC_V2_CONTROLLER_PPTR  0x03882000
volatile struct gic_cpu_iface_map *const gic_cpuiface =
    (volatile struct gic_cpu_iface_map *)(GIC_V2_CONTROLLER_PPTR);


#define IRQ_SET_ALL 0xffffffff

void gic_timer_init()
{

    uint32_t temp;
    gic_dist->enable_clr[0] = IRQ_SET_ALL;
    gic_dist->pending_clr[0] = IRQ_SET_ALL;
    temp=IRQ_SET_ALL;
    gic_dist->enable_set[0]=temp;
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