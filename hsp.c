#include "hsp.h"
#include "aaa_type.h"
#include "aaa_print.h"

#define BIT(_x_) ((uint32_t)(1U) << (_x_))

static inline uint32_t readl(uint32_t addr)
{
	return *(volatile uint32_t *)addr;
}

static inline void writel(uint32_t data, uint32_t addr)
{
	*(volatile uint32_t *)addr = data;
}

void tegra_hsp_sm_writel(uint32_t index, uint32_t value)
{
	writel(value, NV_ADDRESS_MAP_AON_HSP_BASE + HSP_COMMON_SIZE + (HSP_SHRD_MBOX_OFFSET * index));
}

uint32_t tegra_hsp_sm_readl(uint32_t index)
{
	return readl(NV_ADDRESS_MAP_AON_HSP_BASE + HSP_COMMON_SIZE + (HSP_SHRD_MBOX_OFFSET * index));
}

static void tegra_hsp_si_writel(uint32_t index, uint32_t value)
{
	writel(value, NV_ADDRESS_MAP_AON_HSP_BASE + HSP_INT_IE + (HSP_INT_IE_OFFSET * index));
}

static uint32_t tegra_hsp_si_readl(uint32_t index)
{

	return readl(NV_ADDRESS_MAP_AON_HSP_BASE + HSP_INT_IE + (HSP_INT_IE_OFFSET * index));
}

static inline uint32_t tegra_hsp_ss_reg(int index, uint32_t reg)
{
	uint32_t addr = NV_ADDRESS_MAP_AON_HSP_BASE + HSP_COMMON_SIZE + (HSP_SHRD_MBOX_OFFSET * 8) + (HSP_SHRD_SEM_OFFSET * index) + reg;
	return addr;
}

static inline uint32_t tegra_hsp_sm_reg(int index)
{
	uint32_t addr = NV_ADDRESS_MAP_AON_HSP_BASE + HSP_COMMON_SIZE + HSP_SHRD_MBOX_OFFSET * index;
	return addr;
}

static void tegra_hsp_sm_enable(unsigned setbit)
{
	uint32_t ie;

	for (int i = 1; i < 5; i ++ ) {
		ie = tegra_hsp_si_readl(i);
		tegra_hsp_si_writel(i, ie | BIT(setbit));
	}
}

void tegra_hsp_sm_full_enable(uint32_t sm)
{
    uint32_t reg = tegra_hsp_sm_reg(sm) + MBOX_FULL_IE_OFFSET;
    // print_reg(reg);
    writel(1, reg);

    unsigned iebit = HSP_INT_IE_0_mbox_full_enable_SHIFT + sm;
    // print_reg(iebit);
	tegra_hsp_sm_enable(iebit);
}

int tegra_hsp_ss_read(uint32_t index, uint32_t *value)
{
	uint32_t addr;

	addr = tegra_hsp_ss_reg(index, HSP_SHRD_SEM_STA);
	*value = readl(addr);

	return 0;
}

int tegra_hsp_ss_clear(uint32_t index, uint32_t data)
{
	uint32_t addr;

	addr = tegra_hsp_ss_reg(index, HSP_SHRD_SEM_CLR);
	writel(data, addr);

	return 0;
}

int tegra_hsp_ss_set(uint32_t index, uint32_t data)
{
	uint32_t addr;

	addr = tegra_hsp_ss_reg(index, HSP_SHRD_SEM_SET);
	writel(data, addr);

	return 0;
}