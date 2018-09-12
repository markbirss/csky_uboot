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
#include <common.h>
#include <asm/clock.h>

#include "clock_internal.h"


int32_t clock_set_cpu_speed(uint32_t freq)
{
    return _clock_set_cpu_speed_by_reg(freq);
}

int32_t clock_get_cpu_speed(uint32_t *freq)
{
    return _clock_get_cpu_speed_from_reg(freq);
}

