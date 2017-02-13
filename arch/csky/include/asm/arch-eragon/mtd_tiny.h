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
#include "datatype.h"

#ifndef __MTD_TINY_H__
#define __MTD_TINY_H__

typedef enum {
    MTDT_MEMORY_TYPE_PROTOCOL_RECV_BUF = 0x00,
    MTDT_MEMORY_TYPE_PROTOCOL_SEND_BUF = 0x01,
    MTDT_MEMORY_TYPE_INTERNAL_SRAM     = 0x02,
    MTDT_MEMORY_TYPE_INTERNAL_EFLASH   = 0x03,

    MTDT_MEMORY_TYPE_SDRAM_DDR         = 0x10,
    MTDT_MEMORY_TYPE_NOR_FLASH         = 0x11,
    MTDT_MEMORY_TYPE_SPI_FLASH         = 0x12,
    MTDT_MEMORY_TYPE_NAND_FLASH        = 0x13,
    MTDT_MEMORY_TYPE_SD                = 0x14,
    MTDT_MEMORY_TYPE_EMMC              = 0x15
} mtdt_memory_type_t;

#define MTDT_MAX_DEVICES    8

#define MTDT_WRITEABLE      0x04    /* Device is writeable */
#define MTDT_BIT_WRITEABLE  0x08    /* Single bits can be flipped */
#define MTDT_NO_ERASE       0x10    /* No erase necessary */

#define MTDT_CAP_ROM        0
#define MTDT_CAP_RAM        (MTDT_WRITEABLE | MTDT_BIT_WRITEABLE | MTDT_NO_ERASE)
#define MTDT_CAP_NORFLASH   (MTDT_WRITEABLE | MTDT_BIT_WRITEABLE)
#define MTDT_CAP_NANDFLASH	(MTDT_WRITEABLE)
#define MTDT_CAP_EMMCFLASH	(MTDT_WRITEABLE | MTDT_NO_ERASE)

#define MTDT_MAX_PAGE_SIZE 512

typedef struct mtdt_erase_info {
	uint64_t offset;
	uint64_t length;
} mtdt_erase_info_t;

typedef struct mtdt_info {
    uint8_t  type;      /* Defined in mtdt_memory_type_t */
    uint8_t  index;     /* Index for same MTDT type defice */
    uint8_t  flags;     /* Combined by MTDT_CAP_xx */
    uint8_t  reserved;

    uint64_t size;      /* Device total size, unit:Byte */
    uint32_t erasesize; /* Device erase size. For NAND is one block */
    uint16_t writesize; /* Device write size. For NAND is one page */

    uint8_t *name;      /* Device name */
    void    *priv;      /* Device private data pointer */

    int32_t(*erase) (struct mtdt_info *mtdt, struct mtdt_erase_info *erase_info);
    int32_t(*read)  (struct mtdt_info *mtdt, uint64_t offset, uint32_t len, uint32_t *retlen, uint8_t *buf);
    int32_t(*write) (struct mtdt_info *mtdt, uint64_t offset, uint32_t len, uint32_t *retlen, const uint8_t *buf);
} mtdt_info_t;

int32_t mtdt_init(void);
int32_t mtdt_add_device(mtdt_info_t *mtdt);
int32_t mtdt_find_device(mtdt_info_t **mtdt, uint8_t type, uint8_t id);
int32_t mtdt_get_infos(mtdt_info_t ***infos, uint32_t *count);

#endif
