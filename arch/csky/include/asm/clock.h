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
#ifndef __CLOCK_H__
#define __CLOCK_H__
#include <common.h>
#include <asm/datatype.h>
typedef enum {
    CLOCK_CPU_FREQ_1000M= 1000*1000*1000,
    CLOCK_CPU_FREQ_950M = 950*1000*1000,
    CLOCK_CPU_FREQ_940M = 940*1000*1000,
    CLOCK_CPU_FREQ_930M = 930*1000*1000,
    CLOCK_CPU_FREQ_920M = 920*1000*1000,
    CLOCK_CPU_FREQ_910M = 910*1000*1000,
    CLOCK_CPU_FREQ_900M = 900*1000*1000,
    CLOCK_CPU_FREQ_850M = 850*1000*1000,
    CLOCK_CPU_FREQ_800M = 800*1000*1000,
    CLOCK_CPU_FREQ_700M = 700*1000*1000,
    CLOCK_CPU_FREQ_600M = 600*1000*1000,
    CLOCK_CPU_FREQ_400M = 400*1000*1000,
    CLOCK_CPU_FREQ_300M = 300*1000*1000,
    CLOCK_CPU_FREQ_200M = 200*1000*1000,
    CLOCK_CPU_FREQ_150M = 150*1000*1000,
    CLOCK_CPU_FREQ_100M = 100*1000*1000,
    CLOCK_CPU_FREQ_75M  = 75*1000*1000,

    CLOCK_CPU_FREQ_24M  = 24*1000*1000,
    CLOCK_CPU_FREQ_12M  = 12*1000*1000,
    CLOCK_CPU_FREQ_3M   = 3*1000*1000,
    CLOCK_CPU_FREQ_375K = 375*1000,
    CLOCK_CPU_FREQ_46875= 46875,
} clock_cpu_freqs_e;

#define CLOCK_CPU_SPEED_MODE_DEFAULT 0
#define CLOCK_CPU_SPEED_MODE_HIGH    1

int32_t clock_set_cpu_speed_mode(int speed_mode);
int32_t clock_get_cpu_speed_mode(int *speed_mode);

int32_t clock_set_cpu_speed(uint32_t  freq);
int32_t clock_get_cpu_speed(uint32_t *freq);

#endif

