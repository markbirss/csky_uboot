//-------------------------------------------------------------------------
//
//FILE NAME     : init_ddr.c
//AUTHOR        : Juncheng Shen
//FUNCTION      : Synopsys UMCTL2 initial sequence.
//INITIAL DATE  : 2016/06/23
//VERSION       : 1.1
//RELEASE NOTE  : 1.0: initial version.
//                1.1: changed PUB_REG access define by multiplying 4 to
//                     the reg addr.
//updater       :Wu Youfei
//date          :2016/07/12
//version       :1.2
//note          :
//1.add speed grade define DDR3_1333_H and DDR3_1066_G,select different config for different speed grade
//2.modify ADDRMAP register configuration
//3.modify ZQCTL0 register configuration
//
//
//updater       :Wu Youfei
//date          :2016/07/28
//version       :1.3
//note          :
//1.delete configuration of axi port rd/wr QoS configuration registers
//2.delete configuration of PWRCTL register
//3.delete useless while loop in the flow of initialization
//all of above modification is to save space of SRAM
//
//
//updater       :Wu Youfei
//date          :2016/08/03
//version       :1.4
//note          :
//1.add configuration sequence for FPGA(speed bin is DDR3_800_E)
//2.delete INITn register configration
//3.get FPGA define from gcc compile option
//-------------------------------------------------------------------------
//
//
//updater       :Wu Youfei
//date          :2016/08/17
//version       :1.5
//note          :
//1.add configuration sequence for FPGA in DLL_OFF mode(DDR3-250mbps)(
//2.in dll off mode CL=CWL=6 are fixed
//3.config MSTR(uMCTL2) and PUB_MR1 to enable dll off mode
//4.modify tREF_nom(tREFI,7.8us) tREF_min(tRFC,160ns for 2Gb SDRAM) tREF_max(9*7.8us)
//5.config PUB_PLLCR register to 0x00000001,please reference to  FPGA emulation DDR3/2 PHY databook
//
//
//updater       :Wu Youfei
//date          :2016/08/30
//version       :1.6
//note          :
//1.decrease PCFGW_5[9:0] and PCFGR_5[9:0] to raise the priority of axi port5
//2. decrease PERFLPR1[15:0](lpr_max_starve) to 0x20
//3.  decrease PERFWR1[15:0] (w_max_starve) to 0xf
//
//
//updater       :Wu Youfei
//date          :2016/08/31
//version       :1.7
//note          :
//1.config SCHED[1]=1(prefer_wr=1),under this configuration,when there is no pending WR trasaction, switch to RD
//
//updater       :Wu Youfei
//date          :2016/09/14
//version       :1.8
//note          :
//1.add 200Mbps for dll off mode
////-------------------------------------------------------------------------

//updater       :Wu Youfei
//date          :2016/09/19
//version       :1.9
//note          :
//1.add data trainning including write leveling,read dqs gate training,write leveling adjust,read/wr data bit deskew,wr/rd data eye training
//2.do reduce simulation time,the training procedure is off by default,if you want have a try,please open the macro TRAIN_ON.
////-------------------------------------------------------------------------
//
//updater       :Wu Youfei
//date          :2016/09/21
//version       :1.10
//note          :
//1. decrease PERFLPR1[15:0](lpr_max_starve) to 0x2
//2.  decrease PERFWR1[15:0] (w_max_starve) to 0x2
//
////-------------------------------------------------------------------------
//
//updater       :Wu Youfei
//date          :2016/12/09
//version       :1.11
//note          :
//1. add flag in SRAM to distinguish different initialization type
//2. when MC is power up when SDRAM is in self-refresh, do not reset PHY
//3. initialize DDR system when the whole chip power up ,you must reset all related module
//4. add speed bin 1600_K
////-------------------------------------------------------------------------
//
//updater       :Wu Youfei
//date          :2016/12/25
//version       :1.12
//note          :
//1.change PHY mode to RRMODE(rise-to-rise),in this mode,
//the posedge of PHY pll reference clock is align with pll output x1 clk(with little phase difference)
//
//updater       :Wu Youfei
//date          :2016/12/27
//version       :1.13
//note          :
//1.add V6 define in run_tb to distinguish Virtex6 and Virtex7 simulation
//1.get speed info from run_tb,not in this file
//
//updater       :Wu Youfei
//date          :2017/1/04
//version       :1.14
//note          :
//1.do not distinguish v6 and v7 for read DQS training
//
//updater       :Wu Youfei
//date          :2017/1/05
//version       :1.15
//note          :
//1.modify ADDRMAP regs configuration,modify row address range from 14 to 15bits ($$$$0105)
//2.delete include file define_ddr_func.h
//3.do not distinguish initialize type for FPGA
//
//updater       :Wu Youfei
//date          :2017/1/14
//version       :1.16
//note          :
//1.if ddr has already initialize,do not initialize,return immediately
//
//updater       :Wu Youfei
//date          :2017/5/10
//version       :1.17
//note          :
//1.map lcdc axi port to vpr to optimize QoS to avoid underflow
//
//updater       :Wu Youfei
//date          :2017/8/7
//version       :1.18
//note          :
//1.modify SWRC2[28] config,not reset timer2
//
//updater       :Wu Youfei
//date          :2017/11/27
//version       :1.19
//note          :
//1.modify ADDRMAP registers to enhance efficiency of accesing ddr
//
//updater       :Wu Youfei
//date          :2017/12/20
//version       :1.20
//note          :
//1.enable axi port 4/5;
//2.modify SWCR2 reg config
//#include "define_ddr_regs.h"
//#include "clib_define.h"
#include <asm/arch/ddr.h>
//FPGA mode
#define FPGA

//these macros is only used for FPGA
//dll off mode only support for FPGA
//#define DLL_ON_800 //800Mbps(speed bin 800_E)
//#define DLL_OFF_250//250Mbps
#define DLL_OFF_200//200Mbps
//#define DLL_OFF_50//50Mbps

//these macros is only used for ASIC
//speed bin for ASIC
//#define DDR3_1066_G
//#define DDR3_1333_H
//#define DDR3_1600_K


//train on macro
//#define TRAIN_ON
#define THIS_MODULE MODULE_DEV_DDR

