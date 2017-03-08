/*
 * Copyright (c) 2017 CSKY, Inc
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <fdtdec.h>
#include <asm/io.h>
#include <asm/arch-eragon/ck_timer.h>

DECLARE_GLOBAL_DATA_PTR;

#define  SYS_TIMER  ((PCKPStruct_TIMER)timer[TIMER_ID])

static volatile ulong timestamp = 0;

int timer[] = {
	CK_TIMER0_BASSADDR,
	CK_TIMER1_BASSADDR,
	CK_TIMER2_BASSADDR
};

#ifdef CONFIG_INTERRUPT_TIMER

static void cktimer_interrupt(void *arg);

int timer_init(void)
{
	int loadtime;
	timestamp = 0;

	/* load time(ms) */
	loadtime = (int)(TIMER_DEFAULT_FREQ / 100);
	SYS_TIMER->TxLoadCount = loadtime;

	/* register timer interrupt */
	irq_install_handler(TIME_IRQ,cktimer_interrupt,NULL);

	/* Enable Timer interrupt. */
	SYS_TIMER->TxControl &= ~(CK_TIMER_TXCONTROL_INTMASK);
	/*in user-defined running mode*/
	SYS_TIMER->TxControl |= CK_TIMER_TXCONTROL_MODE;
	/* enable the corresponding timer */
	SYS_TIMER->TxControl &= ~(CK_TIMER_TXCONTROL_ENABLE);
	SYS_TIMER->TxControl |= CK_TIMER_TXCONTROL_ENABLE;
	return 0;
}

static void cktimer_interrupt(void *arg)
{
	/* clear a timer interrupt by reading End of Interrupt register(EOI) */
	*((volatile int *)(&(SYS_TIMER->TxEOI)));
	/* add the clock count */
	timestamp++;
}

/*
 * Get timer current register offset (us).
 */
unsigned long csky_timer_offset(void)
{
	unsigned long trr, tcn, offset;
	tcn = SYS_TIMER->TxCurrentValue;
	trr = SYS_TIMER->TxLoadCount;

	tcn = trr - tcn;

	offset = tcn * 10000 / trr;
	return offset;
}

unsigned long timer_get_us(void)
{
	ulong usec;
	usec = get_ticks() * 10 * 1000 + csky_timer_offset();
	return usec;
}
/*
 * delay the timer
 */
void __udelay(unsigned long usec)
{
	/* tick be changed to us */
	ulong tick2usec = timer_get_us();

	ulong endtime = tick2usec + usec;

	/* loop till event */
	while(tick2usec < endtime)
	{
		tick2usec = timer_get_us();
	}
}

ulong get_timer(ulong base)
{
	ulong time;
	time = timer_get_us()/1000 - base;
	return time;
}

void reset_timer(void)
{
	timestamp = 0;
}

/*
 * return the ticks (1 tick = 10 ms)
 */
unsigned long long get_ticks(void)
{
	return timestamp;
}
#else
#define TIMER_CLOCK		(50 * 1000 * 1000)
#define COUNT_TO_USEC(x)	((x) / 50)
#define USEC_TO_COUNT(x)	((x) * 50)
#define TICKS_PER_HZ		(TIMER_CLOCK / CONFIG_SYS_HZ)
#define TICKS_TO_HZ(x)		((x) / TICKS_PER_HZ)

#define TIMER_LOAD_VAL		0xffffffff
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
#endif

