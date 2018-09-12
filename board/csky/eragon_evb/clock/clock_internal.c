/* ---------------------------------------------------------------------------
 *
 * C-Sky Microsystems Confidential
 * -------------------------------
 * This file and all its contents are properties of C-Sky Microsystems. The
 * information contained herein is confidential and proprietary and is not
 * to be disclosed outside of C-Sky Microsystems except under a
 * Non-Disclosured Agreement (NDA).
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------*/
#include <asm/clock.h>
//#include "om.h"
//#include "timer.h"
#define THIS_MODULE MODULE_DEV_CLOCK

#include "clock_internal.h"

typedef struct {
    uint32_t freq;      /* Unit: Hz */
    uint32_t  clk_src;   /* pmu_cfg_clk_sel_e */
    union {
        uint32_t cpu_pll_base_clk;   /* For cpu_pll: pmu_pll_cfg_h_clk_sel_e */
        uint8_t pmu_clk_xi_sel;     /* For pmu_clk: cscr0_xi_sel_e */
    };
} _clock_cpu_freq_setting_t;

int32_t set_reg_field(uint32_t  reg_addr,         /* reg address */
                      uint32_t  field_value,      /* Do not bit move */
                      uint8_t   field_bit_offset, /* Begin bit, 0~31 */
                      uint8_t   field_bit_length) /* Length of field, 0~31 */
{
    int i;
    if ((field_bit_offset + field_bit_length) > 32 ) {
        return FAILURE;
    }

    uint32_t field_mask = 0;
    for (i = 0; i < field_bit_length; ++i) {
        field_mask |= BIT(field_bit_offset + i);
    }

    volatile uint32_t reg_val;
    reg_val = (*((volatile uint32_t*)(reg_addr)) & (~field_mask)) |
              (field_value << field_bit_offset & field_mask);

    *((volatile uint32_t*)(reg_addr)) = reg_val;
    return 0;
}

int32_t get_reg_field(uint32_t  reg_addr,         /* reg address */
                      uint32_t *field_value,      /* bit moved to lowest bit */
                      uint8_t   field_bit_offset, /* Begin bit, 0~31 */
                      uint8_t   field_bit_length) /* Length of field, 0~31 */
{
    int i;
    if ((field_bit_offset + field_bit_length) > 32 ) {
        return FAILURE;
    }

    uint32_t field_mask = 0;
    for (i = 0; i < field_bit_length; ++i) {
        field_mask |= BIT(field_bit_offset + i);
    }

    volatile uint32_t reg_val = *((volatile uint32_t*)(reg_addr));
    *field_value = (reg_val & field_mask) >> field_bit_offset;
    return 0;
}


static void _clock_wait_for_pll_locked(void)
{
    uint32_t reg_value;
    uint32_t time = 0;

#if 1
    do {
//    	timer_init();
//    	udelay(1000);
//	time += 1000;
//	if (time > 1000000) {
//            printf("PLL can't lock in 1 second\n");
//        }
	reg_value = *((volatile uint32_t*)PMU_STATUS_H_ADDR);
    } while( (reg_value & BIT(PMU_STATUS_H_BIT__CPU_PLL_LOCK)) == 0 );

#else
    timer_start(TIMEOUT_COUNT_TIMER);   /* Use timer to measure lock time */
    do {
        timer_elapsed_time(TIMEOUT_COUNT_TIMER, &time);
        if (time > 1000000) {
            LOG_E_I(time);
        }
        reg_value = *((volatile uint32_t*)PMU_STATUS_H_ADDR);
    } while( (reg_value & BIT(PMU_STATUS_H_BIT__CPU_PLL_LOCK)) == 0 );
    timer_stop(TIMEOUT_COUNT_TIMER);
#endif
}

#ifdef CONFIG_IS_FPGA

/* There are only 2 CPU speed in FPGA version:
   1. From CPU PLL, speed is defined in cfg file: CPU_HIGH_FREQ
   2. From PMU CLK, speed is defined in cfg file: CPU_LOW_FREQ
 */