int init_ddr(void){
#ifdef FPGA
    *(volatile int *)_FLAG_DDR_INIT=0x0;//do not distinguish for FPGA
    if(PUB_REG(PUB_PGSR0) == 0x9000005f) {
        return 0;
    }//if DDR is initialized,return

#if defined(DLL_OFF_50)
    //*************************************************//
    ////**************DDR3 50mbps configuration************//
    ////ctl_clk(MC)=12.5MHz
    //*************************************************//
    //**************reset all related module************//
    CHIP_CTRL_REG(0x3c) = CHIP_CTRL_REG(0x3c)&0xffff;
    CHIP_CTRL_REG(0x40) = CHIP_CTRL_REG(0x40)&0xf1ffffff;
    //***********de-reset presetn0(ddrc presetn******************)//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;
    //******************start configuration******************//
    DDR_REG(DBG1)=0x00000001;
    //  DDR_REG(MSTR)=0x00040201;
    DDR_REG(MSTR)=0x00048201;//dll_off_mode=1
    //  DDR_REG(MSTR)=0x00040001;//disable Burst Chop
    DDR_REG(MRCTRL0)=0x0000e010;
    DDR_REG(MRCTRL1)=0x0000a085;
    DDR_REG(PWRTMG)=0x00360003;
    //  DDR_REG(HWLPCTL)=0x006e0002;
    DDR_REG(RFSHCTL0)=0x00216070;//keep refresh_margin=2 not modify
    DDR_REG(RFSHCTL3)=0x00000000;
    //  DDR_REG(RFSHTMG)=0x00300020;
    DDR_REG(RFSHTMG)=0x00030002;//ctl_clk=80ns RFC_nom( also called tREFI 7.8us) tRFC_min(160ns for 2Gb)
    //  DDR_REG(CRCPARCTL0)=0x00000000;
    DDR_REG(INIT0)=0x40080003;//skip_dram_init,sdram initialization is controlled by PHY
    //  DDR_REG(INIT1)=0x0003000f;
    //  DDR_REG(INIT3)=0x05210000;
    //  DDR_REG(INIT4)=0x00000000;
    //  DDR_REG(INIT5)=0x00090000;
    //  DDR_REG(DIMMCTL)=0x00000000;
    DDR_REG(DRAMTMG0)=0x070a0c07;
    DDR_REG(DRAMTMG1)=0x0005020b;
    DDR_REG(DRAMTMG2)=0x00000407;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x03020204;
    DDR_REG(DRAMTMG5)=0x04030302;
    DDR_REG(DRAMTMG8)=0x00007409;
    DDR_REG(DRAMTMG15)=0x00000000;

    //  DDR_REG(ZQCTL0)=0x00800020;
    DDR_REG(ZQCTL0)=0x0080002c;//change tZQCS from 0x20 to 0x2c(80ns/0.93ns(sg093)/2)
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03010101;
    DDR_REG(DFITMG1)=0x00030101;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1(bit 8)
    DDR_REG(DFIUPD0)=0x00400005;
    DDR_REG(DFIUPD1)=0x00180027;
    DDR_REG(DFIUPD2)=0x00000000;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(DFIPHYMSTR)=0x00000000;

    DDR_REG(ADDRMAP1)=0x00171717;//b2 b1 b0
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    //  DDR_REG(ADDRMAP1)=0x00000003;
    //  DDR_REG(ADDRMAP2)=0x01050400;
    //  DDR_REG(ADDRMAP3)=0x05000407;
    //  DDR_REG(ADDRMAP4)=0x00000f0f;
    //  DDR_REG(ADDRMAP5)=0x040f0504;
    //  DDR_REG(ADDRMAP6)=0x0f0f0606;
    //  DDR_REG(ADDRMAP9)=0x0b080608;
    //  DDR_REG(ADDRMAP10)=0x0b090705;
    //  DDR_REG(ADDRMAP11)=0x00000003;

    DDR_REG(ODTCFG)=0x05100840;
    DDR_REG(ODTMAP)=0x00000011;
    //  DDR_REG(SCHED)=0x303a0501;
    DDR_REG(SCHED)=0x303a0503;//prefer_wr=1
    DDR_REG(SCHED1)=0x0000003a;
    DDR_REG(PERFHPR1)=0x3b009fec;
    //  DDR_REG(PERFLPR1)=0x1100e4dc;
    //  DDR_REG(PERFWR1)=0xc7005733;
    //  DDR_REG(PERFLPR1)=0x40000020;//decrease lpr_max_starve to 0x20
    //  DDR_REG(PERFWR1)=0xc700000f;//decrease w_max_starve to 0xf
    DDR_REG(PERFLPR1)=0x40000002;//decrease lpr_max_starve to 0x2
    DDR_REG(PERFWR1)=0xc7000002;//decrease w_max_starve to 0x2

    DDR_REG(DBG0)=0x00000000;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;
    //  DDR_REG(POISONCFG)=0x00100000;

    DDR_REG(PCCFG)=0x00000110;
    DDR_REG(PCFGR_0)=0x000063bd;
    DDR_REG(PCFGW_0)=0x000000ba;
    DDR_REG(PCFGR_1)=0x00003258;
    DDR_REG(PCFGW_1)=0x000023fd;
    DDR_REG(PCFGR_2)=0x00003050;
    DDR_REG(PCFGW_2)=0x000043f5;
    DDR_REG(PCFGR_3)=0x000030d5;
    DDR_REG(PCFGW_3)=0x00003226;
    DDR_REG(PCFGR_4)=0x000072b1;
    DDR_REG(PCFGW_4)=0x000062ce;
    //  DDR_REG(PCFGR_5)=0x000060b5;
    //  DDR_REG(PCFGW_5)=0x000040db;
    DDR_REG(PCFGR_5)=0x00006088;
    DDR_REG(PCFGW_5)=0x000040c8;
    //************enable axi port **************//
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;
    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;
    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000010;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;

    PUB_REG(PUB_MR0)=0x00000521;
    //  PUB_REG(PUB_MR0)=0x00000520;//disable Burst Chop
    PUB_REG(PUB_MR1)=0x00000001;//DLL off enable
    PUB_REG(PUB_MR2)=0x00000000;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x550f6644;
    //  PUB_REG(PUB_DTPR1)=0x1a820280;
    PUB_REG(PUB_DTPR1)=0x1a802280;//tCK=40ns tFAW([10:5])=20CKS tRFC(160ns for 2Gb) miminum refresh interval
    PUB_REG(PUB_DTPR2)=0x10018c03;
    PUB_REG(PUB_PLLCR)=0x00000001;//PLLCR

    PUB_REG(PUB_DSGCR)=0xf0006407;
    //  PUB_REG(PUB_PGCR2)=0x00f80c30;//Note :not support fixed latency mode in emulation(for FPGA) PHY
    //  PUB_REG(PUB_PGCR2)=0x00f00c30;
    PUB_REG(PUB_PGCR2)=0x00f020b7;//max refresh interval=7.8us*9/tCK-400
    PUB_REG(PUB_PTR3)=0x1e3009e3;
    PUB_REG(PUB_PTR4)=0x08000c00;
    while((PUB_REG(PUB_PGSR0))!=0x9000000f);
    PUB_REG(PUB_PIR)=0x00000181;
    while((PUB_REG(PUB_PGSR0))!=0x9000001f);

    //****************Read DQS Gate Training*************//
    PUB_REG(PUB_PIR)=0x00000401;
    while(PUB_REG(PUB_PGSR0)!=0x9000005f);

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;

    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(SWCTL)=0x00000001;

    while((DDR_REG(SWSTAT))!=0x00000001);

    while((DDR_REG(STAT))!=0x00000001);

#elif defined(DLL_OFF_200)

    //*************************************************//
    ////**************DDR3 200mbps configuration************//
    ////ctl_clk(MC)=50MHz
    //*************************************************//
    //**************reset all related module************//
    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)&0xffff;

    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)&0xf1ffffff;

    //***********de-reset presetn0(ddrc presetn******************)//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;
    //******************start configuration******************//
    DDR_REG(DBG1)=0x00000001;
    //  DDR_REG(MSTR)=0x00040201;
    DDR_REG(MSTR)=0x00048201;//dll_off_mode=1
    //  DDR_REG(MSTR)=0x00040001;//disable Burst Chop
    DDR_REG(MRCTRL0)=0x0000e010;
    DDR_REG(MRCTRL1)=0x0000a085;
    DDR_REG(PWRTMG)=0x00360003;
    //  DDR_REG(HWLPCTL)=0x006e0002;
    DDR_REG(RFSHCTL0)=0x00216070;//keep refresh_margin=2 not modify
    DDR_REG(RFSHCTL3)=0x00000000;
    //  DDR_REG(RFSHTMG)=0x00300020;
    DDR_REG(RFSHTMG)=0x000c0008;//ctl_clk=20ns RFC_nom( also called tREFI 7.8us) tRFC_min(160ns for 2Gb)
    //  DDR_REG(CRCPARCTL0)=0x00000000;
    DDR_REG(INIT0)=0x40080003;//skip_dram_init,sdram initialization is controlled by PHY
    //  DDR_REG(INIT1)=0x0003000f;
    //  DDR_REG(INIT3)=0x05210000;
    //  DDR_REG(INIT4)=0x00000000;
    //  DDR_REG(INIT5)=0x00090000;
    //  DDR_REG(DIMMCTL)=0x00000000;
    DDR_REG(DRAMTMG0)=0x070a0c07;
    DDR_REG(DRAMTMG1)=0x0005020b;
    DDR_REG(DRAMTMG2)=0x00000407;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x03020204;
    DDR_REG(DRAMTMG5)=0x04030302;
    DDR_REG(DRAMTMG8)=0x00007409;
    DDR_REG(DRAMTMG15)=0x00000000;

    //  DDR_REG(ZQCTL0)=0x00800020;
    DDR_REG(ZQCTL0)=0x0080002c;//change tZQCS from 0x20 to 0x2c(80ns/0.93ns(sg093)/2)
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03010101;
    DDR_REG(DFITMG1)=0x00030101;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1(bit 8)
    DDR_REG(DFIUPD0)=0x00400005;
    DDR_REG(DFIUPD1)=0x00180027;
    DDR_REG(DFIUPD2)=0x00000000;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(DFIPHYMSTR)=0x00000000;
    DDR_REG(ADDRMAP1)=0x00080808;//b2 b1 b0 $$$$0105
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x07070707;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f070707;//r15 (not used,so config f) r14 r13 r12
    //  DDR_REG(ADDRMAP1)=0x00171717;//b2 b1 b0 $$$$0105
    //  DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    //  DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    //  DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    //  DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    //  DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    //  DDR_REG(ADDRMAP1)=0x00000003;
    //  DDR_REG(ADDRMAP2)=0x01050400;
    //  DDR_REG(ADDRMAP3)=0x05000407;
    //  DDR_REG(ADDRMAP4)=0x00000f0f;
    //  DDR_REG(ADDRMAP5)=0x040f0504;
    //  DDR_REG(ADDRMAP6)=0x0f0f0606;
    //  DDR_REG(ADDRMAP9)=0x0b080608;
    //  DDR_REG(ADDRMAP10)=0x0b090705;
    //  DDR_REG(ADDRMAP11)=0x00000003;

    DDR_REG(ODTCFG)=0x05100840;
    DDR_REG(ODTMAP)=0x00000011;
    //  DDR_REG(SCHED)=0x303a0501;
    DDR_REG(SCHED)=0x303a0503;//prefer_wr=1
    DDR_REG(SCHED1)=0x0000003a;
    DDR_REG(PERFHPR1)=0x3b009fec;
    //  DDR_REG(PERFLPR1)=0x1100e4dc;
    //  DDR_REG(PERFWR1)=0xc7005733;
    //  DDR_REG(PERFLPR1)=0x40000020;//decrease lpr_max_starve to 0x20
    //  DDR_REG(PERFWR1)=0xc700000f;//decrease w_max_starve to 0xf
    DDR_REG(PERFLPR1)=0x40000002;//decrease lpr_max_starve to 0x2
    DDR_REG(PERFWR1)=0xc7000002;//decrease w_max_starve to 0x2
    DDR_REG(DBG0)=0x00000000;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;

    //  DDR_REG(POISONCFG)=0x00100000;
    DDR_REG(PERFVPR1)=0x10;
    //***map lcdc port to vpr(Read)******//
    DDR_REG(PCFGQOS0_2)=0x00100000;//map region1 to vpr
    DDR_REG(PCFGQOS1_2)=0x00000008;//bule queue timeout value=8

    ////***map dx bar port to vpr(Read)******//
    //  DDR_REG(PCFGQOS0_3)=0x00100000;//map region1 to vpr
    //  DDR_REG(PCFGQOS1_3)=0x00000008;//bule queue timeout value=8
    ////***map dx bar port to vpr(Write)******//
    //  DDR_REG(PCFGWQOS0_3)=0x00100000;//map region1 to vpr
    //  DDR_REG(PCFGWQOS1_3)=0x00000008;//bule queue timeout value=8

    DDR_REG(PCCFG)=0x00000110;

    DDR_REG(PCFGR_0)=0x000063bd;
    DDR_REG(PCFGW_0)=0x000000ba;
    DDR_REG(PCFGR_1)=0x00003258;
    DDR_REG(PCFGW_1)=0x000023fd;
    DDR_REG(PCFGR_2)=0x00003050;
    DDR_REG(PCFGW_2)=0x000043f5;
    DDR_REG(PCFGR_3)=0x000030d5;
    DDR_REG(PCFGW_3)=0x00003226;
    DDR_REG(PCFGR_4)=0x000072b1;
    DDR_REG(PCFGW_4)=0x000062ce;
    //  DDR_REG(PCFGR_5)=0x000060b5;
    //  DDR_REG(PCFGW_5)=0x000040db;
    DDR_REG(PCFGR_5)=0x00006088;
    DDR_REG(PCFGW_5)=0x000040c8;
    //************enable axi port **************//
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;
    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;

    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000010;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;
    PUB_REG(PUB_MR0)=0x00000521;
    //  PUB_REG(PUB_MR0)=0x00000520;//disable Burst Chop
    PUB_REG(PUB_MR1)=0x00000001;//DLL off enable
    PUB_REG(PUB_MR2)=0x00000000;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x550f6644;
    //  PUB_REG(PUB_DTPR1)=0x1a820280;
    PUB_REG(PUB_DTPR1)=0x1a808280;//ctl_clk=20ns tFAW=20CKS tRFC(160ns for 2Gb) miminum refresh interval
    PUB_REG(PUB_DTPR2)=0x10018c03;

    PUB_REG(PUB_PLLCR)=0x00000001;//PLLCR

    PUB_REG(PUB_DSGCR)=0xf0006407;
    //  PUB_REG(PUB_PGCR2)=0x00f80c30;//Note :not support fixed latency mode in emulation(for FPGA) PHY
    //  PUB_REG(PUB_PGCR2)=0x00f00c30;
    PUB_REG(PUB_PGCR2)=0x00f020b7;//max refresh interval=7.8us*9/tCK-400
    PUB_REG(PUB_PTR3)=0x1e3009e3;
    PUB_REG(PUB_PTR4)=0x08000c00;

    while(PUB_REG(PUB_PGSR0)!=0x9000000f);
    PUB_REG(PUB_PIR)=0x00000181;
    while(PUB_REG(PUB_PGSR0)!=0x9000001f);

    //****************Read DQS Gate Training*************//
    PUB_REG(PUB_PIR)=0x00000401;
    while(PUB_REG(PUB_PGSR0)!=0x9000005f);

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;

    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(SWCTL)=0x00000001;

    while(DDR_REG(SWSTAT)!=0x00000001);

    while(DDR_REG(STAT)!=0x00000001);

    //  PUB_REG(PUB_DX0BDLR1)=0x25;
    //  PUB_REG(PUB_DX1BDLR1)=0x25;
    //  PUB_REG(PUB_DX2BDLR1)=0x25;
    //  PUB_REG(PUB_DX3BDLR1)=0x25;

    //  PUB_REG(PUB_DX0BDLR2)=0x6;
    //  PUB_REG(PUB_DX1BDLR2)=0x6;
    //  PUB_REG(PUB_DX2BDLR2)=0x6;
    //  PUB_REG(PUB_DX3BDLR2)=0x6;

    //  PUB_REG(PUB_DX0BDLR4)=0x00180000;
    //  PUB_REG(PUB_DX1BDLR4)=0x00180000;
    //  PUB_REG(PUB_DX2BDLR4)=0x00180000;
    //  PUB_REG(PUB_DX3BDLR4)=0x00180000;
