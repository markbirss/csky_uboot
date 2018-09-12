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
#ifndef __CLOCK_INTERNAL_H__
#define __CLOCK_INTERNAL_H__
#include <common.h>
#include <asm/datatype.h>
#include <asm/arch/hardware.h>

#define BIT(nr) (1UL << (nr))

#define FAILURE -1
#define SUCCESS 0
#define SOC_PMU_ADDRBASE MPU_BASEADDR
#define SOC_CHIP_CTRL_ADDRBASE CHIP_CTRL_BASEADDR

#define PMU_CFG_ADDR        (SOC_PMU_ADDRBASE + 0x00)
#define PMU_CFG_BIT__CLK_SEL 8

#define PMU_PLL_CFG_L_ADDR  (SOC_PMU_ADDRBASE + 0x04)
#define PMU_PLL_CFG_H_ADDR  (SOC_PMU_ADDRBASE + 0x08)

#define PMU_STATUS_H_ADDR   (SOC_PMU_ADDRBASE + 0x38)
#define PMU_STATUS_H_BIT__CPU_PLL_LOCK 8

#define PMU_PLL_CFG_H_ADDR (SOC_PMU_ADDRBASE + 0x08)

/* PMU_CFG.CLK_SEL [8:11] */
typedef enum {
    PMU_CFG_CLK_SEL_CPU_PLL_DIV_1 = 0,
    PMU_CFG_CLK_SEL_CPU_PLL_DIV_2 = 1,
    PMU_CFG_CLK_SEL_CPU_PLL_DIV_4 = 2,
    PMU_CFG_CLK_SEL_CPU_PMU_CLK   = 3,
} pmu_cfg_clk_sel_e;

#define CLK_SRC_IS_CPU_PLL(clk_src)                \
    ((clk_src == PMU_CFG_CLK_SEL_CPU_PLL_DIV_1) || \
     (clk_src == PMU_CFG_CLK_SEL_CPU_PLL_DIV_2) || \
     (clk_src == PMU_CFG_CLK_SEL_CPU_PLL_DIV_4) )

#define CLK_SRC_IS_PMU_CLK(clk_src)                \
    (clk_src == PMU_CFG_CLK_SEL_CPU_PMU_CLK)

typedef enum {
    /* Pre-defined Clock freq */
    PMU_PLL_CFG_H_CLK_SEL_800M = 0,
    PMU_PLL_CFG_H_CLK_SEL_600M = 1,
    PMU_PLL_CFG_H_CLK_SEL_400M = 2,
    PMU_PLL_CFG_H_CLK_SEL_300M = 3,

    /* User defined clock, the field CLK_SEL must be 0x4 */
    PMU_PLL_CFG_H_CLK_SEL_USER_DEFINED = 4,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_1000M = 0x42487d0c,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_950M  = 0x424876cc,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_940M  = 0x4248758c,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_930M  = 0x4248744c,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_920M  = 0x4248730c,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_910M  = 0x424871cc,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_900M  = 0x4248708c,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_850M  = 0x42486a4c,
    PMU_PLL_CFG_H_SPECIAL_CLK_VALUE_700M  = 0x42482bc6,
} pmu_pll_cfg_h_clk_sel_e;

#define CHIP_CTRL_CSCR0_ADDR (SOC_CHIP_CTRL_ADDRBASE + 0x00)
typedef struct {
    uint8_t xi_sel:4;       /* [0:3]    */
    uint8_t cfgclk_sel:4;   /* [4:7]    */
    uint8_t dxclk_sel:4;    /* [8:11]   */
    uint8_t lcdcclk_sel:4;  /* [12:15]  */
    uint8_t vpucclk_sel:4;  /* [16:19]  */
    uint8_t vpuaclk_sel:4;  /* [20:23]  */
    uint8_t reserved1:4;    /* [24:27]  */
    uint8_t reserved2:4;    /* [28:31]  */
} chip_ctrl_cscr0_t;

typedef enum {
    CSCR0_XI_SEL_OSCCLK_DIV1   = 0,
    CSCR0_XI_SEL_OSCCLK_DIV2   = 1,
    CSCR0_XI_SEL_OSCCLK_DIV8   = 2,
    CSCR0_XI_SEL_OSCCLK_DIV64  = 3,
    CSCR0_XI_SEL_OSCCLK_DIV512 = 4,
} cscr0_xi_sel_e;

