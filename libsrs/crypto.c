/* libsrs - Sender Rewriting Scheme library
*	
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: crypto.c
*  DESC: cryptography and encoding related functions 
*
*  License:
*
*  The libspf Software License, Version 1.0
*
*  Copyright (c) 2004 James Couzens.  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.
*
*  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
*  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
*  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
*  ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
*  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
*  SUCH DAMAGE.
* 
*/

#include <stdio.h>          /* printf */
#include <time.h>           /* time */
#include <sys/time.h>       /* gettimeofday */
#include <openssl/hmac.h>   /* HMAC */

#include "srs.h"
#include "crypto.h"
#include "util.h"
#include "base.h"

#ifndef EVP_MAX_MD_SIZE
#define EVP_MAX_MD_SIZE (16+20) /* The SSLv3 md5+sha1 type */
#endif

/* CRYPTO_hmac_sha1
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          The contents of 'a', 'b', and 'c' are used to create an HMAC which
*  is 'locked' using 'secret'.  Upon success said digest is then base64 encoded
*  as a form of compression and then returned, upon failure NULL is returned.
*
*  Memory:
*          'buf' is allocated memory (via BASE_10_64_s())  Calling function is
*  expected to free this memory.
*
*/
u_char *CRYPTO_hmac_sha1(const char *secret, const int32_t s_len,
  const u_char *data, const int32_t d_len)
{
  HMAC_CTX      ctx;        /* HMAC structure */
  const EVP_MD  *md;        /* signature type */

  u_char        *digest;    /* digest */
  u_int32_t     di_len;      /* length of digest */

  u_char        *buf;       /* return buffer */
  
  xprintf("Called with: (%s) %i (%s) %i\n",
    secret, s_len, data, d_len);

  /* start timer for HMAC digest generation */
  /*ftime(&srs_time[1].start);*/
  gettimeofday(&srs_time[1].start, &srs_time[1].tzp);
      
  HMAC_CTX_init(&ctx);
  
  md      = EVP_sha1();
  digest  = xmalloc(EVP_MAX_MD_SIZE);
  memset(digest, '\0', EVP_MAX_MD_SIZE);

  HMAC_Init(&ctx, secret, s_len, md);
  HMAC_Update(&ctx, data, d_len);
  HMAC_Final(&ctx, digest, &di_len);
  HMAC_CTX_cleanup(&ctx);
  
  /*digest = HMAC(EVP_sha1(), secret, s_len, s, n, md, NULL);*/
  xvprintf("digest is (%s)\n", digest);
  
  /* halt timer for HMAC digest generation */
  /*ftime(&srs_time[1].finish);*/
  gettimeofday(&srs_time[1].finish, &srs_time[1].tzp);
  xpprintf("SHA1 HMAC created in %lu.%u seconds\n",
    (srs_time[2].finish.tv_sec    - srs_time[2].start.tv_sec),
    (u_int8_t)(srs_time[2].finish.tv_usec - srs_time[2].start.tv_usec));
  
  /* start timer for Base64 encoding */ 
  /*ftime(&srs_time[2].start);*/
  gettimeofday(&srs_time[2].start, &srs_time[2].tzp);

  /* base64 encode the digest 22 + 1 */
  buf = BASE_10_64_s(digest);

  /* halt timer for Base64 encoding */  
  /*ftime(&srs_time[2].finish);*/
  gettimeofday(&srs_time[2].finish, &srs_time[2].tzp);
  xpprintf("Base64 Encoded digest in %lu.%u seconds\n",
    (srs_time[2].finish.tv_sec    - srs_time[2].start.tv_sec),
    (u_int8_t)(srs_time[2].finish.tv_usec - srs_time[2].start.tv_usec));

  xvprintf("final digest: (%s)\n", buf);
  
  xfree(digest);
  return(buf);    
}

/* end crypto.c */