#elif defined(DLL_OFF_250)
    //*************************************************//
    ////**************DDR3 250mbps configuration************//
    //*************************************************//
    //**************reset all related module************//
    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)&0xffff;
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)&0xf1ffffff;
    //***********de-reset presetn0(ddrc presetn******************)//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;
    //******************start configuration******************//
    DDR_REG(DBG1)=0x00000001;
    //  DDR_REG(MSTR)=0x00040201;
    DDR_REG(MSTR)=0x00048201;//dll_off_mode=1
    //  DDR_REG(MSTR)=0x00040001;//disable Burst Chop
    DDR_REG(MRCTRL0)=0x0000e010;
    DDR_REG(MRCTRL1)=0x0000a085;
    DDR_REG(PWRTMG)=0x00360003;
    //  DDR_REG(HWLPCTL)=0x006e0002;
    DDR_REG(RFSHCTL0)=0x00216070;//keep refresh_margin=2 not modify
    DDR_REG(RFSHCTL3)=0x00000000;
    //  DDR_REG(RFSHTMG)=0x00300020;
    DDR_REG(RFSHTMG)=0x000f000a;//tRFC_nom(also called tREFI 7.8us) tRFC_min(160ns for 2Gb)
    //  DDR_REG(CRCPARCTL0)=0x00000000;
    DDR_REG(INIT0)=0x40080003;//skip_dram_init,sdram initialization is controlled by PHY
    //  DDR_REG(INIT1)=0x0003000f;
    //  DDR_REG(INIT3)=0x05210000;
    //  DDR_REG(INIT4)=0x00000000;
    //  DDR_REG(INIT5)=0x00090000;
    //  DDR_REG(DIMMCTL)=0x00000000;
    DDR_REG(DRAMTMG0)=0x070a0c07;
    DDR_REG(DRAMTMG1)=0x0005020b;
    DDR_REG(DRAMTMG2)=0x00000407;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x03020204;
    DDR_REG(DRAMTMG5)=0x04030302;
    DDR_REG(DRAMTMG8)=0x00007409;
    DDR_REG(DRAMTMG15)=0x00000000;

    //  DDR_REG(ZQCTL0)=0x00800020;
    DDR_REG(ZQCTL0)=0x0080002c;//change tZQCS from 0x20 to 0x2c(80ns/0.93ns(sg093)/2)
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03010101;
    DDR_REG(DFITMG1)=0x00030101;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1(bit 8)
    DDR_REG(DFIUPD0)=0x00400005;
    DDR_REG(DFIUPD1)=0x00180027;
    DDR_REG(DFIUPD2)=0x00000000;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(DFIPHYMSTR)=0x00000000;

    DDR_REG(ADDRMAP1)=0x00171717;//b2 b1 b0
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    //  DDR_REG(ADDRMAP1)=0x00000003;
    //  DDR_REG(ADDRMAP2)=0x01050400;
    //  DDR_REG(ADDRMAP3)=0x05000407;
    //  DDR_REG(ADDRMAP4)=0x00000f0f;
    //  DDR_REG(ADDRMAP5)=0x040f0504;
    //  DDR_REG(ADDRMAP6)=0x0f0f0606;
    //  DDR_REG(ADDRMAP9)=0x0b080608;
    //  DDR_REG(ADDRMAP10)=0x0b090705;
    //  DDR_REG(ADDRMAP11)=0x00000003;

    DDR_REG(ODTCFG)=0x05100840;
    DDR_REG(ODTMAP)=0x00000011;
    //  DDR_REG(SCHED)=0x303a0501;
    DDR_REG(SCHED)=0x303a0503;//prefer_wr=1
    DDR_REG(SCHED1)=0x0000003a;
    DDR_REG(PERFHPR1)=0x3b009fec;
    //  DDR_REG(PERFLPR1)=0x1100e4dc;
    //  DDR_REG(PERFWR1)=0xc7005733;
    //  DDR_REG(PERFLPR1)=0x40000020;//decrease lpr_max_starve to 0x20
    //  DDR_REG(PERFWR1)=0xc700000f;//decrease w_max_starve to 0xf
    DDR_REG(PERFLPR1)=0x40000002;//decrease lpr_max_starve to 0x2
    DDR_REG(PERFWR1)=0xc7000002;//decrease w_max_starve to 0x2

    DDR_REG(DBG0)=0x00000000;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;

    //  DDR_REG(POISONCFG)=0x00100000;

    DDR_REG(PCCFG)=0x00000110;

    DDR_REG(PCFGR_0)=0x000063bd;
    DDR_REG(PCFGW_0)=0x000000ba;
    DDR_REG(PCFGR_1)=0x00003258;
    DDR_REG(PCFGW_1)=0x000023fd;
    DDR_REG(PCFGR_2)=0x00003050;
    DDR_REG(PCFGW_2)=0x000043f5;
    DDR_REG(PCFGR_3)=0x000030d5;
    DDR_REG(PCFGW_3)=0x00003226;
    DDR_REG(PCFGR_4)=0x000072b1;
    DDR_REG(PCFGW_4)=0x000062ce;
    //  DDR_REG(PCFGR_5)=0x000060b5;
    //  DDR_REG(PCFGW_5)=0x000040db;
    DDR_REG(PCFGR_5)=0x00006088;
    DDR_REG(PCFGW_5)=0x000040c8;
    //************enable axi port **************//
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;
    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;

    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000010;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;

    PUB_REG(PUB_MR0)=0x00000521;
    //  PUB_REG(PUB_MR0)=0x00000520;//disable Burst Chop
    PUB_REG(PUB_MR1)=0x00000001;//DLL off enable
    PUB_REG(PUB_MR2)=0x00000000;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x550f6644;
    //  PUB_REG(PUB_DTPR1)=0x1a820280;
    PUB_REG(PUB_DTPR1)=0x1a80a280;//tRFC(160ns for 2Gb) miminum refresh interval
    PUB_REG(PUB_DTPR2)=0x10018c03;

    PUB_REG(PUB_PLLCR)=0x00000001;//PLLCR

    PUB_REG(PUB_DSGCR)=0xf0006407;
    //  PUB_REG(PUB_PGCR2)=0x00f80c30;//Note :not support fixed latency mode in emulation(for FPGA) PHY
    //  PUB_REG(PUB_PGCR2)=0x00f00c30;
    PUB_REG(PUB_PGCR2)=0x00f020b7;//max refresh interval=7.8us*9/tCK-400
    PUB_REG(PUB_PTR3)=0x1e3009e3;
    PUB_REG(PUB_PTR4)=0x08000c00;
    while((PUB_REG(PUB_PGSR0))!=0x9000000f);
    PUB_REG(PUB_PIR)=0x00000181;
    while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    //****************Read DQS Gate Training*************//
    PUB_REG(PUB_PIR)=0x00000401;
    while(PUB_REG(PUB_PGSR0)!=0x9000005f);

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;

    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(SWCTL)=0x00000001;

    while((DDR_REG(SWSTAT))!=0x00000001);

    while((DDR_REG(STAT))!=0x00000001);

