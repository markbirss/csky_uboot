/******************************************************************************
 * @file     spiflash.c
 * @brief    The File for the spiflash driver
 * @version  V1.0
 * @date     23. Dec 2016
 ******************************************************************************/
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

#include <asm/arch-eragon/datatype.h>
#include <asm/arch-eragon/spi.h>
#include <asm/arch-eragon/spiflash.h>
#include <configs/eragon.h>

#define THIS_MODULE MODULE_DEV_SPIFLASH

#define SPI_FLASH_BLOCK_SIZE        0x10000
#define SPI_FLASH_SECTOR_SIZE       0x1000
#define SPI_FLASH_PAGE_SIZE         0x100
#define SPI_FLASH_BASEADDR           0x0
#define IS_SPIFLASH_ADDR(addr) \
            ((addr >= 0x0) && (addr < 0x800000))

int32_t spi_norflash_read_status_register(uint8_t *status)
{
    uint8_t command = 0x05;

    /* read status register*/
    spi_open(CK_SPI);
    spi_set_ndf(CK_SPI, 0);
    spi_transfer_query(CK_SPI, CK_SPI_CS0, &command, 1, status, 1);

    status++;
    command = 0x35;
    spi_open(CK_SPI);
    spi_set_ndf(CK_SPI, 0);

    spi_transfer_query(CK_SPI, CK_SPI_CS0, &command, 1, status, 1);
    return SUCCESS;
}

int32_t spi_norflash_write_status_register(uint8_t *status)
{
    uint8_t reg1[2]={0xff, 0xff};
    uint8_t command_enable_write = 0x06;
    uint8_t command[3] = {0x01, 0x0, 0x0};

    spi_open(CK_SPI);
    spi_set_mode(CK_SPI,CK_SPI_TX);

    command[1] = *status;
    command[2] = *(status + 1);

    spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
    spi_send_query(CK_SPI, CK_SPI_CS0, command, 3);

    while((reg1[0] & 0x1) == 1){
        spi_norflash_read_status_register(reg1);
    }

    return SUCCESS;
}