static const _clock_cpu_freq_setting_t s_cpu_freq_settings[] = {
    {CPU_HIGH_FREQ, PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, PMU_PLL_CFG_H_CLK_SEL_800M},
    {CPU_LOW_FREQ,  PMU_CFG_CLK_SEL_CPU_PMU_CLK,   CSCR0_XI_SEL_OSCCLK_DIV64 },
};

static bool _clock_check_cpu_freq_support(uint32_t freq,
                                          const _clock_cpu_freq_setting_t **setting)
{
    int i;
    for (i = 0; i < sizeof(s_cpu_freq_settings)/sizeof(s_cpu_freq_settings[0]); ++i) {
        if (freq == s_cpu_freq_settings[i].freq) {
            *setting = &(s_cpu_freq_settings[i]);
            return true;
        }
    }
    return false;
}

int32_t _clock_get_cpu_speed_from_reg(uint32_t *freq)
{
    uint32_t pmu_cfg_clk_sel;
    get_reg_field(PMU_CFG_ADDR, &pmu_cfg_clk_sel, PMU_CFG_BIT__CLK_SEL, 2);

    if (CLK_SRC_IS_CPU_PLL(pmu_cfg_clk_sel)) {
        *freq = CPU_HIGH_FREQ;
    } else {
        *freq = CPU_LOW_FREQ;
    }
    return SUCCESS;
}

int32_t _clock_set_cpu_speed_by_reg(uint32_t freq_new)
{
    uint32_t freq_old;
    const _clock_cpu_freq_setting_t *setting_new;    /* CPU_PLL or PMU_CLK  */
    const _clock_cpu_freq_setting_t *setting_old;
    volatile uint32_t reg_value;

    /* Check freq_new */
    if (_clock_check_cpu_freq_support(freq_new, &setting_new) == false) {
        printf("freq_new = %d\n", freq_new);
        return FAILURE;
    }

    /* Get & Check old freq and clock source */
    if (_clock_get_cpu_speed_from_reg(&freq_old) != SUCCESS) {
        LOG_E();
        return FAILURE;
    }

    if (freq_old == freq_new) {
        return SUCCESS;
    }

    _clock_op_req_switch_clk_source_cpu_pll(setting_new->clk_src);
    return SUCCESS;
}

#else   /* #ifdef CONFIG_IS_FPGA */

static const _clock_cpu_freq_setting_t s_cpu_freq_settings[] = {
    /* CPU using pre-defined PLL clock */
    {CLOCK_CPU_FREQ_800M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_CLK_SEL_800M}},
    {CLOCK_CPU_FREQ_600M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_CLK_SEL_600M}},
    {CLOCK_CPU_FREQ_400M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_CLK_SEL_400M}},
    {CLOCK_CPU_FREQ_300M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_CLK_SEL_300M}},
    {CLOCK_CPU_FREQ_200M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_2, {PMU_PLL_CFG_H_CLK_SEL_400M}},
    {CLOCK_CPU_FREQ_150M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_2, {PMU_PLL_CFG_H_CLK_SEL_300M}},
    {CLOCK_CPU_FREQ_100M, PMU_CFG_CLK_SEL_CPU_PLL_DIV_4, {PMU_PLL_CFG_H_CLK_SEL_400M}},
    {CLOCK_CPU_FREQ_75M,  PMU_CFG_CLK_SEL_CPU_PLL_DIV_4, {PMU_PLL_CFG_H_CLK_SEL_300M}},

    /* CPU using user-defined PLL clock */
    {CLOCK_CPU_FREQ_1000M,  PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_1000M}},
    {CLOCK_CPU_FREQ_950M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_950M}},
    {CLOCK_CPU_FREQ_940M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_940M}},
    {CLOCK_CPU_FREQ_930M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_930M}},
    {CLOCK_CPU_FREQ_920M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_920M}},
    {CLOCK_CPU_FREQ_910M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_910M}},
    {CLOCK_CPU_FREQ_900M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_900M}},
    {CLOCK_CPU_FREQ_850M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_850M}},
    {CLOCK_CPU_FREQ_700M,   PMU_CFG_CLK_SEL_CPU_PLL_DIV_1, {PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_700M}},

    {CLOCK_CPU_FREQ_24M,  PMU_CFG_CLK_SEL_CPU_PMU_CLK, CSCR0_XI_SEL_OSCCLK_DIV1  },
    {CLOCK_CPU_FREQ_12M,  PMU_CFG_CLK_SEL_CPU_PMU_CLK, CSCR0_XI_SEL_OSCCLK_DIV2  },
    {CLOCK_CPU_FREQ_3M,   PMU_CFG_CLK_SEL_CPU_PMU_CLK, CSCR0_XI_SEL_OSCCLK_DIV8  },
    {CLOCK_CPU_FREQ_375K, PMU_CFG_CLK_SEL_CPU_PMU_CLK, CSCR0_XI_SEL_OSCCLK_DIV64 },
    {CLOCK_CPU_FREQ_46875,PMU_CFG_CLK_SEL_CPU_PMU_CLK, CSCR0_XI_SEL_OSCCLK_DIV512},
};