#elif defined(DLL_ON_800)
    //*************************************************//
    ////**************DDR3_800E configuration************//
    //*************************************************//

    //**************reset all related module************//
    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)&0xffff;

    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)&0xf1ffffff;
    //***********de-reset presetn0(ddrc presetn******************)//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;
    //******************start configuration******************//
    DDR_REG(DBG1)=0x00000001;
    DDR_REG(MSTR)=0x00040201;
    //  DDR_REG(MSTR)=0x00040001;//disable Burst Chop
    DDR_REG(MRCTRL0)=0x0000e010;
    DDR_REG(MRCTRL1)=0x0000a085;
    DDR_REG(PWRTMG)=0x00360003;
    //  DDR_REG(HWLPCTL)=0x006e0002;
    DDR_REG(RFSHCTL0)=0x00e16070;
    DDR_REG(RFSHCTL3)=0x00000000;
    DDR_REG(RFSHTMG)=0x00300020;
    //  DDR_REG(CRCPARCTL0)=0x00000000;
    DDR_REG(INIT0)=0x40080003;//skip_dram_init,sdram initialization is controlled by PHY
    //  DDR_REG(INIT1)=0x0003000f;
    //  DDR_REG(INIT3)=0x05210000;
    //  DDR_REG(INIT4)=0x00000000;
    //  DDR_REG(INIT5)=0x00090000;
    //  DDR_REG(DIMMCTL)=0x00000000;
    DDR_REG(DRAMTMG0)=0x070a0c07;
    DDR_REG(DRAMTMG1)=0x0005020b;
    DDR_REG(DRAMTMG2)=0x00000407;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x03020204;
    DDR_REG(DRAMTMG5)=0x04030302;
    DDR_REG(DRAMTMG8)=0x00007409;
    DDR_REG(DRAMTMG15)=0x00000000;

    //  DDR_REG(ZQCTL0)=0x00800020;
    DDR_REG(ZQCTL0)=0x0080002c;//change tZQCS from 0x20 to 0x2c(80ns/0.93ns(sg093)/2)
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03010101;
    DDR_REG(DFITMG1)=0x00030101;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1i(bit 8)
    DDR_REG(DFIUPD0)=0x00400005;
    DDR_REG(DFIUPD1)=0x00180027;
    DDR_REG(DFIUPD2)=0x00000000;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(DFIPHYMSTR)=0x00000000;

    DDR_REG(ADDRMAP1)=0x00171717;//b2 b1 b0
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    //  DDR_REG(ADDRMAP1)=0x00000003;
    //  DDR_REG(ADDRMAP2)=0x01050400;
    //  DDR_REG(ADDRMAP3)=0x05000407;
    //  DDR_REG(ADDRMAP4)=0x00000f0f;
    //  DDR_REG(ADDRMAP5)=0x040f0504;
    //  DDR_REG(ADDRMAP6)=0x0f0f0606;
    //  DDR_REG(ADDRMAP9)=0x0b080608;
    //  DDR_REG(ADDRMAP10)=0x0b090705;
    //  DDR_REG(ADDRMAP11)=0x00000003;

    DDR_REG(ODTCFG)=0x05100840;
    DDR_REG(ODTMAP)=0x00000011;
    //  DDR_REG(SCHED)=0x303a0501;
    DDR_REG(SCHED)=0x303a0503;//prefer_wr=1
    DDR_REG(SCHED1)=0x0000003a;
    DDR_REG(PERFHPR1)=0x3b009fec;
    //  DDR_REG(PERFLPR1)=0x1100e4dc;
    //  DDR_REG(PERFWR1)=0xc7005733;
    //  DDR_REG(PERFLPR1)=0x40000020;//decrease lpr_max_starve to 0x20
    //  DDR_REG(PERFWR1)=0xc700000f;//decrease w_max_starve to 0xf
    DDR_REG(PERFLPR1)=0x40000002;//decrease lpr_max_starve to 0x2
    DDR_REG(PERFWR1)=0xc7000002;//decrease w_max_starve to 0x2

    DDR_REG(DBG0)=0x00000000;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;

    //  DDR_REG(POISONCFG)=0x00100000;

    DDR_REG(PCCFG)=0x00000110;

    DDR_REG(PCFGR_0)=0x000063bd;
    DDR_REG(PCFGW_0)=0x000000ba;
    DDR_REG(PCFGR_1)=0x00003258;
    DDR_REG(PCFGW_1)=0x000023fd;
    DDR_REG(PCFGR_2)=0x00003050;
    DDR_REG(PCFGW_2)=0x000043f5;
    DDR_REG(PCFGR_3)=0x000030d5;
    DDR_REG(PCFGW_3)=0x00003226;
    DDR_REG(PCFGR_4)=0x000072b1;
    DDR_REG(PCFGW_4)=0x000062ce;
    //  DDR_REG(PCFGR_5)=0x000060b5;
    //  DDR_REG(PCFGW_5)=0x000040db;
    DDR_REG(PCFGR_5)=0x00006088;
    DDR_REG(PCFGW_5)=0x000040c8;
    //************enable axi port **************//
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;
    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;

    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000010;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;

    PUB_REG(PUB_MR0)=0x00000521;
    //  PUB_REG(PUB_MR0)=0x00000520;//disable Burst Chop
    PUB_REG(PUB_MR1)=0x00000000;
    PUB_REG(PUB_MR2)=0x00000000;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x550f6644;
    PUB_REG(PUB_DTPR1)=0x1a820280;
    PUB_REG(PUB_DTPR2)=0x10018c03;

    PUB_REG(PUB_DSGCR)=0xf0006407;
    //  PUB_REG(PUB_PGCR2)=0x00f80c30;//Note :not support fixed latency mode in emulation(for FPGA) PHY
    PUB_REG(PUB_PGCR2)=0x00f00c30;
    PUB_REG(PUB_PTR3)=0x1e3009e3;
    PUB_REG(PUB_PTR4)=0x08000c00;
    while((PUB_REG(PUB_PGSR0))!=0x9000000f);
    PUB_REG(PUB_PIR)=0x00000181;
    while((PUB_REG(PUB_PGSR0))!=0x9000001f);

    //****************Read DQS Gate Training*************//
    PUB_REG(PUB_PIR)=0x00000401;
    while(PUB_REG(PUB_PGSR0)!=0x9000005f);

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;

    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(SWCTL)=0x00000001;

    while((DDR_REG(SWSTAT))!=0x00000001);

    while((DDR_REG(STAT))!=0x00000001);
