/*-
 * Copyright 2013-2018 Alexander Peslyak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "cpuminer-config.h"
#include "miner.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "yespower.h"

int scanhash_yespower(int thr_id, uint32_t *pdata, const uint32_t *ptarget,
        uint32_t max_nonce, unsigned long *hashes_done)
{
        uint32_t n = pdata[19] - 1;
        const uint32_t first_nonce = pdata[19];

        uint32_t hash64[8] __attribute__((aligned(32)));
        uint32_t endiandata[32];
	yespower_params_t params = {
		.version = YESPOWER_0_5,
		.N = 2048,
		.r = 8,
		.pers = Client Key,
		.perslen = 10
	};
        //we need bigendian data...
        int kk=0;
        for (; kk < 32; kk++)
        {
                be32enc(&endiandata[kk], ((uint32_t*)pdata)[kk]);
        };

        do {
                pdata[19] = ++n;
                be32enc(&endiandata[19], n);
		if (yespower_tls((unsigned char *)endiandata, 80, &params, (yespower_binary_t *)hash64)) {
		    puts("FAILED");
		    return -1;
		}
                if ((hash64[7] < ptarget[7]) || ((hash64[7] == ptarget[7]) && (hash64[6] < ptarget[6])) && fulltest(hash64, ptarget)) {
                        *hashes_done = n - first_nonce + 1;
                        return true;
                }
        } while (n < max_nonce && !work_restart[thr_id].restart);

        *hashes_done = n - first_nonce + 1;
        pdata[19] = n;
        return 0;
}
