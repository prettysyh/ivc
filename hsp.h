#ifndef _HSP_H_
#define _HSP_H_

#include "aaa_type.h"

#define NV_ADDRESS_MAP_AON_HSP_BASE					202702848           /* 0x0C150000 */
#define NV_ADDRESS_MAP_TOP0_HSP_COMMON_SIZE         65536               
#define HSP_COMMON_SIZE (NV_ADDRESS_MAP_TOP0_HSP_COMMON_SIZE)
#define HSP_SHRD_MBOX_OFFSET                        0x8000
#define HSP_INT_IE                                  0x100
#define HSP_INT_IE_OFFSET                           0x4
#define HSP_INT_IE_0_mbox_full_enable_SHIFT         0x8
#define HSP_SHRD_SEM_0_SHRD_SMP_STA_CLR_0           0x8
#define HSP_SHRD_SEM_CLR	HSP_SHRD_SEM_0_SHRD_SMP_STA_CLR_0
#define HSP_SHRD_SEM_OFFSET                         0x10000

#define SMBOX_IVC_READY_MSG	                        0x2AAA5555
#define	SMBOX_IVC_NOTIFY	                        0x0000AABB
#define HSP_SHRD_MBOX_TAG_FIELD                     0x1 << 31
#define HSP_SHRD_SEM_SET                            0x4
#define HSP_SHRD_SEM_STA	                        0x0

#define MBOX_FULL_IE_OFFSET                         0x4

void tegra_hsp_sm_writel(uint32_t index, uint32_t value);
uint32_t tegra_hsp_sm_readl(uint32_t index);

void tegra_hsp_sm_full_enable(uint32_t sm);
int tegra_hsp_ss_clear(uint32_t index, uint32_t data);
int tegra_hsp_ss_set(uint32_t index, uint32_t data);
int tegra_hsp_ss_read(uint32_t index, uint32_t *value);

#endif