#endif

#else //ASIC
    //*******select core_ddrc_clk and phy clk(ctl_clk)*******************//
#if defined(DDR3_1066_G)
    CHIP_CTRL_REG(0x54)=CHIP_CTRL_REG(0x54)&0x8fffffff|0x30000000;//sel 266MHz
#elif defined(DDR3_1333_H)
    CHIP_CTRL_REG(0x54)=CHIP_CTRL_REG(0x54)&0x8fffffff|0x20000000;//sel 333MHz
#elif defined(DDR3_1600_K)
    CHIP_CTRL_REG(0x54)=CHIP_CTRL_REG(0x54)&0x8fffffff|0x10000000;//sel 400MHz
#endif


    //*************wait ddr pll lock**********************//
    while((CHIP_CTRL_REG(0x110)&0x1)!=0x1);//wait ddr pll lock

    if((*((volatile int *)_FLAG_DDR_INIT))==0x0){
        //**************reset all related module************//
        CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)&0xffff;
        CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)&0xf1ffffff;
    } else {
        //**************reset all related module except phy apb and phy rst************//
        CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)&0x20ffff;

        CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)&0xfdffffff;
    }

#if defined(DDR3_1066_G)
    ///***********config ddrc and phy sequence**************////////
    ///**********speed bin 1066_G 8-8-8***************//
    //***********de-reset presetn0(ddrc presetn******************)//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;

    DDR_REG(DBG1)=0x00000001;
    //  DDR_REG(PWRCTL)=0x00000001;
    DDR_REG(MSTR)=0x00040201;
    DDR_REG(MRCTRL0)=0x00003010;
    DDR_REG(MRCTRL1)=0x0000255e;
    //  DDR_REG(PWRCTL)=0x00000002;
    DDR_REG(PWRTMG)=0x00360003;
    //  DDR_REG(HWLPCTL)=0x006e0002;
    //  DDR_REG(RFSHCTL0)=0x00e16070;
    DDR_REG(RFSHCTL0)=0x00e06010;//modify refresh_to_x32=0x6
    DDR_REG(RFSHCTL3)=0x00000000;
    DDR_REG(RFSHTMG)=0x0040002b;
    //  DDR_REG(CRCPARCTL0)=0x00000000;

    if((*((volatile int *)_FLAG_DDR_INIT))==0x0) {
        DDR_REG(INIT0)=0x40080003;
    } else {
        DDR_REG(INIT0)=0xc0080003;//power-up in self-refresh mode
        DDR_REG(PWRCTL)=0x00000020;//stay in self-refresh
    }
    //  DDR_REG(INIT1)=0x0003000f;
    //  DDR_REG(INIT3)=0x09410000;
    //  DDR_REG(INIT4)=0x00080000;
    //  DDR_REG(INIT5)=0x00090000;
    //  DDR_REG(DIMMCTL)=0x00000000;

    DDR_REG(DRAMTMG0)=0x090e110a;
    DDR_REG(DRAMTMG1)=0x0007020e;
    DDR_REG(DRAMTMG2)=0x00000407;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x04020305;
    DDR_REG(DRAMTMG5)=0x04030202;
    DDR_REG(DRAMTMG8)=0x00007409;
    DDR_REG(DRAMTMG15)=0x00000000;

    //  DDR_REG(ZQCTL0)=0x00800020;
    DDR_REG(ZQCTL0)=0x01000040;//modify tZQCS  from 0x20 to 0x40 core_ddrc_clk modify tZQoper from 0x80 to 0x100
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03020101;
    DDR_REG(DFITMG1)=0x00030101;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1(bit 8)
    DDR_REG(DFIUPD0)=0x00400005;
    DDR_REG(DFIUPD1)=0x00180027;
    DDR_REG(DFIUPD2)=0x80000000;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(DFIPHYMSTR)=0x00000000;

    //******config hif addr map to sdram addr(ba,row,col)************//
    //  DDR_REG(ADDRMAP1)=0x00161616;//b2 b1 b0
    //  DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    //  DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    //  DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    //  DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    //  DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    DDR_REG(ADDRMAP1)=0x00080808;//b2 b1 b0 $$$$0105
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x07070707;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f070707;//r15 (not used,so config f) r14 r13 r12

    DDR_REG(ODTCFG)=0x05100840;
    DDR_REG(ODTMAP)=0x00000001;
    //  DDR_REG(SCHED)=0x303a0501;
    DDR_REG(SCHED)=0x303a0503;//prefer_wr=1
    DDR_REG(SCHED1)=0x0000003a;
    DDR_REG(PERFHPR1)=0x3b009fec;
    //  DDR_REG(PERFLPR1)=0x1100e4dc;
    //  DDR_REG(PERFWR1)=0xc7005733;
    //  DDR_REG(PERFLPR1)=0x40000020;//decrease lpr_max_starve to 0x20
    //  DDR_REG(PERFWR1)=0xc700000f;//decrease w_max_starve to 0xf
    DDR_REG(PERFLPR1)=0x40000002;//decrease lpr_max_starve to 0x2
    DDR_REG(PERFWR1)=0xc7000002;//decrease w_max_starve to 0x2

    DDR_REG(DBG0)=0x00000000;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;
    //  DDR_REG(POISONCFG)=0x00010011;

    //****************config QoS of axi port*************//
    // qos signal of axi port is tied to 0,so it is not necessary to configa QoS registers//

    //*************config configuration rd/wr register of axi port***********//
    DDR_REG(PCCFG)=0x00000110;

    DDR_REG(PCFGR_0)=0x0000123f;
    DDR_REG(PCFGW_0)=0x0000701c;
    DDR_REG(PCFGR_1)=0x0000216e;
    DDR_REG(PCFGW_1)=0x00005344;
    DDR_REG(PCFGR_2)=0x0000614a;
    DDR_REG(PCFGW_2)=0x00004323;
    DDR_REG(PCFGR_3)=0x000020ef;
    DDR_REG(PCFGW_3)=0x00006223;
    DDR_REG(PCFGR_4)=0x00006181;
    DDR_REG(PCFGW_4)=0x000051e0;
    //  DDR_REG(PCFGR_5)=0x0000730c;
    //  DDR_REG(PCFGW_5)=0x00005398;
    DDR_REG(PCFGR_5)=0x00007008;
    DDR_REG(PCFGW_5)=0x00005008;

    //****************enable axi port******************//
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;

    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;

    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000010;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;

    PUB_REG(PUB_MR0)=0x00000941;
    PUB_REG(PUB_MR1)=0x00000000;
    PUB_REG(PUB_MR2)=0x00000008;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x71948844;
    PUB_REG(PUB_DTPR1)=0x1a82b360;
    PUB_REG(PUB_DTPR2)=0x10019004;
    PUB_REG(PUB_DSGCR)=0xf0046407;//rrmode=1
    //  PUB_REG(PUB_DSGCR)=0xf0006407;
    PUB_REG(PUB_PGCR2)=0x00f8103d;

    PUB_REG(PUB_PTR3)=0x0a100a46;//for reduce simulation time,config smaller value than 500us,which will lead to timing violation in ddr3 modle(ddr3.v)
    //  PUB_REG(PUB_PTR3)=0x0a14111d;//tDINIT0=500us=PUB_PTR3[19:0]*tCK

    PUB_REG(PUB_PTR4)=0x08000c00;//for reduce simulation time,config smaller value than 200us,which will lead to timing violation in ddr3 modle(ddr3.v)
    //  PUB_REG(PUB_PTR4)=0x0801a072;//tDINIT2=200us=PUB_PTR4[17:0]*tCK(reference PUB databook)

    if(*((volatile int *)_FLAG_DDR_INIT)==0x0) { //default do sdram initialization
        //*************wait PHY initialization Done*****************//
        while((PUB_REG(PUB_PGSR0))!=0x9000000f);

        //****************inform PHY to start initialization of SDRAM******//
        PUB_REG(PUB_PIR)=0x00000181;
        //***************wait SDRAM initialization Done*********//
        while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    } else {
        //do not initialize SDRAM when power up because SDRAM is in  self-refresh mode
        //*************wait PHY initialization Done*****************//
        while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    }

