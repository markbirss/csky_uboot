/*
 * SPDX-License-Identifier: GPL-2.0+
 */
#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <asm/io.h>
#include <spl.h>
#include <configs/eragon.h>
#include <asm/spl.h>
#include <asm/arch-eragon/datatype.h>
#include <asm/arch-eragon/mini_printf.h>
#include <asm/arch-eragon/gpio.h>
extern int32_t uart_open( uint32_t uart_addrbase);
//extern uint32_t emmc_host_init(card_info_t *emmc_card_info);
extern void emmc_emmc_read(uint8_t slot_id, uint32_t from, uint32_t len, uint8_t *buf);
extern void sdram_init(void);
#define READ_ADDR 0x3000
//#define READ_ADDR 0x0
void board_init_f(ulong dummy)
{
	/* Clear global data */
	uart_open(CK_UART2_ADDRBASE);

	interrupt_init();
	sdram_init();
	mini_printf("Wellcome to SPL!\n");
}

void board_init_r(gd_t *gd, ulong dest_addr)
{
	int       i, j;
	int8_t    om_judge;
	uint32_t  retlen;
	uint8_t   buff_r[512] = {0x0};
	uint8_t   *ddr_base = (uint8_t *)0x0;
	uint32_t  sram_baseaddr = 0x0;
	void      (*fp)(void);
	uint32_t  ret;
	uint8_t   id[2] = {0x0};

	mini_printf("The U-Boot-spl start.\n");
	mini_printf("U-Boot version is 2016.07, internal version is 0.4\n");
	gpio_set_reuse(GPIOD, 0xff, CK_GPIO_BEHARDWARE);
	om_judge = get_boot_select();

//	switch(om_judge) {
//		case 0x0:
//			mini_printf("This is 0x0\n");
//		case 0x1:
	/* The mode of spiflash */
//		mini_printf("This is 0x1\n");
//		break;
//		case 0x2:
	/* The mode of emmc */
		mini_printf("This is eMMC mode.\n");
		ret = emmc_host_init(NULL);
		if (ret != 0) {
			mini_printf("The eMMC is not exist.\n");
//			break;
		}
		mini_printf("eMMC init ready.\n");
		for (i = 0; i < (CONFIG_UBOOT_SIZE + 511) / 512; i++) {
//            		emmc_emmc_read(0, (READ_ADDR + (i * 512))/0x200, 512, CONFIG_DDR_LOAD_ADDR + (i * 512));
//			emmc_emmc_read(0, (READ_ADDR + (i * 512))/0x200, 512, sram_baseaddr + (i * 512));
//            		emmc_emmc_read(0, 0x3000/0x200, 512, buff_r);
            		emmc_emmc_read(0, (0x200 + (i * 512))/0x200, 512, buff_r);
                        for(j=0;j<512;j++)
                        {
				mini_printf("%x ", buff_r[j]);
			}
		}
//		fp = (void (*)(void ))(*((uint32_t *)(sram_baseaddr)));
//        	fp = (void (*)(void ))(*((uint32_t *)(CONFIG_JUMP_DDR)));
//		(*fp)();
//		for(i=0;i<512;i++) {
//			mini_printf("The %d is %x\n",i,buff_r[i]);
//		}
//		break;
//		defalut:
//		mini_printf("OM mode is %x, please check the OM.\n", om_judge);
//		break;
//	}
	while(1);
}
