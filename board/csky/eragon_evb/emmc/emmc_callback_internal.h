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

#ifndef __EMMC_CALLBACK__INTERNAL_H
#define __EMMC_CALLBACK__INTERNAL_H
#include "emmc_internal.h"

/* Definitions for cmd status */
#define CMD_STAT_ABSENT		    0
#define CMD_STAT_STARTED	    1
#define CMD_STATE_READRESP	    2
#define CMD_STATE_READDAT	    3
#define CMD_STATE_WRITEDAT	    4
#define CMD_COMMAND_DONE	    5
#define CMD_COMMAND_ABORTING	6
#define CMD_STATE_POLLD   	    7

emmc_postproc_callback emmc_get_post_callback(uint32_t);
emmc_preproc_callback emmc_get_pre_callback(uint32_t);
void emmc_set_data_trans_params(uint32_t slot, uint8_t *data_buffer,
                                uint32_t num_of_blocks, uint32_t epoch_count,
                                uint32_t flag, uint32_t custom_blocksize);

void emmc_set_current_task_status_t(uint32_t slot, uint32_t *resp_buffer,
                                    CK_UINT8 *data_buffer);

#endif /* end __EMMC_CALLBACK__INTERNAL_H */