#ifdef TRAIN_ON
    if(*((volatile int *)_FLAG_DDR_INIT)==0x0) { //default do sdram initialization
        //************data training configuration**********************//
        PUB_REG(PUB_DTCR)=PUB_REG(PUB_DTCR)&0xfffffff0|0x1;//DTRPTN=1
        DDR_REG(RFSHCTL3)=0x00000001;//disable auto-refresh
        PUB_REG(PUB_PIR)=0x0000fe01;//run Training
        //***************wait Training Done*********//
        while((PUB_REG(PUB_PGSR0))!=0x90000fff);
        DDR_REG(RFSHCTL3)=0x00000000;//enable auto-refresh
    }
#endif

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(SWCTL)=0x00000001;

    while((DDR_REG(SWSTAT))!=0x00000001);

    *((volatile int *)_RETENTION_CTRL_REG)=0x0;
    *((volatile int *)_RETENTION_CTRL_REG)=0x0;//remove latch to SDRAM before exit self-refresh
    DDR_REG(PWRCTL)=0x00000000;//exit self-refresh

    //***********wait the ddr system be ready to  access*********//
    while((DDR_REG(STAT))!=0x00000001);

#elif defined(DDR3_1333_H)
    //*******config ddrc and phy sequence***************////////
    ///speed bin 1333-H 9-9-9////////
    //***********de-reset presetn0(ddrc presetn******************)//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;

    DDR_REG(DBG1)=0x00000001;

    DDR_REG(MSTR)=0x00040201;
    DDR_REG(MRCTRL0)=0x00003010;
    DDR_REG(MRCTRL1)=0x0000255e;
    //  DDR_REG(PWRCTL)=0x00000002;
    DDR_REG(PWRTMG)=0x00360003;
    //  DDR_REG(HWLPCTL)=0x006e0002;
    //  DDR_REG(RFSHCTL0)=0x00e16070;
    DDR_REG(RFSHCTL0)=0x00e06010;//modify refresh_to_x32=0x6
    DDR_REG(RFSHCTL3)=0x00000000;
    DDR_REG(RFSHTMG)=0x00510036;
    //  DDR_REG(CRCPARCTL0)=0x00000000;
    if((*((volatile int *)_FLAG_DDR_INIT))==0x0) {
        DDR_REG(INIT0)=0x40080003;
    } else {
        DDR_REG(INIT0)=0xc0080003;//power-up in self-refresh mode
        DDR_REG(PWRCTL)=0x00000020;//stay in self-refresh
    }
    //  DDR_REG(INIT1)=0x0003000f;
    //  DDR_REG(INIT3)=0x0b510000;
    //  DDR_REG(INIT4)=0x00100000;
    //  DDR_REG(INIT5)=0x00090000;
    //  DDR_REG(DIMMCTL)=0x00000000;

    DDR_REG(DRAMTMG0)=0x0a0f150c;
    DDR_REG(DRAMTMG1)=0x00080211;
    DDR_REG(DRAMTMG2)=0x00000408;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x05020305;
    DDR_REG(DRAMTMG5)=0x0b040302;
    DDR_REG(DRAMTMG8)=0x00007409;
    DDR_REG(DRAMTMG15)=0x00000000;

    //  DDR_REG(ZQCTL0)=0x00800020;
    DDR_REG(ZQCTL0)=0x01000040;//modify tZQCS  from 0x20 to 0x40 core_ddrc_clk modify tZQoper from 0x80 to 0x100
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03030102;
    DDR_REG(DFITMG1)=0x00030101;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1(bit 8)
    DDR_REG(DFIUPD0)=0x00400005;
    DDR_REG(DFIUPD1)=0x00180027;
    DDR_REG(DFIUPD2)=0x80000000;
    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(DFIPHYMSTR)=0x00000000;

    DDR_REG(ADDRMAP1)=0x00161616;//b2 b1 b0
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    DDR_REG(ODTCFG)=0x0510083c;
    DDR_REG(ODTMAP)=0x00000001;
    //  DDR_REG(SCHED)=0x303a0501;
    DDR_REG(SCHED)=0x303a0503;//prefer_wr=1
    DDR_REG(SCHED1)=0x0000003a;
    DDR_REG(PERFHPR1)=0x3b009fec;
    //  DDR_REG(PERFLPR1)=0x1100e4dc;
    //  DDR_REG(PERFWR1)=0xc7005733;
    //  DDR_REG(PERFLPR1)=0x40000020;//decrease lpr_max_starve to 0x20
    //  DDR_REG(PERFWR1)=0xc700000f;//decrease w_max_starve to 0xf
    DDR_REG(PERFLPR1)=0x40000002;//decrease lpr_max_starve to 0x2
    DDR_REG(PERFWR1)=0xc7000002;//decrease w_max_starve to 0x2

    DDR_REG(DBG0)=0x00000000;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;
    //  DDR_REG(POISONCFG)=0x00010011;

    //****************config QoS of axi port*************//
    // qos signal of axi port is tied to 0,so it is not necessary to configa QoS registers//
    //NA
    //*************config configuration rd/wr register of axi port***********//
    DDR_REG(PCCFG)=0x00000110;

    DDR_REG(PCFGR_0)=0x0000123f;
    DDR_REG(PCFGW_0)=0x0000701c;
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCFGR_1)=0x0000216e;
    DDR_REG(PCFGW_1)=0x00005344;
    DDR_REG(PCFGR_2)=0x0000614a;
    DDR_REG(PCFGW_2)=0x00004323;
    DDR_REG(PCFGR_3)=0x000020ef;
    DDR_REG(PCFGW_3)=0x00006223;
    DDR_REG(PCFGR_4)=0x00006181;
    DDR_REG(PCFGW_4)=0x000051e0;
    //  DDR_REG(PCFGR_5)=0x0000730c;
    //  DDR_REG(PCFGW_5)=0x00005398;
    DDR_REG(PCFGR_5)=0x00007008;
    DDR_REG(PCFGW_5)=0x00005008;
    //****************enable axi port******************//
    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;

    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;
    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000010;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;

    PUB_REG(PUB_MR0)=0x00000b51;
    PUB_REG(PUB_MR1)=0x00000000;
    PUB_REG(PUB_MR2)=0x00000010;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x85589955;
    PUB_REG(PUB_DTPR1)=0x1a835bc0;
    PUB_REG(PUB_DTPR2)=0x10021004;

    PUB_REG(PUB_DSGCR)=0xf0046407;//rrmode=1
    //  PUB_REG(PUB_DSGCR)=0xf0006407;
    PUB_REG(PUB_PGCR2)=0x00f81450;

    PUB_REG(PUB_PTR3)=0x0b800a46;
    PUB_REG(PUB_PTR4)=0x08000c00;

    if(*((volatile int *)_FLAG_DDR_INIT)==0x0) { //default do sdram initialization
        //*************wait PHY initialization Done*****************//
        while((PUB_REG(PUB_PGSR0))!=0x9000000f);

        //****************inform PHY to start initialization of SDRAM******//
        PUB_REG(PUB_PIR)=0x00000181;
        //***************wait SDRAM initialization Done*********//
        while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    } else {
        //do not initialize SDRAM when power up because SDRAM is in  self-refresh mode
        //*************wait PHY initialization Done*****************//
        while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    }

