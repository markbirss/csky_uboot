/*
 * Copyright (C) 2017 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <fdtdec.h>
#include <asm/io.h>
#include <asm/arch-eragon/timer.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SPL_BUILD
#define CK_TIMER0_BASSADDR (ERAGON_TIMER0_BASE - 0xa0000000)
#define CK_TIMER1_BASSADDR (ERAGON_TIMER1_BASE - 0xa0000000)
#define CK_TIMER2_BASSADDR (ERAGON_TIMER2_BASE - 0xa0000000)
#else
#define CK_TIMER0_BASSADDR ERAGON_TIMER0_BASE
#define CK_TIMER1_BASSADDR ERAGON_TIMER1_BASE
#define CK_TIMER2_BASSADDR ERAGON_TIMER2_BASE
#endif

#define CKTIMER_ADDR ((PCKPStruct_TIMER)ERAGON_TIMER0_BASE)
#define PCK_TIMER_CONTROL ((PCKStruct_TIMER_CON)(ERAGON_TIMER0_BASE + 0x8))
#define SYS_TIMER		((PCKPStruct_TIMER)timer[TIMER_ID])
#define TIMER_CLOCK		(SYSTEM_CLOCK * 1000 * 1000)
#define COUNT_TO_USEC(x)	((x) / SYSTEM_CLOCK)
#define USEC_TO_COUNT(x)	((x) * SYSTEM_CLOCK)
#define TICKS_PER_HZ		(TIMER_CLOCK / CONFIG_SYS_HZ)
#define TICKS_TO_HZ(x)		((x) / TICKS_PER_HZ)
#define TIMER_LOAD_VAL		0xffffffff

static volatile ulong timestamp = 0;

int timer[] = {
	CK_TIMER0_BASSADDR,
	CK_TIMER1_BASSADDR,
	CK_TIMER2_BASSADDR
};

static ulong read_timer(void)
{
	ulong time;
	time = SYS_TIMER->TxCurrentValue;
	return ~time;
}
/* init timer register */
int timer_init(void)
{
	gd->arch.tbl = 0;
	gd->arch.tbu = 0;
	/* load time (FULL) */
	SYS_TIMER->TxLoadCount = TIMER_LOAD_VAL;

	/*in user-defined running mode*/
	SYS_TIMER->TxControl |= CK_TIMER_TXCONTROL_MODE;

	/* enable the corresponding timer */
	SYS_TIMER->TxControl &= ~(CK_TIMER_TXCONTROL_ENABLE);
	SYS_TIMER->TxControl |= CK_TIMER_TXCONTROL_ENABLE;
	return 0;
}

ulong get_timer_masked(void)
{
	/* current tick value */
	ulong now = TICKS_TO_HZ(read_timer());

	if (now >= gd->arch.lastinc)	/* normal (non rollover) */
		gd->arch.tbl += (now - gd->arch.lastinc);
	else {
		/* rollover */
		gd->arch.tbl += (TICKS_TO_HZ(TIMER_LOAD_VAL)
				- gd->arch.lastinc) + now;
	}
	gd->arch.lastinc = now;

	return gd->arch.tbl;
}

/* timer without interrupts */
ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

/* delay x useconds */
void __udelay(unsigned long usec)
{
	long tmo = USEC_TO_COUNT(usec);
	ulong now, last = read_timer();

	while (tmo > 0) {
		now = read_timer();
		if (now > last)	/* normal (non rollover) */
			tmo -= now - last;
		else		/* rollover */
			tmo -= TIMER_LOAD_VAL - last + now;
		last = now;
	}
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}

unsigned long timer_get_us(void)
{
	return (get_timer(0) * 1000);
}

