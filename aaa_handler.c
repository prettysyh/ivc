#include "aaa_print.h"
#include "gic.h"
#include "aaa_timer.h"
#include "aaa_type.h"
#include "hsp.h"
#include "ivc_echo.h"

extern unsigned long get_reg ();
extern unsigned long get_spsel ();

void hal()
{
	print_char("is in halt\r\n");
    print_char("currentel is :");
    print_reg(get_reg());
    print_char("sydrome :");
    unsigned int sydrom;
            asm(
	"mrs %[spsr], esr_el1\n\t"
	: [spsr] "=r" (sydrom)
	:
	);
    print_reg(sydrom);
                asm(
	"mrs %[spsr], elr_el1\n\t"
	: [spsr] "=r" (sydrom)
	:
	);
    print_char("return :");
    print_reg(sydrom);
                    asm(
	"mov %[spsr], x30\n\t"
	: [spsr] "=r" (sydrom)
	:
	);
    print_char("lr return :");
    print_reg(sydrom);
}

void lower_syn()
{
    print_char("lower_sync\r\n");
    
    unsigned int id;
    asm ("mrs %0, esr_el1" : "=r" (id));
    print_char("syd is : ");
    print_reg(id);
}

void cur_syn()
{
    print_char("curr_sync\r\n");
    unsigned int id;
    asm ("mrs %0, esr_el1" : "=r" (id));
    print_char("syd is : ");
    print_reg(id);
}

void irq_hal_c()
{
	print_char("current irq handling\r\n");
	unsigned long temp= get_iar();
    print_char("iar: ");
    print_reg(temp);

    if(temp<32)
    {
        // ivc_notify_remote();
        uint32_t data = tegra_hsp_sm_readl(2);          /* 读取ivc存入2号sm中的data */
        tegra_hsp_sm_writel(2, 0);
        print_char("tegra_hsp_sm_readl(2): ");
        print_reg(data);

        uint32_t ss_val;
        tegra_hsp_ss_read(2, &ss_val);
        tegra_hsp_ss_clear(2, ss_val);

        if (data == (uint32_t)SMBOX_IVC_NOTIFY | HSP_SHRD_MBOX_TAG_FIELD) {         /* 若data为0x8000AABB，则为ivc通知ccplex该读/写 */
            if (R_WRITE_COUNT > A_READ_COUNT) {        /* 读 */
                msg_rx();
            }

            if (A_WRITE_COUNT == R_READ_COUNT) {        /* 写 */
                msg_tx();
            }
        }

        tegra_hsp_ss_clear(2, 0);       /* 清2号ss的bit0 */
        set_timer(3000);
    }
    else {
        uint32_t data = tegra_hsp_sm_readl(2);          /* 读取ivc存入2号sm中的data */
        tegra_hsp_sm_writel(2, 0);
        print_char("tegra_hsp_sm_readl(2): ");
        print_reg(data);

        uint32_t ss_val;
        tegra_hsp_ss_read(2, &ss_val);
        tegra_hsp_ss_clear(2, ss_val);

        if (data == (uint32_t)SMBOX_IVC_NOTIFY | HSP_SHRD_MBOX_TAG_FIELD) {         /* 若data为0x8000AABB，则为ivc通知ccplex该读/写 */
            if (R_WRITE_COUNT > A_READ_COUNT) {        /* 读 */
                msg_rx();
            }

            if (A_WRITE_COUNT == R_READ_COUNT) {        /* 写 */
                msg_tx();
            }
        }

        tegra_hsp_ss_clear(2, 0);       /* 清2号ss的bit0 */
    } // 133 ~ 136

    write_eoi(temp);
}

// void fiq_hal_c()
// {
// 	print_char("current irq handling\r\n");
// 	unsigned long temp= get_iar();
//     print_char("iar: ");
//     print_reg(temp);

//     uint32_t data = tegra_hsp_sm_readl(2);          /* 读取ivc存入2号sm中的data */
//     tegra_hsp_sm_writel(2, 0);
//     print_char("tegra_hsp_sm_readl(2): %u");

//     uint32_t ss_val;
//     tegra_hsp_ss_read(2, &ss_val);
//     tegra_hsp_ss_clear(2, ss_val);

//     if (data == (uint32_t)SMBOX_IVC_NOTIFY | HSP_SHRD_MBOX_TAG_FIELD) {         /* 若data为0x8000AABB，则为ivc通知ccplex该读/写 */
//         if (R_WRITE_COUNT > A_READ_COUNT) {        /* 读 */
//             msg_rx();
//         }

//         if (A_WRITE_COUNT == R_READ_COUNT) {        /* 写 */
//             msg_tx();
//         }
//     }

//     tegra_hsp_ss_clear(2, 0);       /* 清2号ss的bit0 */

//     write_eoi(temp);
// }