#ifdef TRAIN_ON

    if(*((volatile int *)_FLAG_DDR_INIT)==0x0) { //default do sdram initialization
        //************data training configuration**********************//
        PUB_REG(PUB_DTCR)=PUB_REG(PUB_DTCR)&0xfffffff0|0x1;//DTRPTN=1
        DDR_REG(RFSHCTL3)=0x00000001;//disable auto-refresh
        PUB_REG(PUB_PIR)=0x0000fe01;//run Training
        //***************wait Training Done*********//
        while((PUB_REG(PUB_PGSR0))!=0x90000fff);
        DDR_REG(RFSHCTL3)=0x00000000;//enable auto-refresh
    }
#endif

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;

    DDR_REG(DFIMISC)=0x00000011;
    DDR_REG(SWCTL)=0x00000001;

    while((DDR_REG(SWSTAT))!=0x00000001);

    *((volatile int *)_RETENTION_CTRL_REG)=0x0;//remove latch to SDRAM before exit self-refresh
    *((volatile int *)_RETENTION_CTRL_REG)=0x0;//repeat one time to delay
    DDR_REG(PWRCTL)=0x00000000;//exit self-refresh

    while((DDR_REG(STAT))!=0x00000001);

#elif defined(DDR3_1600_K)
    //*******config ddrc and phy sequence***************////////
    ///speed bin 1600_K 11-11-11////////
    //***********de-reset presetn0(ddrc presetn******************//)
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xa000000;

    DDR_REG(DBG1)=0x00000001;
    DDR_REG(MSTR)=0x00040401;
    DDR_REG(MRCTRL0)=0x00004010;
    DDR_REG(MRCTRL1)=0x0000f8f3;
    DDR_REG(PWRTMG)=0x00020000;

    DDR_REG(RFSHCTL0)=0x00a0f020;
    DDR_REG(RFSHTMG)=0x00610042;//tRFC=66clocks
    if((*((volatile int *)_FLAG_DDR_INIT))==0x0) {
        DDR_REG(INIT0)=0x40080003;
    } else {
        DDR_REG(INIT0)=0xc0080003;//power-up in self-refresh mode
        DDR_REG(PWRCTL)=0x00000020;//stay in self-refresh
    }

    DDR_REG(DRAMTMG0)=0x11101a0e;
    DDR_REG(DRAMTMG1)=0x000a0814;
    DDR_REG(DRAMTMG2)=0x00000509;
    DDR_REG(DRAMTMG3)=0x00002006;
    DDR_REG(DRAMTMG4)=0x01020306;
    DDR_REG(DRAMTMG5)=0x05090202;
    DDR_REG(DRAMTMG8)=0x00005e04;
    DDR_REG(DRAMTMG15)=0x00000000;

    DDR_REG(ZQCTL0)=0x41000040;//tZQoper from 0x80 to 0x100
    DDR_REG(ZQCTL1)=0x00000070;

    DDR_REG(DFITMG0)=0x03090107;
    DDR_REG(DFITMG1)=0x00030101;
    //  DDR_REG(DFILPCFG0)=0x0700b030;
    DDR_REG(DFILPCFG0)=0x07001101;//dfi_lp_en_sr=1(bit 8)
    DDR_REG(DFIUPD0)=0x20400005;
    DDR_REG(DFIUPD1)=0x001b0027;
    DDR_REG(DFIUPD2)=0x00000000;
    DDR_REG(DFIMISC)=0x00000001;
    DDR_REG(DFIPHYMSTR)=0x00000000;

    DDR_REG(ADDRMAP1)=0x00161616;//b2 b1 b0
    DDR_REG(ADDRMAP2)=0x00000000;//b5 b4 b3 b2
    DDR_REG(ADDRMAP3)=0x00000000;//b9 b8 b7 b6
    DDR_REG(ADDRMAP4)=0x00000f0f;//b11 b10(not used,so config f)
    DDR_REG(ADDRMAP5)=0x04040404;//r11 r10-r2 r1 r0
    DDR_REG(ADDRMAP6)=0x0f040404;//r15 (not used,so config f) r14 r13 r12

    DDR_REG(ODTCFG)=0x0b100f28;
    DDR_REG(ODTMAP)=0x00000010;

    DDR_REG(SCHED)=0x288c0503;//prefer_wr=1 pageclose=0
    DDR_REG(SCHED1)=0x00000000;

    DDR_REG(PERFHPR1)=0xdc000010;
    DDR_REG(PERFLPR1)=0x33000010;
    DDR_REG(PERFWR1)=0x64000010;

    DDR_REG(DBG0)=0x00000010;
    DDR_REG(DBG1)=0x00000000;
    DDR_REG(DBGCMD)=0x00000000;
    DDR_REG(SWCTL)=0x00000001;

    DDR_REG(PCCFG)=0x00000110;

    DDR_REG(PCFGR_0)=0x000063bd;
    DDR_REG(PCFGW_0)=0x000000ba;
    DDR_REG(PCFGR_1)=0x00003258;
    DDR_REG(PCFGW_1)=0x000023fd;
    DDR_REG(PCFGR_2)=0x00003050;
    DDR_REG(PCFGW_2)=0x000043f5;
    DDR_REG(PCFGR_3)=0x000030d5;
    DDR_REG(PCFGW_3)=0x00003226;
    DDR_REG(PCFGR_4)=0x000072b1;
    DDR_REG(PCFGW_4)=0x000062ce;
    DDR_REG(PCFGR_5)=0x000060b5;
    DDR_REG(PCFGW_5)=0x000040db;

    DDR_REG(PCTRL_0)=0x00000001;
    DDR_REG(PCTRL_1)=0x00000001;
    DDR_REG(PCTRL_2)=0x00000001;
    DDR_REG(PCTRL_3)=0x00000001;
    DDR_REG(PCTRL_4)=0x00000001;
    DDR_REG(PCTRL_5)=0x00000001;

    //**************de-reset all other reset *****************//
    //**************de-reset all other reset *****************//
    CHIP_CTRL_REG(0x40)=CHIP_CTRL_REG(0x40)|0xe000000;

    CHIP_CTRL_REG(0x3c)=CHIP_CTRL_REG(0x3c)|0xffff0000;

    DDR_REG(DBG1)=0x00000000;
    DDR_REG(SWCTL)=0x00000000;
    DDR_REG(DFIMISC)=0x00000000;
    PUB_REG(PUB_DCR)=0x0000048b;

    PUB_REG(PUB_PTR0)=0x10000c10;
    PUB_REG(PUB_PTR1)=0x00800080;
    PUB_REG(PUB_PTR2)=0x00080421;

    PUB_REG(PUB_PTR3)=0x0a100a46;//for reduce simulation time,config smaller value than 500us,which will lead to timing violation in ddr3 modle(ddr3.v)
    //  PUB_REG(PUB_PTR3)=0x0a14111d;//tDINIT0=500us=PUB_PTR3[19:0]*tCK

    PUB_REG(PUB_PTR4)=0x08000c00;//for reduce simulation time,config smaller value than 200us,which will lead to timing violation in ddr3 modle(ddr3.v)
    //  PUB_REG(PUB_PTR4)=0x0801a072;//tDINIT2=200us=PUB_PTR4[17:0]*tCK(reference PUB databook)

    PUB_REG(PUB_MR0)=0x00000d70;
    PUB_REG(PUB_MR1)=0x00000008;
    PUB_REG(PUB_MR2)=0x00000018;
    PUB_REG(PUB_MR3)=0x00000000;

    PUB_REG(PUB_DTPR0)=0x9d9cbb6f;
    PUB_REG(PUB_DTPR1)=0x1a840400;
    PUB_REG(PUB_DTPR2)=0x10021405;

    PUB_REG(PUB_PGCR1)=0x0200c461;
    PUB_REG(PUB_DSGCR)=0xf0046407;//rrmode=1
    //  PUB_REG(PUB_DSGCR)=0xf0006407;
    PUB_REG(PUB_PGCR2)=0x00f81860;

    if(*((volatile int *)_FLAG_DDR_INIT)==0x0) { //default do sdram initialization
        //*************wait PHY initialization Done*****************//
        while((PUB_REG(PUB_PGSR0))!=0x9000000f);
        //****************inform PHY to start initialization of SDRAM******//
        PUB_REG(PUB_PIR)=0x00000181;
        //***************wait SDRAM initialization Done*********//
        while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    } else {//do not initialize SDRAM when power up because SDRAM is in  self-refresh mode
        //*************wait PHY initialization Done*****************//
        while((PUB_REG(PUB_PGSR0))!=0x9000001f);
    }

