/* libsrs - Sender Rewriting Scheme library
*	
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  FILE: base.c
*  DESC: base encoding functions 
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

#include "srs.h"      /* libsrs structures / defines */
#include "main.h"     /* libsrs public functions */
#include "util.h"     /* libsrs utility */
#include "base.h"     /* libsrs base encoding/decoding */

/* BASE_64_encode
*
*  Author: Unknown (based on code taken from perl XS wrapper)
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Convert a string of chars from base10 to base64.  Allocates memory
*  which is assigned to 'buf' and returned.  Calling function is required to
*  free buf.
*
*/
u_char *BASE_10_64_s(const u_char *s)
{
  size_t        len;    /* length of s */
  int16_t       n;      /* number of bytes in s */
  
  const u_char  *eom;   /* end of string */
  u_char        *buf;   /* return buffer */
  u_char        *d;     /* working ptr */

  u_char        c1;     /* utility */
  u_char        c2;     /* utility */
  u_char        c3;     /* utility */
  
  if (s == NULL)
  {
    xeprintf("Called with a NULL char\n");
    return(NULL);
  }
  
  len = strlen((char *)s);
  xvprintf("BASE_10_64_s :: Called with (%s) : %i\n", s, len);
    
  eom = (s + 16);
  
  /* our hash is alwasy 23 chars, must add to len of s */
  n = (len + 7);
  
  buf = xmalloc(n);
  memset(buf, '\0', n);
  d = buf;
  
  xvprintf("BASE_10_64_s :: Allocated %i bytes of memory\n", n);
    
  while (1)
  {
    c1    = *s++;
    *d++  = BASE_64[c1>>2];
    
    if (s == eom)
    {
      *d++ = BASE_64[(c1 & 0x3) << 4];
      break;
    }
    
    c2    = *s++;
    c3    = *s++;
    *d++  = BASE_64[((c1 & 0x3) << 4) | ((c2 & 0xF0) >> 4)];
    *d++  = BASE_64[((c2 & 0xF) << 2) | ((c3 & 0xC0) >> 6)];
    *d++  = BASE_64[c3 & 0x3F];
  }
  
  *d = '\0';
  
  return(buf);
}


/* BASE_10_64_c
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   Returned char is 'c' converted from base10 to base64
*
*  base32 character set:
*
*  Dec Val Enc  Dec Val Enc  Dec Val Enc  Dec Val Enc
*  65   0  A    82  17  R   105  34  i   122  51  z
*  66   1  B    83  18  S   106  35  j    48  52  0
*  67   2  C    84  19  T   107  36  k    49  53  1
*  68   3  D    85  20  U   108  37  l    50  54  2
*  69   4  E    86  21  V   109  38  m    51  55  3
*  70   5  F    87  22  W   110  39  n    52  56  4
*  71   6  G    88  23  X   111  40  o    53  57  5 
*  72   7  H    89  24  Y   112  41  p    54  58  6
*  73   8  I    90  25  Z   113  42  q    55  59  7
*  74   9  J    97  26  a   114  43  r    56  60  8
*  75  10  K    98  27  b   115  44  s    57  61  9
*  76  11  L    99  28  c   116  45  t    43  62  +
*  77  12  M   100  29  d   117  46  u    47  63  /
*  78  13  N   101  30  e   118  47  v
*  79  14  O   102  31  f   119  48  w    (pad) =
*  80  15  P   103  32  g   120  49  x
*  81  16  Q   104  33  h   121  50  y    
*      
*/
u_char BASE_10_64_c(u_char c)
{
  if (c < 26)
  {
    return('A' + c);
  }
  if (c < 52)
  {
    return('a' + (c - 26));
  }
  if (c < 62)
  {
    return('0' + (c - 52));
  }
  if (c == 62)
  {
    return('+');
  }
    
  return('/');
}


/* BASE_64_decode_c
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   Returned char is 'c' converted from base64 to base10
*
*/
u_char BASE_64_10_c(u_char c)
{
  if (c >= 'A' && c <= 'Z')
  {
    return(c - 'A');
  }
  if (c >= 'a' && c <= 'z')
  {
    return(c - 'a' + 26);
  }
  if (c >= '0' && c <= '9')
  {
    return(c - '0' + 52);
  }
  if (c == '+')
  {
    return(62);
  }
  
  return(63);
}

/* BASE_32_10_c
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Convert a single char from base32 to base10.  This function quite
*  simply adds 32 to each character, which would then make it lower case, and
*  then subtracts 97 which is the integer value of the letter 'a', which then
*  leaves us with the integer corresponding to the base32 letter in our table.
*
*  base32 character set:
*
*  Dec Val Enc  Dec Val Enc  Dec Val Enc  Dec Val Enc
*  65   0  A    74   9  J    83  18  S    51  27  3
*  66   1  B    75  10  K    84  19  T    52  28  4
*  67   2  C    76  11  L    85  20  U    53  29  5
*  68   3  D    77  12  M    86  21  V    54  30  6
*  69   4  E    78  13  N    87  22  W    55  31  7
*  70   5  F    79  14  O    88  23  X
*  71   6  G    80  15  P    89  24  Y    (pad) =
*  72   7  H    81  16  Q    90  25  Z
*  73   8  I    82  17  R    50  26  2
*
*/
u_int8_t BASE_32_10_c(u_char c)
{
/*  u_int8_t r = (c + 32 - 97);*/

  /* 'c' isn't 2-7  or A-Z */
  if (c < 50 || c > 90)
  {
    return(BASE_32_INVALID);
  }
  
  /* 'c' is 2-7 */
  if (c >= 50 && c <= 55)
  {
    /* 26 letters in the alphabet */
    return(c - 26);
  }
  
  /* 'c' is A-Z */
  if (c >= 65 && c <= 90)
  {
    return(c + 32 - 97);
  }
  
  return(BASE_32_INVALID);
}

/* BASE_32_10_c
*
*  Author: James Couzens <jcouzens@obscurity.org>
*
*  Date:   01/08/04
*
*  Desc:   
*          Convert a single char from base32 to base10.  This function quite
*  simply adds 32 to each character, which would then make it lower case, and
*  then subtracts 97 which is the integer value of the letter 'a', which then
*  leaves us with the integer corresponding to the base32 letter in our table.
*
*  base32 character set:
*/
u_char BASE_32_itoc(u_int8_t c)
{
  u_char r;

  if (c > 25)
  {
    r = (c - 26 + 0x32);
    xprintf("BASE_32_itoc :: %u converts to %c\n", c, r);

    return(r);
  }
  
  r = (c + 0x61);
  xprintf("BASE_32_itoc :: %u converts to %c\n", c, r);
  
  return(r);
}

u_int8_t BASE_32_ctoi(u_char c)
{
  u_char r;
  
  if (c < 0x61)
  {
    return ((r = (c + 26 - 0x32)));
  }
  return((r = (c - 0x61)));
}

/* end base64.c */
