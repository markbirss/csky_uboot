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
#include <asm/arch-eragon/emmc_interface.h>
#include <configs/eragon.h>

#define CEATA_MAP	0xffffffff
static uint32_t base_adr = SOC_EMMC_ADDRBASE;

/*
 * Set the particular bits of the specified register.
 * @param[in] reg	The particular register to which the bits are to be set
 * @param[in] val	The bitmask for the bits which are to be set.
 * return 	The new value of the register
 */
uint32_t emmc_set_bits(controller_reg_e reg, uint32_t val)
{
    uint32_t *reg_addr;
    reg_addr = (uint32_t *)(base_adr + reg);

    *reg_addr |= val;
    return *reg_addr;
}

/*
 * Clear the particular bits of the specified register.
 * @param[in] reg	The particular register to which the bits are to be cleared.
 * @param[in] val	The bitmask for the bits which are to be cleared.
 * return 	The new value of the register
 */
uint32_t emmc_clear_bits(controller_reg_e reg, uint32_t val)
{
    uint32_t *reg_addr;
    reg_addr = (uint32_t *)(base_adr + reg);

    *reg_addr &= (~val);
    return *reg_addr;
}

void plat_loop(uint32_t value)
{
    uint32_t counter;
    int i;
    volatile int j;

    for (counter = 0; counter < (value * 5); counter++)
        for (i = 0; i < 100; i++) {
            j = i;
        }
}
/*
 * Set the value of the specified register.
 * @param[in] reg	The particular register for which values are to be set.
 * @param[in] val	The value.
 * return 	The new value of the register
 */
uint32_t emmc_set_register(controller_reg_e reg, uint32_t val)
{
    uint32_t *reg_addr;
    reg_addr = (uint32_t *)(base_adr + reg);

    *reg_addr = val;
    return *reg_addr;
}

/*
 * Read the value of the specified register.
 * @param[in] reg	The particular register which is to be read.
 * return 	The value of the register
 */
uint32_t emmc_read_register(controller_reg_e reg)
{
    uint32_t *reg_addr;
    uint32_t retval;

    reg_addr = (uint32_t *)(base_adr + reg);
    retval = *reg_addr;

    /* if request is to read CDETECT, make sure you return
       the hacked ;) CEATA_MAP bit */
    if (CDETECT == reg) {
        retval &= CEATA_MAP;
    }

    return retval;
}