int32_t set_reg_field(uint32_t  reg_addr,         /* reg address */
                      uint32_t  field_value,      /* Do not bit move */
                      uint8_t   field_bit_offset, /* Begin bit, 0~31 */
                      uint8_t   field_bit_length);/* Length of field, 0~31 */

int32_t get_reg_field(uint32_t  reg_addr,         /* reg address */
                      uint32_t *field_value,      /* bit moved to lowest bit */
                      uint8_t   field_bit_offset, /* Begin bit, 0~31 */
                      uint8_t   field_bit_length);/* Length of field, 0~31 */


int32_t _clock_get_cpu_speed_from_reg(uint32_t *freq);
int32_t _clock_set_cpu_speed_by_reg(uint32_t freq_new);

static inline void _clock_op_req_switch_clk_source_pmu_clk(void)
{
    /* Bit(s)   Value   Name            Comments
       -----------------------------------------------------------------------
       1            1   REQ             low power request, auto cleared by hardware
       2            1   BYPASS          low power bypass mode
       5:3        000   MODE            0 <-> clock switch
       11:8      0011   CLK_SEL         3 <-> pmu clk
     */
    *((volatile uint32_t*)PMU_CFG_ADDR) = 0x306;
}


static inline void _clock_op_set_cpu_pll_assert(void)
{
    /* Bit(s)   Value   Name            Comments
       -----------------------------------------------------------------------
       6            1   FOUTVCOPD       VCO rate output clock power down
       5            1   FOUT4PHASEPD    Post divide power down
       1            1   PWRDN           cpu pll power down, active high
       0            1   RESET           cpu pll reset, active high
     */
    *((volatile uint32_t*)PMU_PLL_CFG_L_ADDR) = 0x63;
}

static inline void _clock_op_req_cpu_pll_power_switch(void)
{
    /* Bit(s)   Value   Name            Comments
       -----------------------------------------------------------------------
       1            1   REQ             low power request, auto cleared by hardware
       2            1   BYPASS          cpu pll power down, active high
       5:3        011   MODE            3 <-> cpu pll power down
     */
    *((volatile uint32_t*)PMU_CFG_ADDR) = 0x1e;
}

static inline void _clock_op_set_cpu_pll_clk(pmu_pll_cfg_h_clk_sel_e pll_clk)
{
    if ((pll_clk & 0x70000000) == 0x40000000) {
        /* User defined CPU PLL */
        *((volatile uint32_t*)PMU_PLL_CFG_H_ADDR) = pll_clk;
    } else {
        /* Pre-defined CPU PLL
           Bit(s)   Value   Name            Comments
           -----------------------------------------------------------------------
           30:28      001   CLK_SEL         0 <-> 800MHz
                                            1 <-> 600MHz
                                            2 <-> 400MHz
                                            3 <-> 300MHz
         */
        set_reg_field(PMU_PLL_CFG_H_ADDR, pll_clk, 28, 3);
    }
}

static inline void _clock_op_req_cpu_pll_update(void)
{
    /* Bit(s)   Value   Name            Comments
       -----------------------------------------------------------------------
       1            1   REQ             low power request, auto cleared by hardware
       2            1   BYPASS          low power bypass mode
       5:3        010   MODE            2 <-> cpu pll frequency update
     */
    *((volatile uint32_t*)PMU_CFG_ADDR) = 0x16;
}

static inline void _clock_op_set_cpu_pll_deassert(void)
{
    /* Bit(s)   Value   Name            Comments
       -----------------------------------------------------------------------
       0            0   RESET           cpu pll deassert (un-reset / release)
       1            0   PWRDN           cpu pll power down, active high
       6            1   FOUTVCOPD       VCO rate output clock power down
     */
    *((volatile uint32_t*)PMU_PLL_CFG_L_ADDR) = 0x40;
}

static inline void _clock_op_req_switch_clk_source_cpu_pll(pmu_cfg_clk_sel_e clk_src)
{
    /* Bit(s)   Value   Name            Comments
       -----------------------------------------------------------------------
       1            1   REQ             low power request, auto cleared by hardware
       2            1   BYPASS          low power bypass mode
       5:3        000   MODE            low power mode, used for low power state
       11:8      0000   CLK_SEL         0 <-> cpu pll, div=1
                                        1 <-> cpu pll, div=2
                                        2 <-> cpu pll, div=4
                                        3 <-> pmu clk
     */
    *((volatile uint32_t*)PMU_CFG_ADDR) = (clk_src << 8) | 0x06;
}

#endif  /* __CLOCK_INTERNAL_H__ */