static bool _clock_check_cpu_freq_support(uint32_t freq,
                                          const _clock_cpu_freq_setting_t **setting)
{
    int i;
    for (i = 0; i < sizeof(s_cpu_freq_settings)/sizeof(s_cpu_freq_settings[0]); ++i) {
        if (freq == s_cpu_freq_settings[i].freq) {
            *setting = &(s_cpu_freq_settings[i]);
            return true;
        }
    }
    return false;
}

int32_t _clock_get_cpu_speed_from_reg(uint32_t *freq)
{
    int ret = FAILURE;
    *freq = 0;
    uint32_t pmu_cfg_clk_sel;
    get_reg_field(PMU_CFG_ADDR, &pmu_cfg_clk_sel, PMU_CFG_BIT__CLK_SEL, 2);

    if (pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PMU_CLK) {
        ret = SUCCESS;
        const int32_t pmu_clock = 24*1000*1000;
        volatile chip_ctrl_cscr0_t cscr0 =
            *((volatile chip_ctrl_cscr0_t*)(CHIP_CTRL_CSCR0_ADDR));

        switch (cscr0.xi_sel) {
        case CSCR0_XI_SEL_OSCCLK_DIV1:
            *freq = pmu_clock / 1;
            break;
        case CSCR0_XI_SEL_OSCCLK_DIV2:
            *freq = pmu_clock / 2;
            break;
        case CSCR0_XI_SEL_OSCCLK_DIV8:
            *freq = pmu_clock / 8;
            break;
        case CSCR0_XI_SEL_OSCCLK_DIV64:
            *freq = pmu_clock / 64;
            break;
        case CSCR0_XI_SEL_OSCCLK_DIV512:
            *freq = pmu_clock / 512;
            break;
        default:
            printf("cscr0.xi_sel=%d\n", cscr0.xi_sel);
            ret = FAILURE;
            break;
        }
    } else if (pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PLL_DIV_1 ||
               pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PLL_DIV_2 ||
               pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PLL_DIV_4) {
        uint8_t div;
        if (pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PLL_DIV_1)
            div = 1;
        else if (pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PLL_DIV_2)
            div = 2;
        else if (pmu_cfg_clk_sel == PMU_CFG_CLK_SEL_CPU_PLL_DIV_4)
            div = 4;
        else {
            printf("div=%d\n", div);
            return FAILURE;
        }

        uint32_t cpu_base_freq;
        uint32_t soc_pmu_pll_cfg_h_clk_sel;
        get_reg_field(PMU_PLL_CFG_H_ADDR, &soc_pmu_pll_cfg_h_clk_sel, 28, 3);
        if (soc_pmu_pll_cfg_h_clk_sel == PMU_PLL_CFG_H_CLK_SEL_800M)
            cpu_base_freq = 800 * 1000 * 1000;
        else if (soc_pmu_pll_cfg_h_clk_sel == PMU_PLL_CFG_H_CLK_SEL_600M)
            cpu_base_freq = 600 * 1000 * 1000;
        else if (soc_pmu_pll_cfg_h_clk_sel == PMU_PLL_CFG_H_CLK_SEL_400M)
            cpu_base_freq = 400 * 1000 * 1000;
        else if (soc_pmu_pll_cfg_h_clk_sel == PMU_PLL_CFG_H_CLK_SEL_300M)
            cpu_base_freq = 300 * 1000 * 1000;
        else if (soc_pmu_pll_cfg_h_clk_sel == PMU_PLL_CFG_H_CLK_SEL_USER_DEFINED) {
            volatile uint32_t reg_pmu_pll_cfg_h_val =
            *((volatile uint32_t*)(PMU_PLL_CFG_H_ADDR));
            switch (reg_pmu_pll_cfg_h_val) {
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_1000M:
                cpu_base_freq = CLOCK_CPU_FREQ_1000M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_950M:
                cpu_base_freq = CLOCK_CPU_FREQ_950M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_940M:
                cpu_base_freq = CLOCK_CPU_FREQ_940M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_930M:
                cpu_base_freq = CLOCK_CPU_FREQ_930M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_920M:
                cpu_base_freq = CLOCK_CPU_FREQ_920M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_910M:
                cpu_base_freq = CLOCK_CPU_FREQ_910M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_900M:
                cpu_base_freq = CLOCK_CPU_FREQ_900M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_850M:
                cpu_base_freq = CLOCK_CPU_FREQ_850M;
                break;
            case PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_700M:
                cpu_base_freq = CLOCK_CPU_FREQ_700M;
                break;
            default:
                printf("reg_pmu_pll_cfg_h_val=%d\n", reg_pmu_pll_cfg_h_val);
                return FAILURE;
            }
        }
        else {
            printf("soc_pmu_pll_cfg_h_clk_sel=%d\n", soc_pmu_pll_cfg_h_clk_sel);
            return FAILURE;
        }

        *freq = cpu_base_freq / div;
        ret = SUCCESS;

    } else {
        printf("pmu_cfg_clk_sel=%d\n", pmu_cfg_clk_sel);
    }
    return ret;
}