int32_t spi_norflash_erase_chip(uint8_t id)
{
    uint8_t reg1[2]={0xff, 0xff};
    uint8_t reg[2]={0x00, 0x00};
    uint8_t command_enable_write = 0x06;
    uint8_t command_erase_chip = 0x60;

    spi_open(CK_SPI);
    spi_set_mode(CK_SPI, CK_SPI_TX);

    spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
    while((reg[0] & 0x2) == 0){
        spi_norflash_read_status_register(reg);
    }
    spi_set_mode(CK_SPI, CK_SPI_TX);
    spi_send_query(CK_SPI, CK_SPI_CS0, &command_erase_chip, 1);
    while(((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)){
        spi_norflash_read_status_register(reg1);
    }

    return SUCCESS;
}

int32_t spi_norflash_erase_block(uint8_t id, uint32_t offset, uint32_t length)
{
    uint8_t reg1[2]={0xff, 0xff};
    uint8_t reg[2]={0x00, 0x00};
    uint8_t command_enable_write = 0x06;
    uint8_t command_erase_block[4] = {0xd8, 0x0, 0x0, 0x0};

    if(length & (SPI_FLASH_BLOCK_SIZE - 1)){
//        LOG_D_S("erase length is not aligned by block size");
//        LOG_E();
        return FAILURE;
    }
    offset += SPI_FLASH_BASEADDR;
    while(length){
        spi_open(CK_SPI);
        spi_set_mode(CK_SPI, CK_SPI_TX);

        command_erase_block[1] = (offset >> 16) & 0xff;
        command_erase_block[2] = (offset >> 8) & 0xff;
        command_erase_block[3] = offset & 0xff;

        spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
        while((reg[0] & 0x2) == 0){
            spi_norflash_read_status_register(reg);
        }
        spi_set_mode(CK_SPI, CK_SPI_TX);
        spi_send_query(CK_SPI, CK_SPI_CS0, command_erase_block, 4);
        while(((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)){
            spi_norflash_read_status_register(reg1);
        }
        offset += SPI_FLASH_BLOCK_SIZE;
        length -= SPI_FLASH_BLOCK_SIZE;
    }

    return SUCCESS;
}

int32_t spi_norflash_erase_sector(uint8_t id, uint32_t offset, uint32_t length)
{
    uint8_t reg1[2]={0xff, 0xff};
    uint8_t reg[2]={0x00, 0x00};
    uint8_t command_enable_write = 0x06;
    uint8_t command_erase_sector[5] = {0x20, 0x0, 0x0, 0x0};

    if(length & (SPI_FLASH_SECTOR_SIZE - 1)){
        return -1;
    }
    offset += SPI_FLASH_BASEADDR;
    while(length) {
        spi_open(CK_SPI);
        spi_set_mode(CK_SPI, CK_SPI_TX);

        command_erase_sector[1] = (offset >> 16) & 0xff;
        command_erase_sector[2] = (offset >> 8) & 0xff;
        command_erase_sector[3] = offset & 0xff;

        spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
        while((reg[0] & 0x2) == 0){
            spi_norflash_read_status_register(reg);
        }
        spi_set_mode(CK_SPI, CK_SPI_TX);
        spi_send_query(CK_SPI, CK_SPI_CS0, command_erase_sector, 4);
        while(((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)){
            spi_norflash_read_status_register(reg1);
        }
        offset += SPI_FLASH_SECTOR_SIZE;
        length -= SPI_FLASH_SECTOR_SIZE;
    }
    return SUCCESS;

}

int32_t spiflash_erase(uint8_t id, ERASE_TYPE_t type, uint32_t offset, uint32_t length)
{
    switch(type){
    case ERASE_BY_SECTOR:
        spi_norflash_erase_sector(id, offset, length);
        break;
    case ERASE_BY_BLOCK:
        spi_norflash_erase_block(id, offset, length);
        break;
    case ERASE_BY_CHIP:
        spi_norflash_erase_chip(id);
        break;
    default:
        return FAILURE;
    }

    return SUCCESS;
}

int32_t spi_norflash_write_page(uint8_t id, uint32_t dst_addr, uint8_t *src_buf, uint32_t len)
{
    uint8_t reg1[2]={0xff, 0xff};
    uint8_t reg[2]={0x00, 0x00};
    uint8_t command_enable_write = 0x06;
    uint8_t command_page_write[260] = {0x02, 0x0, 0x0, 0x0};
    if(len == 0){
        return FAILURE;
    }

    spi_open(CK_SPI);
    spi_set_mode(CK_SPI,CK_SPI_TX);
    spi_set_ndf(CK_SPI, len - 1);

    command_page_write[1] = (dst_addr >> 16) & 0xff;
    command_page_write[2] = (dst_addr >> 8) & 0xff;
    command_page_write[3] = dst_addr & 0xff;

    memcpy(&command_page_write[4], src_buf, len);
    spi_send_query(CK_SPI, CK_SPI_CS0, &command_enable_write, 1);
    while((reg[0] & 0x2) == 0){
        spi_norflash_read_status_register(reg);
    }
    spi_set_mode(CK_SPI, CK_SPI_TX);
    spi_send_query(CK_SPI, CK_SPI_CS0, command_page_write, len + 4);

    while(((reg1[0] & 0x1) == 1) || ((reg1[0] & 0x2) == 0x2)){
        spi_norflash_read_status_register(reg1);
    }
    return SUCCESS;
}

int32_t spiflash_write(uint8_t id, uint32_t offset, const uint8_t *buf, uint32_t length, uint32_t *retlen)
{
    uint32_t first_page_space;
    uint8_t *p = (uint8_t *)buf;
    uint32_t page_num;
    uint8_t i;

    offset += SPI_FLASH_BASEADDR;
    first_page_space = SPI_FLASH_PAGE_SIZE - (offset & (SPI_FLASH_PAGE_SIZE - 1));
    if(length < first_page_space){
        first_page_space = length;
    }
    spi_norflash_write_page(id, offset, p, first_page_space);
    offset += first_page_space;
    p += first_page_space;
    length -= first_page_space;

    page_num = length >> 8;
    for(i=0; i<page_num; i++)
    {
        spi_norflash_write_page(id, offset, p, SPI_FLASH_PAGE_SIZE);
        offset += SPI_FLASH_PAGE_SIZE;
        p += SPI_FLASH_PAGE_SIZE;
        length -= SPI_FLASH_PAGE_SIZE;
    }
    spi_norflash_write_page(id, offset, p, length);
    return SUCCESS;
}

int32_t spiflash_read_id(uint8_t *id)
{
    uint8_t command[4]={0x90, 0x0, 0x0, 0x0};
    /* read the ID*/
    spi_open(CK_SPI);
    spi_set_ndf(CK_SPI, 1);

    spi_transfer_query(CK_SPI, CK_SPI_CS0, command, 4, id, 2);
    return SUCCESS;
}

int32_t spiflash_read(uint8_t id, uint32_t offset, uint8_t *buf, uint32_t length, uint32_t * retlen)
{
    uint8_t command[4]={0x3, 0x0, 0x0, 0x00};

    offset += SPI_FLASH_BASEADDR;
    spi_open(CK_SPI);
    spi_set_ndf(CK_SPI, length - 1);

    command[1] = (offset >> 16) & 0xff;
    command[2] = (offset >> 8) & 0xff;
    command[3] = offset & 0xff;

    spi_transfer_query(CK_SPI, CK_SPI_CS0, command, 4, buf, length);
    return SUCCESS;
}
#if 0
int spi_norflash_program(uint32_t dst_addr, uint8_t *src_buf, uint32_t len)
{
    uint32_t i;
    uint32_t block_addr;
    uint32_t sector_addr;
    uint32_t page_num;
    uint32_t first_page_space;
    uint8_t *p = src_buf;
    uint32_t length;

    if(!IS_SPIFLASH_ADDR(dst_addr) || !IS_SPIFLASH_ADDR(dst_addr+len) || src_buf== NULL){
        return -1;
    }

    length = len;
    /* erase the region */
    if(length > SPI_FLASH_BLOCK_SIZE)
    {
        block_addr = dst_addr;
        while(block_addr < (dst_addr + length)) {
            spi_norflash_erase_block(block_addr);
            block_addr += SPI_FLASH_BLOCK_SIZE;
        }
        spi_norflash_erase_block(dst_addr + length);


    }else if(length < SPI_FLASH_BLOCK_SIZE && length > SPI_FLASH_SECTOR_SIZE)
    {
        sector_addr = dst_addr;
        while(sector_addr < (dst_addr + length)) {
            spi_norflash_erase_sector(sector_addr);
            sector_addr += SPI_FLASH_SECTOR_SIZE;
        }
        spi_norflash_erase_sector(dst_addr + length);

    }
    else{
        spi_norflash_erase_chip();
    }
    first_page_space = SPI_FLASH_PAGE_SIZE - (dst_addr & (SPI_FLASH_PAGE_SIZE - 1));
    spi_norflash_write_page(dst_addr, p, first_page_space);
    dst_addr += first_page_space;
    p += first_page_space;
    length -= first_page_space;

    page_num = length >> 8;
    for(i=0; i<page_num-1; i++)
    {
        spi_norflash_write_page(dst_addr, p, SPI_FLASH_PAGE_SIZE);
        dst_addr += SPI_FLASH_PAGE_SIZE;
        p += SPI_FLASH_PAGE_SIZE;
        length -= SPI_FLASH_PAGE_SIZE;
    }
    spi_norflash_write_page(dst_addr, p, length);

    return 0;
}
#endif