#ifdef TRAIN_ON
    if(*((volatile int *)_FLAG_DDR_INIT)==0x0) { //default do sdram initialization
        //************data training configuration**********************//
        PUB_REG(PUB_DTCR)=PUB_REG(PUB_DTCR)&0xfffffff0|0x1;//DTRPTN=1
        DDR_REG(RFSHCTL3)=0x00000001;//disable auto-refresh
        PUB_REG(PUB_PIR)=0x0000fe01;//run Training
        //***************wait Training Done*********//
        while((PUB_REG(PUB_PGSR0))!=0x90000fff);
        DDR_REG(RFSHCTL3)=0x00000000;//enable auto-refresh
    }
#endif

    PUB_REG(PUB_DX0LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX1LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX2LCDLR2)=0x0000006e;
    PUB_REG(PUB_DX3LCDLR2)=0x0000006e;
    DDR_REG(DFIMISC)=0x00000001;
    DDR_REG(SWCTL)=0x00000001;
    while((DDR_REG(SWSTAT))!=0x00000001);

    *((volatile int *)_RETENTION_CTRL_REG)=0x0;
    *((volatile int *)_RETENTION_CTRL_REG)=0x0;//remove latch to SDRAM before exit self-refresh
    DDR_REG(PWRCTL)=0x00000000;//exit self-refresh

    while((DDR_REG(STAT))!=0x00000001);
#endif//speed bin
#endif//FPGA
    return 0;
}