int32_t _clock_set_cpu_speed_by_reg(uint32_t freq_new)
{
    uint32_t freq_old;
    const _clock_cpu_freq_setting_t *setting_new;    /* CPU_PLL or PMU_CLK  */
    const _clock_cpu_freq_setting_t *setting_old;

    /* Check freq_new */
    if (_clock_check_cpu_freq_support(freq_new, &setting_new) == false) {
        printf("freq_new=%d\n", freq_new);
        return FAILURE;
    }

    /* Get & Check old freq and clock source */
    if (_clock_get_cpu_speed_from_reg(&freq_old) != SUCCESS) {

        printf("Error 1");
        return FAILURE;
    }


    if (_clock_check_cpu_freq_support(freq_old, &setting_old) == false) {
        printf("freq_old=%d\n", freq_old);
        return FAILURE;
    }

    if (CLK_SRC_IS_CPU_PLL(setting_new->clk_src)) {
        if (CLK_SRC_IS_CPU_PLL(setting_old->clk_src)) {
            /* switch to pmu clock before new CPU pll locked, then switch back */
            _clock_op_req_switch_clk_source_pmu_clk();
        }

        /**** Power down PLL ****/
        _clock_op_set_cpu_pll_assert();
        _clock_op_req_cpu_pll_power_switch();

        /**** Update PLL ********/
        _clock_op_set_cpu_pll_clk(setting_new->cpu_pll_base_clk); /* e.g. PMU_PLL_CFG_H_CLK_SEL_600M */
        _clock_op_req_cpu_pll_update();

        /**** Power on PLL ******/
        _clock_op_set_cpu_pll_deassert();
        _clock_op_req_cpu_pll_power_switch();

        /**** wait and switch clock source ****/
        _clock_wait_for_pll_locked();
        _clock_op_req_switch_clk_source_cpu_pll(setting_new->clk_src);
    }
    else if (CLK_SRC_IS_PMU_CLK(setting_new->clk_src)) {
        if (CLK_SRC_IS_CPU_PLL(setting_old->clk_src)) {
            _clock_op_req_switch_clk_source_pmu_clk();
        }
        set_reg_field(CHIP_CTRL_CSCR0_ADDR, setting_new->pmu_clk_xi_sel, 0, 4);
    }
    else {
        printf("setting_old->clk_src=%d, setting_new->clk_src=%d\n",
		setting_old->clk_src, setting_new->clk_src);
        return FAILURE;
    }

    return SUCCESS;
}
#endif  /* #ifdef CONFIG_IS_FPGA */

